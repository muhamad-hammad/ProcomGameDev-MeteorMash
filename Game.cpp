#include "Game.h"

void Game::initVariables()
{
	this->endgame = false;
	this->window = nullptr;

	this->points = 0;
	this->health = 50;
	this->enemySpawnTimerMax = 75.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 10;
	this->MouseHold = false;
	this->backgroundVelocity = 1.f;
}
void Game::initWindow()
{
	this->videomode.height = 600;
	this->videomode.width = 800;
	this->window = new sf::RenderWindow(sf::VideoMode(1400,800), "Game1", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(144);

	this->backgroundView.setSize(this->window->getSize().x, this->window->getSize().y);
	this->backgroundView.setCenter(this->window->getSize().x / 2, this->window->getSize().y / 2);;
	//this->backgroundView.zoom(1.f);
}
void Game::initFonts()
{
	if (this->font.loadFromFile("Fonts.ttf"))
	{
		std::cout << "ERROR: initfonts Failed to load" << std::endl;
	}
}
void Game::inittext()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(18);
	this->uiText.setFillColor(sf::Color::Red);
	this->uiText.setString("NONE");

	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(24);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("GAME OVER!");
	this->gameOverText.setPosition(800 / 2.f, 600 / 2.f);

	this->tryagainText.setFont(this->font);
	this->tryagainText.setCharacterSize(24);
	this->tryagainText.setFillColor(sf::Color::Red);
	this->tryagainText.setString("Try again!");
	this->tryagainText.setPosition(800 / 2.f, 600 / 2.f + 30.f);
}
void Game::initEnemies()
{
	this->enemies.clear();
	if (!this->asteroidTexture.loadFromFile("asteroid01.png"))
	{
		std::cout << "ERROR: Could not load asteroid texture" << std::endl;
	}
	else
	{
		this->enemy.setTexture(&this->asteroidTexture); // Set the texture only if it's loaded successfully
	}
	this->enemy.setPosition(10.0f, 10.0f);
	this->enemy.setSize(sf::Vector2f(50.f, 50.f));
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));// reduces the size of enemy by half (initial value of scale is 1) so it becomes 0.5*1
	this->enemy.setTexture(&this->asteroidTexture); // Use the asteroid texture
	this->enemy.setOutlineThickness(1.f);
	this->enemy.setOutlineColor(sf::Color::Black);
}
void Game::initBackground()
{
	if (!backgroundTexture.loadFromFile("space2.jpg"))
	{
		std::cout << "ERROR loading background" << std::endl;
	}
	// Initialize the first background sprite
	backgroundSprite1.setTexture(backgroundTexture);
	backgroundSprite1.setScale(
		float(window->getSize().x) / backgroundTexture.getSize().x,
		float(window->getSize().y) / backgroundTexture.getSize().y);
	// Initialize the second background sprite, positioned above the first
	backgroundSprite2.setTexture(backgroundTexture);
	backgroundSprite2.setScale(backgroundSprite1.getScale());
	backgroundSprite2.setPosition(0, -static_cast<float>(window->getSize().y));
}
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->inittext();
	this->initEnemies();
	this->initBackground();
}
Game::~Game()
{
	delete this->window;
}
const bool Game::running() const
{
	return this->window->isOpen();
}
const bool Game::getEndgame() const
{
	return this->endgame;
}
void Game::spawnEnemy()
{
	this->enemy.setTexture(&this->asteroidTexture);
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);
	int type = rand() % 5;
	float scale;
	switch (type)
	{
	case 0:
		scale = 0.2f;
		break;
	case 1:
		scale = 0.4f;
		break;
	case 2:
		scale = 0.6f;
		break;
	case 3:
		scale = 0.8f;
		break;
	case 4:
		scale = 1.0f;
		break;
	default:
		scale = 1.2f;
		break;
	}
	float baseSizeX = 272; // Assuming your asteroid.png has a base size of 100x100
	float baseSizeY = 184;
	this->enemy.setSize(sf::Vector2f(baseSizeX * scale, baseSizeY * scale));
	this->enemies.push_back(this->enemy);
}
void Game::PollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::R && this->endgame)
			{
				this->initVariables();
				this->inittext();
				this->initEnemies();
			}
			break;
		}
	}
}
void Game::UpdateMousePos()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points << std::endl << "health: " << this->health << std::endl;
	this->uiText.setString(ss.str());
}
//moving and upadating the enemy
void Game::UpdateEnemies()
{
	// Updating the timer for enemy spawning
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
	{
		// Spawn the enemy and reset the timer
		this->spawnEnemy();
		this->enemySpawnTimer = 0.f;
	}
	else
	{
		this->enemySpawnTimer += 1.f;
	}
	// Move the enemies and check for off-screen enemies
	for (int i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].move(0.f, 3.f);
		// If the enemy goes off-screen, remove it and decrease health
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			break; // Break out of the loop to avoid vector index issues after erase
		}
	}
	// Check if the left mouse button was pressed and released (click)
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->MouseHold == false)
		{
			this->MouseHold = true;
			bool hit = false;

			// Check if an enemy was clicked
			for (size_t i = 0; i < this->enemies.size() && !hit; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
					{
						this->points += 10;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
					{
						this->points += 7;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
					{
						this->points += 5;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
					{
						this->points += 3;
					}
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
					{
						this->points += 1;
					}
					// If enemy was clicked, remove it and increase points
					hit = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}// If no enemy was clicked and it is a new click, decrease health by 1
			if (!hit)
			{
				this->health -= 1;
			}
		}
	}
	else
	{
		this->MouseHold = false;
	}
}
void Game::Update()
{
	this->PollEvents();
	this->UpdateMousePos();
	this->updateText();

	if (this->health <= 0)
	{
		this->endgame = true;
	}// Always update the mouse position and the text to reflect the current score and health
	if (!this->endgame)
	{ // Move both background sprites down
		backgroundSprite1.move(0, backgroundVelocity);
		backgroundSprite2.move(0, backgroundVelocity);
		// Check if the first background sprite has moved completely off the bottom of the screen
		if (backgroundSprite1.getPosition().y >= window->getSize().y)
		{
			backgroundSprite1.setPosition(0, -static_cast<float>(window->getSize().y));
		}
		// Check if the second background sprite has moved completely off the bottom of the screen
		if (backgroundSprite2.getPosition().y >= window->getSize().y)
		{
			backgroundSprite2.setPosition(0, -static_cast<float>(window->getSize().y));
		}
		this->UpdateEnemies();
	}
	else
	{// Game over conditions
		this->gameOverText.setPosition(sf::Vector2f(800 / 2.0f - this->gameOverText.getGlobalBounds().width / 2.0f, 600 / 2.0f - this->gameOverText.getGlobalBounds().height));
		this->tryagainText.setPosition(sf::Vector2f(800 / 2.0f - this->tryagainText.getGlobalBounds().width / 2.0f, 600 / 2.0f + 30.f));
	}
}
void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}
void Game::RenderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}
void Game::Render()
{
	this->window->clear();
	this->window->draw(backgroundSprite1);
	this->window->draw(backgroundSprite2);
	if (!this->endgame)
	{
		this->renderText(*this->window);
		this->RenderEnemies(*this->window);
	}
	else
	{// Draw game over and try again texts
		this->window->draw(this->gameOverText);
		this->window->draw(this->tryagainText);
	}
	this->window->display();
}
//the cod
#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<vector>
#include<ctime>
#include<sstream>
/*
* class that acts as the game engine
* wrapper class...
*/

#pragma once
class Game
{
private:
	//private variables
	// window

	// Existing member variables...
	sf::Texture asteroidTexture;
	std::vector<sf::Sprite> asteroids;

	sf::RenderWindow *window;

	sf::Event ev;
	sf::VideoMode videomode;
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	//resources
	sf::Font font;

	//Text
	sf::Text uiText;
	sf::Text gameOverText;
	sf::Text tryagainText;

	//Mouse Positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//View
	sf::View backgroundView;
	sf::Sprite backgroundSprite1;
	sf::Sprite backgroundSprite2;

	// Game Logic
	bool endgame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool MouseHold;
	float backgroundVelocity;
	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	//privte functions 
	void initVariables();
	void initWindow();
	void initFonts();
	void inittext();
	void initEnemies();
	void initBackground();
	void initAudio();
public: 
	// constructors and destructors
	Game();
	virtual ~Game();

	// accessors

	const bool running() const;
	const bool getEndgame() const;

	//Functions
	void spawnEnemy();
	void PollEvents();
	void UpdateMousePos();
	void updateText();
	void UpdateEnemies();
	void Update();
	void renderText(sf:: RenderTarget& target);
	void RenderEnemies(sf::RenderTarget& target);
	void Render();
};


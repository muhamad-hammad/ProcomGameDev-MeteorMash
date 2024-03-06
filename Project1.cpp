#include<iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "Game.h"

int main()
{
    // Init rand
    std::srand(static_cast<unsigned> (time(NULL)));
    Game game;
    while (game.running())
    {
        //update
        game.Update();
        //render
        game.Render();
    }
}
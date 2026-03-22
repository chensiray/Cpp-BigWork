#pragma once
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<functional>
#include<string>
#include<vector>
#include<random>
#include<numeric>
#include<iostream>
extern sf::Font font;
extern sf::Color button_hovered;
extern sf::Texture heartTexture;
extern sf::Texture musicTexture;
extern sf::Texture pauseTexture;
extern sf::Texture awardTexture;
extern sf::Time gameTime;
extern std::random_device rd;
extern std::mt19937 gen;
extern const float pi;
extern sf::Music musicmain;
extern sf::Music musiclevel1;
extern sf::Music musiclevel2;
extern sf::Music musiclevel3;
extern sf::Music musiclevel4;
void updateViewport(const sf::RenderWindow& window, sf::View& gameview);
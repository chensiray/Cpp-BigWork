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
extern sf::Texture heartTexture;
extern sf::Texture musicTexture;
extern sf::Texture pauseTexture;
extern sf::Texture awardTexture;
extern sf::Texture clockTexture;
extern sf::Texture shieldTexture;
extern sf::Texture speedTexture;
extern sf::Texture clearTexture;
extern sf::Texture coinTexture;
extern sf::Texture texture[6];
extern sf::Time gameTime;
extern std::random_device rd;
extern std::mt19937 gen;
extern const float pi;
extern sf::Music musicmain;
extern sf::Music musiclevel1;
extern sf::Music musiclevel2;
extern sf::Music musiclevel3;
extern sf::Music musiclevel4;
extern sf::String Back;
extern sf::String Bingo;
extern sf::String heart;
extern sf::String clocktext;
extern sf::String clear;
extern sf::String shield;
extern sf::String speed;
extern sf::String cointext;
void updateViewport(const sf::RenderWindow& window, sf::View& gameview);
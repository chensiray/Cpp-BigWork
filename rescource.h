#pragma once
#include<SFML/Graphics.hpp>
#include<functional>
#include<string>
#include<vector>
#include<random>
#include<numeric>
#include<iostream>
extern sf::Font font;
extern sf::Color button_default;
extern sf::Color button_hovered;
extern sf::Texture heartTexture;
extern sf::Time gameTime;
extern std::random_device rd;
extern std::mt19937 gen;
extern const float pi;
void updateViewport(const sf::RenderWindow& window, sf::View& gameview);
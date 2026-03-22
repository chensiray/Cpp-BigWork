#pragma once
#include"rescource.h"
class Button :public sf::Drawable
{
public:
	Button(const sf::String& content, const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color);
	bool headleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	void update(const sf::Vector2f& mousePos);
	void setText(const sf::String& text);
private:
	sf::RectangleShape background;
	sf::Text text = sf::Text(font);
	sf::Color color;
	bool isHovered;
	sf::Time birthTime;
	sf::Time animationDuration;
	bool isAnimating;
};

class MusicButton :public Button
{
public:
	MusicButton(const sf::Vector2f& size, const sf::Vector2f& position);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool headleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void update();
private:
	sf::Sprite self = sf::Sprite(musicTexture);
	bool ismusic;
};
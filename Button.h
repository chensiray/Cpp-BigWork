#pragma once
#include"rescource.h"
class Button :public sf::Drawable
{
public:
	Button(const sf::String& content,const sf::Vector2f &size, const sf::Vector2f& position);
	bool headleEvent(const sf::Event& event, const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	void update(const sf::Vector2f& mousePos);
private:
	sf::RectangleShape background;
	sf::Text text = sf::Text(font);
	bool isHovered;
	sf::Time birthTime;
	sf::Time animationDuration;
	bool isAnimating;
};
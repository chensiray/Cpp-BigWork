#pragma once
#include"rescource.h"
class Player :public sf::Drawable
{
public:
	Player(const int& life, const float& x, const float& y);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	void hurt();
	bool alive()const;
	void addlife(const int& cnt);
	int getlife()const;
	sf::FloatRect getBounds()const;
	sf::Vector2f getPosition()const;
private:
	sf::RectangleShape self = sf::RectangleShape({ 20.f,20.f });
	int life;
	bool isimmune;
	sf::Time immuneBirthTime;
	std::vector<sf::Sprite*> hearts;
};
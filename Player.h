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
	void addspeed(const float& rate);
	void shield();
	sf::FloatRect getBounds()const;
	sf::Vector2f getPosition()const;
	int havecoin;
	int haveclear;
	int haveclock;
	int haveshield;
private:
	sf::RectangleShape self = sf::RectangleShape({ 20.f,20.f });
	float speed;
	int life;
	bool isimmune;
	bool isshield;
	sf::Time immuneBirthTime;
	sf::Time shieldBirthTime;
	std::vector<sf::Sprite> hearts = { sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture) };
};
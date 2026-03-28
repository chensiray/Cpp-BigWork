#pragma once
#include"rescource.h"
#include"ParticleSystem.h"
#include"Player.h"
class Item :sf::Drawable
{
public:
	Item(const sf::Vector2f& position, const int& type, const float& life);
	~Item();
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	void update();
	bool isactive;
	void checkCollision(Player& player);
private:
	sf::Sprite self = sf::Sprite(heartTexture);
	ParticleSystem* particle;
	sf::Time birthTime;
	float life;
	int type;
};
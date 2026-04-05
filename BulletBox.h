#pragma once
#include "Bullet.h"
class BulletBox
{
public:
	BulletBox(const sf::Vector2f& position, std::vector<std::unique_ptr<Bullet>> &bullets);
	void setPosition(const sf::Vector2f& position);
	void round(const float& size, const int& type, const float& r);
	void follow(const float& size, const int& type,Player *const target);
	void sector(const float& size, const int& type,const float& angle);
	void randomfall(const sf::Vector2f& size, const int& type,const float& angle);
	void shoot(const float& size, const int& type, const float& angle);
	void boom(const float& size, const int& type, const float& angle, const float& wait, const int& count);
	void plane(const sf::Vector2f& size, const int& type, const float& angle, const float& wait);
	void serpentine(const float& size, const int& type, const float& angle);
private:
	sf::Vector2f position;
	std::vector<std::unique_ptr<Bullet>>* bullets;
};
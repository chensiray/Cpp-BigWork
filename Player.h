#pragma once
#include"resource.h"
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
	void addspeed();
	void shield();
	sf::FloatRect getBounds()const;
	sf::Vector2f getPosition()const;
	int havecoin;
	int haveclear;
	int haveclock;
	int haveshield;
private:
	sf::RectangleShape self = sf::RectangleShape({ 20.f,20.f });
	sf::CircleShape glow;
	sf::Vector2f speed;
	std::deque<sf::Vector2f> trail;
	sf::VertexArray trailvertics;
	int trailmax;
	float m_acceleration;
	float maxspeed;
	float friction;
	int life;
	bool isimmune;
	bool isshield;
	sf::Clock glowClock;
	sf::Time immuneBirthTime;
	sf::Time shieldBirthTime;
	std::vector<sf::Sprite> hearts = { sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture), sf::Sprite(heartTexture) };
};
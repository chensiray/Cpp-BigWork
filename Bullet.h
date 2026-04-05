#pragma once
#include"resource.h"
#include"player.h"
class BulletBox;
class Bullet :public sf::Drawable
{
public:
	Bullet(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const int& type);
	virtual void update() = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override = 0;
	virtual bool checkCollision(const sf::FloatRect& playerBounds)const = 0;
	float getv()const;
	float getTime()const;
	void restart();
	std::deque<sf::Vector2f> trail;
	sf::VertexArray trailvertics;
	int trailmax;
	bool isactive;
	static int count;
private:
	sf::RectangleShape self;
	sf::Time birthTime;
	float velocity;
	int type;
};

class RoundBullet :public Bullet
{
public:
	RoundBullet(const float& radius, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type);
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool checkCollision(const sf::FloatRect& playerBounds)const override;
	void update()override;
private:
	sf::CircleShape self;
	float angle;
};

class TriangleBullet :public Bullet
{
public:
	TriangleBullet(const float& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const int& type, Player* target);
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool checkCollision(const sf::FloatRect& playerBounds)const override;
	void update()override;
private:
	sf::CircleShape self = sf::CircleShape(40.f, 3);
	float angle;
	bool isfollow;
	Player* target;
};

class RectangleBullet :public Bullet
{
public:
	RectangleBullet(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type, const float& wait);
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool checkCollision(const sf::FloatRect& playBounds)const override;
	void update()override;
private:
	sf::RectangleShape self;
	sf::Color color;
	float angle;
	float wait;
	bool ismove;
};

class BoomBullet :public Bullet
{
public:
	BoomBullet(const float& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type, const float& wait, BulletBox* boxptr);
	~BoomBullet();
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool checkCollision(const sf::FloatRect& playBounds)const override;
	void update()override;
private:
	sf::CircleShape self;
	sf::Color color;
	BulletBox* box;
	float angle;
	float wait;
};

class PlaneBullet :public Bullet
{
public:
	PlaneBullet(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type, const float& wait,BulletBox* boxptr);
	~PlaneBullet();
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool checkCollision(const sf::FloatRect& playBounds)const override;
	void update()override;
private:
	sf::RectangleShape self;
	sf::Color color;
	float angle;
	float wait;
	BulletBox* box;
};

class SerpentineBullet :public Bullet
{
public:
	SerpentineBullet(const float& radius, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type);
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool checkCollision(const sf::FloatRect& playerBounds)const override;
	void update()override;
private:
	sf::CircleShape self;
	float angle;
};
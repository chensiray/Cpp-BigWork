#include"Bullet.h"
#include"BulletBox.h"
#include"Player.h"
int Bullet::count = 0;
Bullet::Bullet(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const int& type)
{
	this->self.setPosition(position);
	this->self.setSize(size);
	this->self.setFillColor(color);
	this->trailvertics.setPrimitiveType(sf::PrimitiveType::Points);
	this->velocity = velocity;
	this->isactive = true;
	this->type = type;
	this->birthTime = gameTime;
	this->trailmax = 20;
	this->count++;
}
float Bullet::getv()const
{
	if (this->type == 1)
	{
		return this->velocity;
	}
	if (this->type == 2)
	{
		float elapse = this->getTime() / 0.4f;
		return this->velocity * exp(1 - elapse) + 4.f;
	}
	if (this->type == 3)
	{
		float elapse = this->getTime() / 1.2f;
		return this->velocity * exp(elapse) + 3.f;
	}
	return 0;
}
float Bullet::getTime()const
{
	return (gameTime - this->birthTime) / sf::seconds(1.f);
}
void Bullet::restart()
{
	this->birthTime = gameTime;
}
//RoundBullet
RoundBullet::RoundBullet(const float& radius, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type) :Bullet({radius,radius}, position, color, velocity, type)
{
	this->self.setPosition(position);
	this->self.setRadius(radius);
	this->self.setOrigin({ radius,radius });;
	this->self.setFillColor(color);
	this->angle = angle;
}
void RoundBullet::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->trailvertics, states);
	target.draw(this->self, states);
}
bool RoundBullet::checkCollision(const sf::FloatRect& playerBounds)const
{
	float dx = this->self.getPosition().x - std::max(playerBounds.position.x, std::min(this->self.getPosition().x, playerBounds.position.x + playerBounds.size.x));
	float dy = this->self.getPosition().y - std::max(playerBounds.position.y, std::min(this->self.getPosition().y, playerBounds.position.y + playerBounds.size.y));
	return (dx * dx + dy * dy) <= this->self.getRadius() * this->self.getRadius();
}
void RoundBullet::update()
{
	sf::Vector2f delta = { std::uniform_real_distribution<float>(-10.f,10.f)(gen),std::uniform_real_distribution<float>(-10.f,10.f)(gen) };
	this->trail.push_front(this->self.getPosition() + delta);
	if (trail.size() > this->trailmax) trail.pop_back();
	this->trailvertics.clear();
	sf::Color color({ 255,0,0,255 });
	for (const auto& pos : trail)
	{
		trailvertics.append(sf::Vertex(pos, color));
		color.a -= static_cast<std::uint8_t>(1.f / this->trailmax * 255);
	}
	float v = this->getv();
	this->self.move({ v * cos(this->angle), v * sin(this->angle) });
	if (this->self.getPosition().x < -100 || this->self.getPosition().x > 2000 || this->self.getPosition().y < -100 || self.getPosition().y > 1200)
	{
		this->isactive = false;
	}
}
//TriangleBullet
TriangleBullet::TriangleBullet(const float& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const int& type, Player *target) :Bullet({ size,size }, position, color, velocity, type)
{
	this->self.setPosition(position);
	this->self.setRadius(size / 2);
	this->self.setOrigin({ size / 2.f,size / 2.f });
	this->self.setFillColor(color);
	this->trailmax = 30;
	this->target = target;
	this->angle = 0;
	this->isfollow = true;
}
void TriangleBullet::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->trailvertics, states);
	target.draw(this->self, states);
}
bool TriangleBullet::checkCollision(const sf::FloatRect& playerBounds)const
{
	float dx = this->self.getPosition().x - std::max(playerBounds.position.x, std::min(this->self.getPosition().x, playerBounds.position.x + playerBounds.size.x));
	float dy = this->self.getPosition().y - std::max(playerBounds.position.y, std::min(this->self.getPosition().y, playerBounds.position.y + playerBounds.size.y));
	return (dx * dx + dy * dy) <= this->self.getRadius() * this->self.getRadius();
}
void TriangleBullet::update()
{
	sf::Vector2f delta = { std::uniform_real_distribution<float>(-10.f,10.f)(gen),std::uniform_real_distribution<float>(-10.f,10.f)(gen) };
	this->trail.push_front(this->self.getPosition() + delta);
	if (trail.size() > this->trailmax) trail.pop_back();
	this->trailvertics.clear();
	sf::Color color({ 255,0,0,255 });
	for (const auto& pos : trail)
	{
		trailvertics.append(sf::Vertex(pos, color));
		color.a -= static_cast<std::uint8_t>(1.f / this->trailmax * 255);
	}
	float v = this->getv();
	sf::Vector2f to = this->target->getPosition();
	float dx = to.x - this->self.getPosition().x;
	float dy = to.y - this->self.getPosition().y;
	if (this->isfollow)
	{
		this->angle = atan2(dy, dx);
	}
	if (dx * dx + dy * dy < 30000.f || this->getTime() > 3.f)
	{
		this->isfollow = false;
	}
	this->self.move({ v * cos(this->angle),v * sin(this->angle) });
	this->self.rotate(sf::degrees(12));
	if (this->self.getPosition().x < -100 || this->self.getPosition().x > 2000 || this->self.getPosition().y < -100 || self.getPosition().y > 1200)
	{
		this->isactive = false;
	}
}
//RectangleBullet
RectangleBullet::RectangleBullet(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type, const float& wait) :Bullet(size, position, color, velocity, type)
{
	this->self.setPosition(position);
	this->self.setSize(size);
	this->self.setFillColor(color);
	this->self.setOrigin(size / 2.f);
	this->trailmax = 300;
	this->color = color;
	this->angle = angle;
	this->wait = wait;
	this->ismove = false;
}
void RectangleBullet::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->trailvertics, states);
	target.draw(this->self, states);
}
bool RectangleBullet::checkCollision(const sf::FloatRect& playerBounds)const
{
	sf::FloatRect bulletRect = this->self.getGlobalBounds();
	if (const std::optional intersection = bulletRect.findIntersection(playerBounds))
	{
		return true;
	}
	return false;
}
void RectangleBullet::update()
{
	if (this->ismove)
	{
		sf::Vector2f delta = { std::uniform_real_distribution<float>(-10.f,10.f)(gen),std::uniform_real_distribution<float>(-30.f,30.f)(gen) };
		this->trail.push_front(this->self.getPosition() + delta);
		delta = { std::uniform_real_distribution<float>(-10.f,10.f)(gen),std::uniform_real_distribution<float>(-30.f,30.f)(gen) };
		this->trail.push_front(this->self.getPosition() + delta);
		while (trail.size() > this->trailmax) trail.pop_back();
		this->trailvertics.clear();
		sf::Color color({ 255,0,0,255 });
		for (const auto& pos : trail)
		{
			trailvertics.append(sf::Vertex(pos, color));
			color.a -= static_cast<std::uint8_t>(1.f / this->trailmax * 255);
		}
		float v = this->getv();
		this->self.move({ v * cos(this->angle), v * sin(this->angle) });
		if (this->self.getPosition().x < -100 || this->self.getPosition().x > 2000 || this->self.getPosition().y < -100 || self.getPosition().y > 1200)
		{
			this->isactive = false;
		}
	}
	else
	{
		float rest = this->wait - this->getTime();
		if (rest > 0.f && rest < 0.75f)
		{
			this->self.setFillColor(this->color);
			if (sin(rest * 8 * pi) > 0)
			{
				this->self.setFillColor(sf::Color::White);
			}
		}
		if (rest <= 0.f)
		{
			this->self.setFillColor(this->color);
			this->ismove = true;
			this->restart();
		}
	}
}
//BoomBullet
BoomBullet::BoomBullet(const float& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type, const float& wait, BulletBox* boxptr) :Bullet({ size,size }, position, color, velocity, type)
{
	this->box = boxptr;
	this->self.setPosition(position);
	this->self.setRadius(size / 2.f);
	this->self.setOrigin({ size / 2.f,size / 2.f });
	this->self.setFillColor(color);
	this->trailmax = 50;
	this->angle = angle;
	this->color = color;
	this->wait = wait;
}
BoomBullet::~BoomBullet()
{
	delete this->box;
}
void BoomBullet::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->trailvertics, states);
	target.draw(this->self, states);
}
bool BoomBullet::checkCollision(const sf::FloatRect& playerBounds)const
{
	float dx = this->self.getPosition().x - std::max(playerBounds.position.x, std::min(this->self.getPosition().x, playerBounds.position.x + playerBounds.size.x));
	float dy = this->self.getPosition().y - std::max(playerBounds.position.y, std::min(this->self.getPosition().y, playerBounds.position.y + playerBounds.size.y));
	return (dx * dx + dy * dy) <= this->self.getRadius() * this->self.getRadius();
}
void BoomBullet::update()
{
	sf::Vector2f delta = { std::uniform_real_distribution<float>(-10.f,10.f)(gen),std::uniform_real_distribution<float>(-10.f,10.f)(gen) };
	this->trail.push_front(this->self.getPosition() + delta);
	if (trail.size() > this->trailmax) trail.pop_back();
	this->trailvertics.clear();
	sf::Color color({ 255,0,0,255 });
	for (const auto& pos : trail)
	{
		trailvertics.append(sf::Vertex(pos, color));
		color.a -= static_cast<std::uint8_t>(1.f / this->trailmax * 255);
	}
	float v = this->getv();
	this->self.move({ v * cos(this->angle), v * sin(this->angle) });
	if (this->self.getPosition().x < -100 || this->self.getPosition().x > 2000 || this->self.getPosition().y < -100 || self.getPosition().y > 1200)
	{
		this->isactive = false;
	}
	this->box->setPosition(self.getPosition());
	float rest = this->wait - this->getTime();
	if (rest > 0.f && rest < 0.75f)
	{
		this->self.setFillColor(this->color);
		if (sin(rest * 8 * pi) > 0)
		{
			this->self.setFillColor(sf::Color::White);
		}
	}
	if (rest <= 0.f)
	{
		this->self.setFillColor(this->color);
		this->isactive = false;
		this->box->round(10.f, 2, -30.f);
	}
}
//PlaneBullet
PlaneBullet::PlaneBullet(const sf::Vector2f& size, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type, const float& wait, BulletBox* boxptr) :Bullet(size, position, color, velocity, type)
{
	this->self.setSize(size);
	this->self.setPosition(position);
	this->self.setFillColor(color);
	this->self.setOrigin(size / 2.f);
	this->trailmax = 60;
	this->angle = angle;
	this->wait = wait;
	this->color = color;
	this->box = boxptr;
}
PlaneBullet::~PlaneBullet()
{
	delete this->box;
}
void PlaneBullet::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->trailvertics, states);
	target.draw(this->self, states);
}
bool PlaneBullet::checkCollision(const sf::FloatRect& playerBounds)const
{
	sf::FloatRect bulletRect = this->self.getGlobalBounds();
	if (const std::optional intersection = bulletRect.findIntersection(playerBounds))
	{
		return true;
	}
	return false;
}
void PlaneBullet::update()
{
	sf::Vector2f delta = { std::uniform_real_distribution<float>(-10.f,10.f)(gen),std::uniform_real_distribution<float>(-10.f,10.f)(gen) };
	this->trail.push_front(this->self.getPosition() + delta);
	if (trail.size() > this->trailmax) trail.pop_back();
	this->trailvertics.clear();
	sf::Color color({ 255,0,0,255 });
	for (const auto& pos : trail)
	{
		trailvertics.append(sf::Vertex(pos, color));
		color.a -= static_cast<std::uint8_t>(1.f / this->trailmax * 255);
	}
	float v = this->getv();
	this->self.move({ v * cos(this->angle), v * sin(this->angle) });
	if (this->self.getPosition().x < -100 || this->self.getPosition().x > 2000 || this->self.getPosition().y < -100 || self.getPosition().y > 1200)
	{
		this->isactive = false;
	}
	this->box->setPosition(this->self.getPosition());
	if (this->getTime() > this->wait)
	{
		this->box->round(10.f, 2, 0.f);
		this->restart();
	}
}
//SerpentineBullet
SerpentineBullet::SerpentineBullet(const float& radius, const sf::Vector2f& position, const sf::Color& color, const float& velocity, const float& angle, const int& type) :Bullet({ radius,radius }, position, color, velocity, type)
{
	this->self.setRadius(radius);
	this->self.setFillColor(color);
	this->self.setPosition(position);
	this->self.setOrigin({ radius,radius });
	this->angle = angle;
}
void SerpentineBullet::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->trailvertics, states);
	target.draw(this->self, states);
}
bool SerpentineBullet::checkCollision(const sf::FloatRect& playerBounds)const
{
	float dx = this->self.getPosition().x - std::max(playerBounds.position.x, std::min(this->self.getPosition().x, playerBounds.position.x + playerBounds.size.x));
	float dy = this->self.getPosition().y - std::max(playerBounds.position.y, std::min(this->self.getPosition().y, playerBounds.position.y + playerBounds.size.y));
	return (dx * dx + dy * dy) <= this->self.getRadius() * this->self.getRadius();
}
void SerpentineBullet::update()
{
	sf::Vector2f delta = { std::uniform_real_distribution<float>(-10.f,10.f)(gen),std::uniform_real_distribution<float>(-10.f,10.f)(gen) };
	this->trail.push_front(this->self.getPosition() + delta);
	if (trail.size() > this->trailmax) trail.pop_back();
	this->trailvertics.clear();
	sf::Color color({ 255,0,0,255 });
	for (const auto& pos : trail)
	{
		trailvertics.append(sf::Vertex(pos, color));
		color.a -= static_cast<std::uint8_t>(1.f / this->trailmax * 255);
	}
	float v = this->getv();
	this->self.move({ v * cos(this->angle), v * sin(this->angle) });
	float vy = 20.f * cos(this->getTime() * pi);
	this->self.move({ vy * sin(this->angle), vy * cos(this->angle) });
	if (this->self.getPosition().x < -100 || this->self.getPosition().x > 2000 || this->self.getPosition().y < -100 || self.getPosition().y > 1200)
	{
		this->isactive = false;
	}
}

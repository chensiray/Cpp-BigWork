#include "Player.h"
Player::Player(const int& life, const float& x, const float& y)
{
	this->life = life;
	this->self.setOrigin({ 10.f,10.f });
	this->self.setPosition({ x,y });
	this->self.setFillColor(sf::Color::Cyan);
	this->glow.setFillColor(sf::Color({ 0,255,255,60 }));
	this->glow.setRadius(20.f);
	this->glow.setOrigin({ 20.f,20.f });
	this->glowClock.restart();
	this->isimmune = false;
	this->haveclear = 0;
	this->haveclock = 0;
	this->havecoin = 0;
	this->haveshield = 0;
	this->m_acceleration = 2400.f;
	this->maxspeed = 1200.f;
	this->speed = { 0.f, 0.f };
	this->friction = 3.f;
	this->isshield = false;
	this->trailmax = 55;
	for (int i = 0; i < 10; i++)
	{
		this->hearts[i].setPosition({-5.f + i * 70.f,10.f});
		this->hearts[i].setScale({0.05f,0.05f});
	}
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	for (int i = 0; i < std::min(this->life, 10); i++)
	{
		target.draw(this->hearts[i]);
	}
	target.draw(this->glow, states);
	target.draw(this->trailvertics, states);
	target.draw(this->self, states);
}
void Player::update()
{
	//trail
	sf::Vector2f delta = { std::uniform_real_distribution<float>(-8.f,8.f)(gen),std::uniform_real_distribution<float>(-8.f,8.f)(gen) };
	this->trail.push_front(this->self.getPosition() + delta);
	if (trail.size() > this->trailmax) trail.pop_back();
	this->trailvertics.clear();
	sf::Color color({ 0,255,255,255 });
	for (const auto& pos : trail)
	{
		trailvertics.append(sf::Vertex(pos, color));
		color.a -= static_cast<std::uint8_t>(1.f / this->trailmax * 255);
	}
	//glow
	glow.setFillColor(sf::Color({ 0,255,255,static_cast <std::uint8_t>(60 + 40 * sin(pi * this->glowClock.getElapsedTime().asSeconds())) }));
	//move
	sf::Time deltaTime = sf::seconds(1.f / 60.f);
	sf::Vector2f input(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		input.y -= 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		input.y += 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		input.x -= 1;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		input.x += 1;
	}
	if (input.x != 0.f && input.y != 0.f)
	{
		input = input / std::hypot(input.x, input.y);
	}
	sf::Vector2f acceleration = input * this->m_acceleration;
	this->speed += acceleration * deltaTime.asSeconds();
	if (std::hypot(this->speed.x, this->speed.y) > this->maxspeed)
	{
		this->speed = this->speed / std::hypot(this->speed.x, this->speed.y) * this->maxspeed;
	}
	this->speed -= this->speed * this->friction * deltaTime.asSeconds();
	this->self.move(this->speed * deltaTime.asSeconds());
	sf::Vector2f position = this->self.getPosition();
	position.x = position.x < 10 ? 10 : position.x;
	position.x = position.x > 1910 ? 1910 : position.x;
	position.y = position.y <= 10 ? 10 : position.y;
	position.y = position.y >= 1070 ? 1070 : position.y;
	this->self.setPosition(position);
	this->glow.setPosition(position);
	//injury
	if (this->isshield)
	{
		this->self.setFillColor(sf::Color::Green);
		if ((gameTime - shieldBirthTime) / sf::seconds(1.f) > 3.f)
		{
			this->isshield = false;
			this->self.setFillColor(sf::Color::Cyan);
		}
	}
	else
	{
		this->self.setFillColor(sf::Color::Cyan);
	}
	if (this->isimmune)
	{
		float elapse = (gameTime-immuneBirthTime) / sf::seconds(1.f);
		if (elapse > 1.5f)
		{
			this->isimmune = false;
			this->self.setFillColor(sf::Color::Cyan);
		}
		else
		{
			this->self.setFillColor(sf::Color::Cyan);
			if (sin(elapse * pi * 6) > 0)
			{
				this->self.setFillColor(sf::Color::White);
			}
		}
	}
}
sf::FloatRect Player::getBounds()const
{
	return this->self.getGlobalBounds();
}
sf::Vector2f Player::getPosition()const
{
	return this->self.getPosition();
}
void Player::hurt()
{
	if (this->isimmune || this->isshield) return;
	this->life--;
	this->isimmune = true;
	this->immuneBirthTime = gameTime;
}
void Player::shield()
{
	this->isshield = true;
	this->shieldBirthTime = gameTime;
}
void Player::addlife(const int& cnt)
{
	this->life += cnt;
}
int Player::getlife()const
{
	return this->life;
}
bool Player::alive()const
{
	return this->life > 0;
}
void Player::addspeed()
{
	this->m_acceleration += 400.f;
	this->maxspeed += 200.f;
	if (this->m_acceleration > 3400.f)
	{
		this->m_acceleration = 3400.f;
		this->maxspeed = 1700.f;
	}
}
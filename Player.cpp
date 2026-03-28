#include "Player.h"
Player::Player(const int& life, const float& x, const float& y)
{
	this->life = life;
	this->self.setOrigin({ 10.f,10.f });
	this->self.setPosition({ x,y });
	this->self.setFillColor(sf::Color::Cyan);
	this->isimmune = false;
	this->haveclear = 0;
	this->haveclock = 0;
	this->havecoin = 0;
	this->haveshield = 0;
	this->speed = 10.f;
	this->isshield = false;
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
	target.draw(this->self, states);
}
void Player::update()
{
	//move
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		this->self.move({ 0.f,-this->speed});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		this->self.move({ 0.f,this->speed });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		this->self.move({ -this->speed,0.f });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		this->self.move({ this->speed,0.f });
	}
	sf::Vector2f position = this->self.getPosition();
	position.x = position.x < 10 ? 10 : position.x;
	position.x = position.x > 1910 ? 1910 : position.x;
	position.y = position.y <= 10 ? 10 : position.y;
	position.y = position.y >= 1070 ? 1070 : position.y;
	this->self.setPosition(position);
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
		if (elapse > 1.f)
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
void Player::addspeed(const float& rate)
{
	this->speed += rate;
	this->speed = std::min(this->speed, 20.f);
}
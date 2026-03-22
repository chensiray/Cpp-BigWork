#include "Player.h"
Player::Player(const int& life, const float& x, const float& y)
{
	this->life = life;
	this->self.setOrigin({ 10.f,10.f });
	this->self.setPosition({ x,y });
	this->self.setFillColor(sf::Color::Cyan);
	this->isimmune = false;
	for (int i = 0; i < 10; i++)
	{
		sf::Sprite* heart = new sf::Sprite(heartTexture);
		heart->setPosition({ -5.f+i*70.f,10.f });
		heart->setScale({ 0.05f,0.05f });
		this->hearts.push_back(heart);
	}
}
void Player::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	for (int i = 0; i < this->life; i++)
	{
		target.draw(*(this->hearts[i]));
	}
	target.draw(this->self, states);
}
void Player::update()
{
	//move
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		this->self.move({ 0.f,-10.f});
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		this->self.move({ 0.f,10.f });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		this->self.move({ -10.f,0.f });
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		this->self.move({ 10.f,0.f });
	}
	sf::Vector2f position = this->self.getPosition();
	position.x = position.x < 10 ? 10 : position.x;
	position.x = position.x > 1910 ? 1910 : position.x;
	position.y = position.y <= 10 ? 10 : position.y;
	position.y = position.y >= 1070 ? 1070 : position.y;
	this->self.setPosition(position);
	//injury
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
	if (this->isimmune) return;
	this->life--;
	this->isimmune = true;
	this->immuneBirthTime = gameTime;
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
#include"Item.h"
Item::Item(const sf::Vector2f& position,const int& type,const float& life)
{
	this->self.setOrigin({ 960.f,540.f });
	this->self.setScale({ 0.05f,0.05f });
	this->self.setTexture(texture[type]);
	this->self.setPosition(position);
	sf::Color color = sf::Color::Yellow;
	if (type == 5)
	{
		color = sf::Color({ 235,115,142 });
	}
	this->particle = new ParticleSystem(position, color, 500, life, 50.f);
	this->type = type;
	this->life = life;
	this->birthTime = gameTime;
	this->isactive = true;
}
Item::~Item()
{
	delete this->particle;
}
void Item::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->self, states);
	this->particle->draw(target, states);
}
void Item::update()
{
	if ((gameTime - this->birthTime) / sf::seconds(1.f) > life)
	{
		this->isactive = false;
	}
	else
	{
		this->particle->update();
	}
}
void Item::checkCollision(Player& player)
{
	sf::FloatRect playerBounds = player.getBounds();
	float dx = this->self.getPosition().x - std::max(playerBounds.position.x, std::min(this->self.getPosition().x, playerBounds.position.x + playerBounds.size.x));
	float dy = this->self.getPosition().y - std::max(playerBounds.position.y, std::min(this->self.getPosition().y, playerBounds.position.y + playerBounds.size.y));
	if ((dx * dx + dy * dy) < 50.f * 50.f)
	{
		this->isactive = false;
		switch (this->type)
		{
		case 0:
			player.addlife(1);
			break;
		case 1:
			player.addspeed();
			break;
		case 2:
			player.haveshield++;
			break;
		case 3:
			player.haveclock++;
			break;
		case 4:
			player.haveclear++;
			break;
		case 5:
			player.havecoin++;
			break;
		}
	}
}
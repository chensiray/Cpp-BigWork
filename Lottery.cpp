#include"Lottery.h"
Lottery::Lottery(const sf::Vector2f& position)
{
	this->self1 = sf::RectangleShape({ 1000.f,300.f });
	this->self1.setOrigin({ 500.f,150.f });
	this->self1.setPosition(position);
	this->self1.setFillColor(sf::Color::Green);
	this->self2 = sf::RectangleShape({ 900.f,250.f });
	this->self2.setOrigin({ 450.f,125.f });
	this->self2.setPosition(position);
	this->self2.setFillColor(sf::Color::Black);
	this->wait = 4.f;
	this->isroll = false;
	this->last = 1;
	for (int i = 0; i < 3; i++)
	{
		this->display[i] = 0;
		this->box[i].setOrigin({960.f,540.f});
		this->box[i].setScale({ 0.15f,0.15f });
		this->box[i].setPosition(position + sf::Vector2f({ (i - 1) * 250.f,0.f }));
	}
	this->change();
	this->bingo = new Button(Bingo, { 300.f,120.f }, position + sf::Vector2f({ 450.f,400.f }), sf::Color::Blue);
	this->back = new Button(Back, { 300.f,120.f }, position + sf::Vector2f({ -450.f,400.f }), sf::Color::Red);
}
void Lottery::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->self1, states);
	target.draw(this->self2, states);
	this->bingo->draw(target, states);
	this->back->draw(target, states);
	target.draw(this->box[0], states);
	target.draw(this->box[1], states);
	target.draw(this->box[2], states);
}
Lottery::~Lottery()
{
	delete this->bingo;
	delete this->back;
}
bool Lottery::update(const sf::Vector2f& mousePos)
{
	this->bingo->update(mousePos);
	this->back->update(mousePos);
	if (!isroll) return false;
	float time = this->getTime();
	float now = sin(exp(time/1.1f));
	if (this->last * now <= 0)
	{
		this->change();
	}
	this->last = now;
	if (time > 5.5f)
	{
		this->isroll = false;
		return true;
	}
	return false;
}
void Lottery::change()
{
	for (int i = 0; i < 3; i++)
	{
		int id = std::uniform_int_distribution<int>(0, 6)(gen);
		if (id >= 5 || id == this->display[i]) id = display[(i + 2) % 3];
		this->display[i] = id;
		this->box[i].setTexture(texture[id]);
	}
}
int Lottery::headleEvent(const sf::Event& event, const sf::RenderWindow& window)const
{
	if (isroll) return 0;
	if (this->bingo->headleEvent(event, window))
	{
		return 1;
	}
	if (this->back->headleEvent(event, window))
	{
		return 2;
	}
	return 0;
}
void Lottery::getPrize()
{
	this->isroll = true;
	this->birthTime = gameTime;
}
float Lottery::getTime()const
{
	return (gameTime - this->birthTime) / sf::seconds(1.f);
}
int Lottery::getDisplay()const
{
	if (display[0] == display[1] && display[1] == display[2])
	{
		return display[0] * 10 + 2;
	}
	else if (display[0] == display[1] || display[1] == display[2])
	{
		return display[1] * 10 + 1;
	}
	else if (display[0] == display[2])
	{
		return display[0] * 10 + 1;
	}
	else
	{
		return 0;
	}
}
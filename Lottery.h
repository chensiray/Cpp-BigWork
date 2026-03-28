#pragma once
#include"rescource.h"
#include"Button.h"
class Lottery :public sf::Drawable
{
public:
	Lottery(const sf::Vector2f& position);
	~Lottery();
	bool update(const sf::Vector2f& mousePos);
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	int headleEvent(const sf::Event& event, const sf::RenderWindow& window)const;
	void getPrize();
	int getDisplay()const;
private:
	float getTime()const;
	void change();
	sf::Sprite box[3] = { sf::Sprite(heartTexture), sf::Sprite(heartTexture) ,sf::Sprite(heartTexture) };
	sf::RectangleShape self1;
	sf::RectangleShape self2;
	int display[3];
	Button* bingo;
	Button* back;
	float wait;
	sf::Time birthTime;
	bool isroll;
	float last;
};
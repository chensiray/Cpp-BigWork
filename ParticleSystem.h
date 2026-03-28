#pragma once
#include"rescource.h"
class ParticleSystem :public sf::Drawable
{
public:
	ParticleSystem(const sf::Vector2f& position,const sf::Color& color,const int& count,const float& duration,const float& radius);
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states)const override;
	bool isactive;
private:
	struct particle
	{
		float life;
		float birthlife;
		float angle;
		float velocity;
	};
	void reset(const size_t& index);
	float getElapsed();
	std::vector<particle> particles;
	sf::VertexArray self;
	sf::Time birthTime;
	sf::Time lastTime;
	sf::Vector2f position;
	sf::Color color;
	float duration;
	float radius;
};
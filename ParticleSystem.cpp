#include"ParticleSystem.h"
ParticleSystem::ParticleSystem(const sf::Vector2f& position, const sf::Color& color, const int& count, const float& duration, const float& radius)
{
	this->self = sf::VertexArray(sf::PrimitiveType::Points, count);
	this->particles = std::vector<particle>(count);
	this->birthTime = gameTime;
	this->lastTime = gameTime;
	this->duration = duration;
	this->position = position;
	this->isactive = true;
	this->color = color;
	this->radius = radius;
}
void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->self, states);
}
void ParticleSystem::update()
{
	if ((gameTime - this->birthTime) / sf::seconds(1.f) > this->duration)
	{
		this->isactive = false;
		return;
	}
	float elapsed = this->getElapsed();
	for (size_t i = 0; i < this->particles.size(); i++)
	{
		particle& p = particles[i];
		p.life -= elapsed;
		if (p.life <= 0.f)
		{
			reset(i);
		}
		this->self[i].position.x += p.velocity * elapsed * cos(p.angle);
		this->self[i].position.y += p.velocity * elapsed * sin(p.angle);
		float dx = this->self[i].position.x - this->position.x;
		float dy = this->self[i].position.y - this->position.y;
		float ratio = exp(-0.4f * sqrt(dx * dx + dy * dy) / radius);
		this->self[i].color.a = static_cast<std::uint8_t>(ratio * 255);
	}
}
void ParticleSystem::reset(const size_t& index)
{
	this->particles[index].angle = std::uniform_real_distribution<float>(0, 2 * pi)(gen);
	this->self[index].color = this->color;
	this->self[index].position = this->position;
	float angle = std::uniform_real_distribution<float>(0, 2 * pi)(gen);
	float radius = std::uniform_real_distribution<float>(0, this->radius)(gen);
	this->self[index].position.x += radius * cos(angle);
	this->self[index].position.y += radius * sin(angle);
	this->particles[index].velocity = std::uniform_real_distribution<float>(20.f, 50.f)(gen);
	float maxlife = this->duration - (gameTime - this->birthTime) / sf::seconds(1.f);
	if (maxlife > 0.2f)
		this->particles[index].birthlife = std::uniform_real_distribution<float>(0.f, maxlife)(gen);
	else
		this->particles[index].birthlife = maxlife;
	this->particles[index].life = this->particles[index].birthlife;
}
float ParticleSystem::getElapsed()
{
	float elapsed = (gameTime - this->lastTime) / sf::seconds(1.f);
	lastTime = gameTime;
	return elapsed;
}
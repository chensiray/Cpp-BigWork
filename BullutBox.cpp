#include "BulletBox.h"
BulletBox::BulletBox(const sf::Vector2f& position, std::vector<std::unique_ptr<Bullet>>& bullets)
{
    this->position = position;
    this->bullets = &bullets;
}
void BulletBox::setPosition(const sf::Vector2f& position)
{
    this->position = position;
}
void BulletBox::round(const float& size, const int& type, const float& r)
{
    sf::Vector2f po = this->position;
    std::uniform_real_distribution<float> dis(0.0, pi / 6);
    float phase = dis(gen);
    for (int i = 1; i <= 12; i++)
    {
        po.x -= r * cos(i * pi / 6 + phase);
        po.y -= r * sin(i * pi / 6 + phase);
        this->bullets->push_back(std::unique_ptr<RoundBullet>(new RoundBullet({ size,size }, po, sf::Color::Red, 5.f, phase + i * pi / 6, type)));
        po = this->position;
    }
}
void BulletBox::follow(const float& size, const int& type,Player *const target)
{
    this->bullets->push_back(std::unique_ptr<TriangleBullet>(new TriangleBullet(size, this->position, sf::Color::Red, 5.f, type, target)));
}
void BulletBox::sector(const float& size, const int& type,const float& angle)
{
    for (int i = 0; i <= 8; i++)
    {
        this->bullets->push_back(std::unique_ptr<RoundBullet>(new RoundBullet({ size,size }, this->position, sf::Color::Red, 5.f, angle + pi * i / 24 - pi / 6, type)));
    }
}
void BulletBox::randomfall(const sf::Vector2f& size, const int& type, const float& angle)
{
    std::vector<int> re(5);
    std::iota(re.begin(), re.end(), 1);
    std::shuffle(re.begin(), re.end(), gen);
    sf::Vector2f po = this->position;
    for (int i = 0; i <= 4; i++)
    {
        po.x += (2 - i) * sin(angle) * size.x * 1.2f;
        po.y += (2 - i) * cos(angle) * size.y * 1.2f;
        this->bullets->push_back(std::unique_ptr<RectangleBullet>(new RectangleBullet(size, po, sf::Color::Red, 30.f, angle, type, re[i] * 1.5f)));
        po = this->position;
    }
}
void BulletBox::boom(const float& size, const int& type, const float& angle, const float& wait, const int& count)
{
    for (int i = 1; i <= count; i++)
    {
        this->bullets->push_back(std::unique_ptr<BoomBullet>(new BoomBullet(size, this->position, sf::Color::Red, 5.f, angle + cos(i*pi) * (i / 2) * pi / 6, type, wait, new BulletBox(this->position, *bullets))));
    }
}
void BulletBox::plane(const sf::Vector2f& size, const int& type, const float& angle, const float& wait)
{
    this->bullets->push_back(std::unique_ptr<PlaneBullet>(new PlaneBullet(size, this->position, sf::Color::Red, 5.f, angle, type, wait, new BulletBox(this->position, *bullets))));
}
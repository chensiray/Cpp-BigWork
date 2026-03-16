#include"rescource.h"
sf::Font font("HarmonyOS_Sans_SC_Regular.ttf");
sf::Texture heartTexture("RedHeart.png");
const float pi = 3.1415926f;
sf::Color button_default(0, 0, 255);
sf::Color button_hovered(0, 50, 200);
sf::Time gameTime = sf::seconds(0.f);
std::random_device rd;
std::mt19937 gen(rd());
void updateViewport(const sf::RenderWindow& window, sf::View& gameview)
{
    sf::Vector2u windowSize = window.getSize();
    if (windowSize.x == 0 || windowSize.y == 0) return;
    float windowRatio = static_cast<float>(windowSize.x) / windowSize.y;
    float viewRatio = 1920.f / 1080.f;
    sf::FloatRect viewport;
    if (windowRatio > viewRatio)
    {
        viewport.size.x = viewRatio / windowRatio;
        viewport.size.y = 1.0f;
    }
    else
    {
        viewport.size.x = 1.0f;
        viewport.size.y = windowRatio / viewRatio;
    }
    viewport.position.x = (1.0f - viewport.size.x) / 2.0f;
    viewport.position.y = (1.0f - viewport.size.y) / 2.0f;
    gameview.setViewport(viewport);
    return;
}
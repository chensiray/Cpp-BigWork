#include"rescource.h"
sf::Font font("HarmonyOS_Sans_SC_Regular.ttf");
sf::Texture heartTexture("RedHeart.png");
sf::Texture musicTexture("music.png");
sf::Texture pauseTexture("pause.png");
sf::Texture awardTexture("award.png");
sf::Texture clockTexture("clock.png");
sf::Texture shieldTexture("shield.png");
sf::Texture speedTexture("speed.png");
sf::Texture clearTexture("clear.png");
sf::Texture coinTexture("coin.png");
sf::Music musicmain("Mesmerizing Galaxy Loop.mp3");
sf::Music musiclevel1("Obliteration.mp3");
sf::Music musiclevel2("Darkling.mp3");
sf::Music musiclevel3("Galactic Rap.mp3");
sf::Music musiclevel4("Screen Saver.mp3");
sf::Texture texture[6] = { heartTexture, speedTexture, shieldTexture, clockTexture, clearTexture ,coinTexture};
const float pi = 3.1415926f;
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
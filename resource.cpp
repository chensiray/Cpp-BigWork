#include"resource.h"
sf::Font font;
sf::Texture heartTexture;
sf::Texture musicTexture;
sf::Texture pauseTexture;
sf::Texture awardTexture;
sf::Texture clockTexture;
sf::Texture shieldTexture;
sf::Texture speedTexture;
sf::Texture clearTexture;
sf::Texture coinTexture;
sf::Texture titleTexture;
sf::Music musicmain;
sf::Music musiclevel1;
sf::Music musiclevel2;
sf::Music musiclevel3;
sf::Music musiclevel4;
sf::SoundBuffer lotterybuffer;
sf::Sound lotterysound(lotterybuffer);
sf::SoundBuffer lotteryloopbuffer;
sf::Sound lotteryloop(lotteryloopbuffer);
sf::SoundBuffer dingbuffer;
sf::Sound ding(dingbuffer);
sf::SoundBuffer clearbuffer;
sf::Sound clearsound(clearbuffer);
sf::SoundBuffer pressbuffer;
sf::Sound press(pressbuffer);
sf::Texture texture[6];
sf::Time gameTime = sf::seconds(0.f);
std::random_device rd;
std::mt19937 gen(rd());
const float pi = 3.1415926f;
bool loadResources()
{
	bool success = true;
	if (!font.openFromFile("resource/HarmonyOS_Sans_SC_Regular.ttf"))
	{
		std::cerr << "错误：无法加载字体文件 resource/HarmonyOS_Sans_SC_Regular.ttf" << std::endl;
		success = false;
	}
	if (!heartTexture.loadFromFile("resource/RedHeart.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/RedHeart.png" << std::endl;
		success = false;
	}
	if (!musicTexture.loadFromFile("resource/music.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/music.png" << std::endl;
		success = false;
	}
	if (!pauseTexture.loadFromFile("resource/pause.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/pause.png" << std::endl;
		success = false;
	}
	if (!awardTexture.loadFromFile("resource/award.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/award.png" << std::endl;
		success = false;
	}
	if (!clockTexture.loadFromFile("resource/clock.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/clock.png" << std::endl;
		success = false;
	}
	if (!shieldTexture.loadFromFile("resource/shield.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/shield.png" << std::endl;
		success = false;
	}
	if (!speedTexture.loadFromFile("resource/speed.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/speed.png" << std::endl;
		success = false;
	}
	if (!clearTexture.loadFromFile("resource/clear.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/clear.png" << std::endl;
		success = false;
	}
	if (!coinTexture.loadFromFile("resource/coin.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/coin.png" << std::endl;
		success = false;
	}
	if (!titleTexture.loadFromFile("resource/title.png"))
	{
		std::cerr << "错误：无法加载图像文件 resource/title.png" << std::endl;
		success = false;
	}
	if (!musicmain.openFromFile("resource/Mesmerizing Galaxy Loop.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/Mesmerizing Galaxy Loop.mp3" << std::endl;
		success = false;
	}
	if (!musiclevel1.openFromFile("resource/Obliteration.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/Obliteration.mp3" << std::endl;
		success = false;
	}
	if (!musiclevel2.openFromFile("resource/Darkling.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/Darkling.mp3" << std::endl;
		success = false;
	}
	if (!musiclevel3.openFromFile("resource/Galactic Rap.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/Galactic Rap.mp3" << std::endl;
		success = false;
	}
	if (!musiclevel4.openFromFile("resource/Screen Saver.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/Screen Saver.mp3" << std::endl;
		success = false;
	}
	if (!lotterybuffer.loadFromFile("resource/lottery.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/lottery.mp3" << std::endl;
		success = false;
	}
	if (!lotteryloopbuffer.loadFromFile("resource/lotteryloop.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/lotteryloop.mp3" << std::endl;
		success = false;
	}
	if (!dingbuffer.loadFromFile("resource/ding.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/ding.mp3" << std::endl;
		success = false;
	}
	if (!clearbuffer.loadFromFile("resource/clear.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/clear.mp3" << std::endl;
		success = false;
	}
	if (!pressbuffer.loadFromFile("resource/press.mp3"))
	{
		std::cerr << "错误：无法加载音频文件 resource/press.mp3" << std::endl;
		success = false;
	}
	sf::Sound lotterysound(lotterybuffer);
	sf::Sound lotteryloop(lotteryloopbuffer);
	sf::Sound ding(dingbuffer);
	sf::Sound clearsound(clearbuffer);
	sf::Sound press(pressbuffer);
	texture[0] = heartTexture;
	texture[1] = speedTexture;
	texture[2] = shieldTexture;
	texture[3] = clockTexture;
	texture[4] = clearTexture;
	texture[5] = coinTexture;
	return success;
}
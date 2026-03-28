#include "Button.h"
Button::Button(const sf::String& content,const sf::Vector2f &size, const sf::Vector2f& position,const sf::Color& color)
{
	this->background.setSize(size);
	this->background.setOrigin(size / 2.f);
	this->background.setPosition(position);
	this->background.setFillColor(color);
	this->color = color;
	this->text = sf::Text(font, content, (int)size.y/3);
	this->text.setFillColor(sf::Color::White);
	sf::FloatRect textBounds = this->text.getLocalBounds();
	float textx = textBounds.position.x + textBounds.size.x / 2.f;
	float texty = textBounds.position.y + textBounds.size.y / 2.f;
	this->text.setOrigin({ textx,texty });
	this->text.setPosition(this->background.getPosition());
	this->isAnimating = false;
	this->animationDuration = sf::seconds(0.2f);
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->background, states);
	target.draw(this->text, states);
};
void Button::setText(const sf::String& text)
{
	this->text.setString(text);
}
bool Button::headleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	if (const auto* mousepressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousepressed->button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(mousepressed->position);
			if (this->background.getGlobalBounds().contains(mousePos))
			{
				this->isAnimating = true;
				this->birthTime = gameTime;
				return true;
			}
		}
	}
	return false;
}
void Button::update(const sf::Vector2f& mousePos)
{
	if (this->isAnimating) //点击动画
	{
		sf::Time elapsed = gameTime - birthTime;
		float progress = elapsed / animationDuration;
		if (progress > 1.0f)
		{
			background.setScale({ 1.f,1.f });
			text.setScale({ 1.f,1.f });
			this->isAnimating = false;
		}
		else
		{
			float scale = 1.0f - 0.2f * sin(progress * pi);
			this->background.setScale({ scale,scale });
			this->text.setScale({ scale,scale });
		}
	}
	else //悬浮动画
	{
		if (this->background.getGlobalBounds().contains(mousePos))
		{
			this->color.a = static_cast<std::uint8_t>(180);
		}
		else
		{
			this->color.a = static_cast<std::uint8_t>(255);
		}
		this->background.setFillColor(this->color);
	}
}
MusicButton::MusicButton(const sf::Vector2f& size, const sf::Vector2f& position) :Button(sf::String(""), size, position, sf::Color::White)
{
	this->self = sf::Sprite(musicTexture);
	this->self.setOrigin({ 960.f,540.f });
	this->self.setScale({ 0.1f,0.1f });
	this->self.setPosition(position);
	this->ismusic = true;
}
void MusicButton::draw(sf::RenderTarget& target, sf::RenderStates states)const
{
	target.draw(this->self, states);
}
bool MusicButton::headleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	if (const auto* mousepressed = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousepressed->button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(mousepressed->position);
			if (this->self.getGlobalBounds().contains(mousePos))
			{
				this->ismusic = !this->ismusic;
				return true;
			}
		}
	}
	return false;
}
void MusicButton::update()
{
	if (this->ismusic)
	{
		this->self.setTexture(musicTexture);
	}
	else
	{
		this->self.setTexture(pauseTexture);
	}
	this->self.rotate(sf::degrees(1));
}
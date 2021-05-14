#include "Background.h"

Background::Background()
{

}

void Background::draw(sf::RenderWindow& window)
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Color(40,40,40,255));
	rect.setSize(sf::Vector2f(window.getSize()));
	window.draw(rect);
}

Background::~Background()
{

}
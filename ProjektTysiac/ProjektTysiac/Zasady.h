#pragma once
#include <SFML/Graphics.hpp>

class Zasady
{
public:
	Zasady();
	~Zasady();

	void draw(sf::RenderWindow& window);
	void ScrollWGore();
	void ScrollWDol();
	void ScrollReset();

private:
	sf::Texture texture;
	sf::Sprite tlo;
	sf::Vector2u wielkoscTla;
	sf::View view;

	sf::Font font;
};
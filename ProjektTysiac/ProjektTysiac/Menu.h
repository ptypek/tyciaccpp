#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
public:
	Menu(float x, float y);
	~Menu();

	void draw(sf::RenderWindow& window);
	void wDol();
	void wGore();
	int getOpcja();
private:
	sf::Font font;
	sf::Text menu[3];
	sf::RectangleShape poleMenu;
	int opcja = 0;
	
	sf::Vector2f srodek;
};


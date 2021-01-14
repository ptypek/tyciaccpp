#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "naglowek.h"


class Licytacja{
public:
	Licytacja();
	~Licytacja();

	void draw(sf::RenderWindow& window);
	void wDol();
	void wGore();

	int getOpcja();
	int getLicytacjaUzyte();
	int getLicytujeZa();

	void licytuj();
	int licytujBot(Karta[]);
private:
	sf::Font font;
	sf::Text licytacja[5];
	sf::RectangleShape poleLicytacja;

	int opcja = 0;
	int licytacjaUzyte[5];
	int licytujeZa;
};


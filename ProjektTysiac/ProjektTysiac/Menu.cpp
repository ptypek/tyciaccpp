#include "Menu.h"
#include <iostream>

Menu::Menu(float x, float y) {
	if (!font.loadFromFile("sansation.ttf")) {
		std::cout << "Nie udalo sie pobrac czcionki";
	}

	menu[0].setFont(font);
	menu[0].setString("Graj");
	menu[0].setCharacterSize(50.0f);
	menu[0].setOrigin(sf::Vector2f(menu[0].getGlobalBounds().width / 2, menu[0].getGlobalBounds().height / 2));
	menu[0].setPosition(sf::Vector2f(x / 2, y / (3 + 1) * 1));

	menu[1].setFont(font);
	menu[1].setString("Zasady");
	menu[1].setCharacterSize(50.0f);
	menu[1].setOrigin(sf::Vector2f(menu[1].getGlobalBounds().width / 2, menu[1].getGlobalBounds().height / 2));
	menu[1].setPosition(sf::Vector2f(x / 2, y / (3 + 1) * 2));


	menu[2].setFont(font);
	menu[2].setString("Wyjdz");
	menu[2].setCharacterSize(50.0f);
	menu[2].setOrigin(sf::Vector2f(menu[2].getGlobalBounds().width / 2, menu[2].getGlobalBounds().height / 2));
	menu[2].setPosition(sf::Vector2f(x / 2, y / (3 + 1) * 3));

	poleMenu.setSize(sf::Vector2f(200.0f, 100.0f));
	poleMenu.setFillColor(sf::Color::Red);
	poleMenu.setOrigin(sf::Vector2f(poleMenu.getGlobalBounds().width / 2, poleMenu.getGlobalBounds().height / 2));
	poleMenu.setPosition(sf::Vector2f(x / 2, y / (3 + 1) * 1));
}

Menu::~Menu() {
}

void Menu::draw(sf::RenderWindow& window) {
	window.draw(poleMenu);
	for (int i = 0; i < 3; i++) {
		window.draw(menu[i]);
	}
}

void Menu::wDol() {
	if (opcja < 2) {
		opcja++;
		poleMenu.setPosition(sf::Vector2f(menu[opcja].getPosition().x, menu[opcja].getPosition().y ));
		std::cout << " w dol dziala : " <<opcja;
	}
	else {
		opcja = 0;
		poleMenu.setPosition(sf::Vector2f(menu[opcja].getPosition().x, menu[opcja].getPosition().y));
		std::cout << " w dol NIEdziala : " << opcja;
	}
}
void Menu::wGore() {
	if (opcja > 0) {	
		opcja--;
		poleMenu.setPosition(sf::Vector2f(menu[opcja].getPosition().x, menu[opcja].getPosition().y));
		std::cout << " w gore dziala : " << opcja;
	}
	else {
		opcja = 2;
		poleMenu.setPosition(sf::Vector2f(menu[opcja].getPosition().x, menu[opcja].getPosition().y));
		std::cout << " w gore NIEdziala : " << opcja;
	}
}
int Menu::getOpcja() {
	return opcja;
}

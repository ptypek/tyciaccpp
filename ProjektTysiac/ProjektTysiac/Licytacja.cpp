#include "Licytacja.h"


Licytacja::Licytacja():
	licytujeZa(0)
{
	if (!font.loadFromFile("sansation.ttf")) {
		std::cout << "Nie udalo sie pobrac czcionki";
	}
	for (int i = 0; i < 5; i++) {
		licytacjaUzyte[i] = -1;
	}
	for (int i = 0; i < 5; i++) {
		licytacja[i].setFont(font);
		licytacja[i].setFillColor(sf::Color::Yellow);
		licytacja[i].setCharacterSize(32);
		licytacja[i].setPosition(sf::Vector2f(150.0f, 250.0f + (50.0f * i)));
	}
	licytacja[0].setString("PASS");
	licytacja[1].setString("110");
	licytacja[2].setString("120");
	licytacja[3].setString("130");
	licytacja[4].setString("140");

	poleLicytacja.setSize(sf::Vector2f(150.0f, 50.0f));
	poleLicytacja.setFillColor(sf::Color::Blue);
	poleLicytacja.setOrigin(sf::Vector2f(poleLicytacja.getGlobalBounds().width / 2, poleLicytacja.getGlobalBounds().height / 2));
	poleLicytacja.setPosition(sf::Vector2f(licytacja[0].getPosition().x + 50.0f, licytacja[0].getPosition().y + 10.0f));

}

Licytacja::~Licytacja() {

}

void Licytacja::draw(sf::RenderWindow& window) {
	window.draw(poleLicytacja);
	for (int i = 0; i < 5; i++) {
		window.draw(licytacja[i]);
	}
}

void Licytacja::wDol() {
	if (opcja < 4) {
		opcja++;
		poleLicytacja.setPosition(sf::Vector2f(licytacja[opcja].getPosition().x + 50.0f, licytacja[opcja].getPosition().y + 10.0f));
	}
	else {
		opcja = 0;
		poleLicytacja.setPosition(sf::Vector2f(licytacja[opcja].getPosition().x + 50.0f, licytacja[opcja].getPosition().y + 10.0f));
	}
};

void Licytacja::wGore() {
	if (opcja > 0) {
		opcja--;
		poleLicytacja.setPosition(sf::Vector2f(licytacja[opcja].getPosition().x + 50.0f, licytacja[opcja].getPosition().y + 10.0f));
	}
	else {
		opcja = 4;
		poleLicytacja.setPosition(sf::Vector2f(licytacja[opcja].getPosition().x + 50.0f, licytacja[opcja].getPosition().y + 10.0f));
	}
};

void Licytacja::licytuj() {
	std::cout << opcja << std::endl;
	licytacjaUzyte[opcja] = opcja;
	switch (opcja){
	case 0:
		for (int i = 1; i < 5; i++) {
			licytacjaUzyte[i] = i;
			licytacja[i].setFillColor(sf::Color::Red);
		}
		licytujeZa = -1;
		break;
	case 1:
		for (int i = 0; i < 2; i++) {
			licytacjaUzyte[i] = i;
			licytacja[i].setFillColor(sf::Color::Red);
		}
		licytujeZa = 110;
		break;
	case 2:
		for (int i = 0; i < 3; i++) {
			licytacjaUzyte[i] = i;
			licytacja[i].setFillColor(sf::Color::Red);
		}
		licytujeZa = 120;
		break;
	case 3:
		for (int i = 0; i < 4; i++) {
			licytacjaUzyte[i] = i;
			licytacja[i].setFillColor(sf::Color::Red);
		}
		licytujeZa = 130;
		break;
	case 4:
		for (int i = 0; i < 5; i++) {
			licytacjaUzyte[i] = i;
			licytacja[i].setFillColor(sf::Color::Red);
		}
		licytujeZa = 140;
		break;
	default:
		break;
	}
}

int Licytacja::licytujBot(Karta kartyBota[]) {
	int as{};
	for (int i = 0; i < 8; i++) {
		if (kartyBota[i].figura == "A")
			as++;
	}
	if (as <= 1 && licytujeZa < 110) {
		std::cout << "110" << std::endl;
		return 110;
	}
	else if(as >= 2 && licytujeZa < 120) {
		std::cout << "120" << std::endl;
		return 120;
	}
	else {
		std::cout << "PASS" << std::endl;
		return -1;
	}
}

int Licytacja::getOpcja() {
	return opcja;
}
int Licytacja::getLicytacjaUzyte() {
	return licytacjaUzyte[opcja];
}
int Licytacja::getLicytujeZa() {
	return licytujeZa;
}
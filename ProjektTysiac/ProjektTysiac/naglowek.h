#ifndef NAGLOWEK_H
#define NAGLOWEK_H

//DEFINICJE KART
class Karta{
public:
	std::string kolor;
	std::string figura;
	int wartosc{};
	sf::Sprite card;
	bool uzyta;
	bool meldunek;
};


//*******  karty.cpp  ********//

void wczytajKarty(Karta[]);    // Wczytywanie kart do talii
void przetasujKarty(Karta[]);  // Tasowanie kart w talii
void rozdanie(Karta[], Karta[], Karta[], Karta[], Karta[]);
void przypiszObraz(Karta[],sf::Texture*);

#endif
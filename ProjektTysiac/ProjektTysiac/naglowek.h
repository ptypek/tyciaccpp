#ifndef NAGLOWEK_H
#define NAGLOWEK_H

//DEFINICJE KART
class Karta{
public:
	std::string kolor;
	std::string figura;
	int wartosc{};
	sf::Sprite card;
};


//*******  karty.cpp  ********//

void wczytajKarty(Karta[]);    // Wczytywanie kart do talii
void przetasujKarty(Karta[]);  // Tasowanie kart w talii
void rozdanie(Karta[], Karta[], Karta[], Karta[], Karta[]);
void przypiszObraz(Karta[],sf::Texture*);

//*******  animacje.cpp  ********//
void move(sf::Event,sf::Vector2i, Karta[],float*, float*, int*);
#endif
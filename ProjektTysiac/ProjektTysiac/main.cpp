#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "naglowek.h"
#include "Menu.h"
#include "Zasady.h"
using namespace std;

class Gra{
private:
	Karta talia[24], gracz1[8], gracz2[8], gracz3[8], musik[3];
	sf::RenderWindow window;
	sf::RectangleShape wybierzKarte;
	sf::Texture texture;
	sf::Texture texturePlansza;
	sf::View view;
	
	sf::Sprite plansza;
	
	sf::Font czcionka;
	sf::Text punkty1, punkty2, punkty3, wygrana, przegrana;
	int pkt1, pkt2, pkt3;

	sf::Vector2i pozycjaMyszy;
	bool przemieszczanieKarty;
	bool przetasowanieTalii;
	float dx, dy, x, y;

	Menu menu;
	int przejscieDo;
	int ktoraKarta;
	int kartyUzyte[8];
	bool czyReset;
	int doResetuZostalo;

	Zasady zasady;

	void przebieg();
	void okno();
	void pozycjaKart(Karta[], Karta[], Karta[]);
	
	void wLewo();
	void wPrawo();
	void wystawKarte(Karta&);

	int tmp;
	void pobieranieWartosci(Karta&);
	void reset();

public:
	Gra();
	void rozpocznij();
};

Gra::Gra()
	:window(sf::VideoMode(1200, 800), "Gra w tysiaca", sf::Style::Close | sf::Style::Resize),
	menu(window.getSize().x, window.getSize().y),
	zasady(),
	pkt1(0),
	pkt2(0),
	pkt3(0),
	dx(0),
	dy(0),
	x(50.0f),
	y(50.0f),
	przemieszczanieKarty(false),
	przetasowanieTalii(false),
	czyReset(false),
	przejscieDo(0),
	ktoraKarta(0),
	doResetuZostalo(0),
	tmp(0)
{
	texture.loadFromFile("talia.png");
	texturePlansza.loadFromFile("plansza.png");
	
	plansza.setTexture(texturePlansza);

	view.setSize(sf::Vector2f(1200.0f, 800.0f));
	view.setCenter(sf::Vector2f(600.0f, 400.0f));

	czcionka.loadFromFile("sansation.ttf");
	punkty1.setFont(czcionka);
	punkty2.setFont(czcionka);
	punkty3.setFont(czcionka);
	punkty1.setString(to_string(pkt1));
	punkty2.setString(to_string(pkt2));
	punkty3.setString(to_string(pkt3));
	punkty1.setPosition(sf::Vector2f(200.0f, 200.0f));
	punkty2.setPosition(sf::Vector2f(900.0f, 400.0f));
	punkty3.setPosition(sf::Vector2f(200.0f, 600.0f));
	wygrana.setString("Wygrana!");
	przegrana.setString("Przegrana!");
	
	wczytajKarty(talia);
	przypiszObraz(talia, &texture);

	pozycjaMyszy = sf::Mouse::getPosition();

	wybierzKarte.setSize(sf::Vector2f(60.0f, 60.0f));
	wybierzKarte.setFillColor(sf::Color::Red);
	wybierzKarte.setOrigin(sf::Vector2f(wybierzKarte.getGlobalBounds().width / 2, wybierzKarte.getGlobalBounds().height / 2));
	wybierzKarte.setPosition(sf::Vector2f(120.0f, 80.0f));

	for (int i = 0; i < 8; i++)
		kartyUzyte[i] = -1;
}

void Gra::pozycjaKart(Karta gracz1[8], Karta gracz2[8], Karta gracz3[8]) {
	for (int i = 0; i < 8; i++) {
		gracz1[i].card.setPosition(sf::Vector2f(120.0f + (110.0f * i), 80.0f ));
		gracz3[i].card.setPosition(sf::Vector2f(120.0f + (110.0f * i), 700.0f ));
		gracz2[i].card.setPosition(sf::Vector2f(1100.0f, 100.0f + (100.0f * i)));
	}
}

//**************Wybór kart******************//

void Gra::wLewo() {
	if (ktoraKarta >= 0 ) {
		do {
			if (ktoraKarta == 0)
				ktoraKarta = 6;
			else
			ktoraKarta--;
		} while (ktoraKarta == kartyUzyte[ktoraKarta]);
		wybierzKarte.setPosition(sf::Vector2f(gracz1[ktoraKarta].card.getPosition().x, gracz1[ktoraKarta].card.getPosition().y));
	}
}
void Gra::wPrawo() {
	if (ktoraKarta <= 6) {
		do {
			if (ktoraKarta == 6)
				ktoraKarta = 0;
			else
				ktoraKarta++;
		} while (ktoraKarta == kartyUzyte[ktoraKarta]);
		wybierzKarte.setPosition(sf::Vector2f(gracz1[ktoraKarta].card.getPosition().x, gracz1[ktoraKarta].card.getPosition().y));
	}
}
void Gra::wystawKarte(Karta& karta) {
	karta.card.setPosition(sf::Vector2f(550.0f, 400.0f));
	wybierzKarte.setPosition(sf::Vector2f(karta.card.getPosition().x, karta.card.getPosition().y));
}

//*************Mechanika gry****************//

void Gra::pobieranieWartosci(Karta& kartaGracza) {
	gracz2[tmp].card.setPosition(sf::Vector2f(630.0f, 400.0f));
	gracz3[tmp].card.setPosition(sf::Vector2f(710.0f, 400.0f));

	if (kartaGracza.kolor == gracz2[tmp].kolor && kartaGracza.kolor == gracz3[tmp].kolor) {
		if (kartaGracza.wartosc > gracz2[tmp].wartosc && kartaGracza.wartosc > gracz3[tmp].wartosc) {
			cout << "  Wygrywa gracz 1 (te same kolory)  "<<endl;
			pkt1 ++ ;
		}
		else if (gracz2[tmp].wartosc > kartaGracza.wartosc && gracz2[tmp].wartosc > gracz3[tmp].wartosc) {
			cout << "  Wygrywa gracz 2 (te same kolory)  " << endl;
			pkt2++;
		}
		else if (gracz3[tmp].wartosc > kartaGracza.wartosc && gracz3[tmp].wartosc > gracz2[tmp].wartosc) {
			cout << "  Wygrywa gracz 3 (te same kolory)  " << endl;
			pkt3++;
		}
	}
	else if (kartaGracza.kolor == gracz2[tmp].kolor) {
		if (kartaGracza.wartosc > gracz2[tmp].wartosc) {
			cout << "  Wygrywa gracz 1 (gracz 2 i 1 te same kolory)  " << endl;
			pkt1++;
		}
		else {
			cout << "  Wygrywa gracz 2 (gracz 2 i 1 te same kolory)  " << endl;
			pkt2++;
		}
	}
	else if (kartaGracza.kolor == gracz3[tmp].kolor) {
		if (kartaGracza.wartosc > gracz3[tmp].wartosc) {
			cout << "  Wygrywa gracz 1 (gracz 2 i 1 te same kolory)  " << endl;
			pkt1++;
		}
		else {
			cout << "  Wygrywa gracz 3 (gracz 2 i 1 te same kolory)  " << endl;
			pkt3++;
		}
	}
	else {
		cout << "  Wygrywa gracz 1 (brak kolorow)     " << endl;
		pkt1++;
	}
	reset();

	punkty1.setString(to_string(pkt1));
	punkty2.setString(to_string(pkt2));
	punkty3.setString(to_string(pkt3));
	tmp++;
}

void Gra::reset() {
	doResetuZostalo++;
	if (doResetuZostalo == 5 || czyReset == true) {
		przetasujKarty(talia);
		rozdanie(gracz1, gracz2, gracz3, talia, musik);
		pozycjaKart(gracz1, gracz2, gracz3);
		doResetuZostalo = 0;
		if (czyReset == true) {
			pkt1 = 0;
			pkt2 = 0;
			pkt3 = 0;
			punkty1.setString(to_string(pkt1));
			punkty2.setString(to_string(pkt2));
			punkty3.setString(to_string(pkt3));
		}
		for (int i = 0; i < 8; i++)
			kartyUzyte[i] = -1;
		czyReset = false;
		tmp = 0;
	}
}


void Gra::przebieg() {
	sf::Event zdarzenie;
	while (window.pollEvent(zdarzenie)) {
		switch (zdarzenie.type) {
		case sf::Event::Closed:
			cout << "Zamykam";
			window.close();
			break;
		case sf::Event::KeyReleased:
			if (przejscieDo == 0) {
				switch (zdarzenie.key.code) {
				case sf::Keyboard::Down:
					menu.wDol();
					break;
				case sf::Keyboard::Up:
					menu.wGore();
					break;
				case sf::Keyboard::Return:
					switch (menu.getOpcja()) {
					case 0:
						przejscieDo = 1;
						przetasujKarty(talia);
						rozdanie(gracz1, gracz2, gracz3, talia, musik);
						pozycjaKart(gracz1, gracz2, gracz3);
						break;
					case 1:
						przejscieDo = 2;
						break;
					case 2:
						window.close();
						break;
					}
				}
			}
			if (przejscieDo == 1) {
				switch (zdarzenie.key.code) {
				case sf::Keyboard::Escape:
					przejscieDo = 0;
					break;
				case sf::Keyboard::Left:
					wLewo();
					break;
				case sf::Keyboard::Right:
					wPrawo();
					break;
				case sf::Keyboard::A:
					wystawKarte(gracz1[ktoraKarta]);
					pobieranieWartosci(gracz1[ktoraKarta]);
					kartyUzyte[ktoraKarta] = ktoraKarta;
					wPrawo();
					break;
				case sf::Keyboard::R:
					czyReset = true;
					reset();
					break;
				}
				
			}
			if (przejscieDo == 2) {
				switch (zdarzenie.key.code){
				case sf::Keyboard::Escape:
					przejscieDo = 0;
					zasady.ScrollReset();
					break;
				default:
					break;
				}
			}
			break;
		case sf::Event::MouseWheelScrolled:
			if (przejscieDo == 2) {
				if (zdarzenie.mouseWheelScroll.delta > 0) {
					zasady.ScrollWGore();
				}
				else if (zdarzenie.mouseWheelScroll.delta < 0) {
					zasady.ScrollWDol();
				}
			}
		default:
			break;
		}
	}
}

void Gra::okno() {
	window.clear();
	if (przejscieDo==0) {
		window.setView(view);
		menu.draw(window);
	}
	else if (przejscieDo == 1) {
		window.setView(view);
		window.draw(plansza);
		window.draw(wybierzKarte);
		for (int i = 0; i < 8; i++) {
			window.draw(gracz1[i].card);
			window.draw(gracz2[i].card);
			window.draw(gracz3[i].card);
		}
		window.draw(punkty1);
		window.draw(punkty2);
		window.draw(punkty3);
	}
	else if (przejscieDo == 2) {
		zasady.draw(window);
	}
	window.display();
}

void Gra::rozpocznij() {
	while (window.isOpen()) {
		przebieg();
		okno();
	}
}


int main() {
	Gra gra;
	gra.rozpocznij();

	
	//sf::RenderWindow window(sf::VideoMode(1200, 800), "Gra w tysiaca", sf::Style::Close | sf::Style::Resize);
	//Karta talia[24];
	//Karta gracz1[7], gracz2[7], gracz3[7], musik[3];
	//sf::RectangleShape porownanie(sf::Vector2f(250.0f, 250.0f));
	//porownanie.setFillColor(sf::Color::Red);

	//sf::Texture texture;
	//texture.loadFromFile("talia.png");
	//sf::Texture texturaPlansza;
	//texturaPlansza.loadFromFile("plansza.png");
	//sf::Sprite plansza;
	//plansza.setTexture(texturaPlansza);
	//
	//sf::Font czcionka;
	//czcionka.loadFromFile("sansation.ttf");
	//int c = 152;

	//sf::Text punkty1, punkty2, punkty3;
	//punkty1.setFont(czcionka);
	//punkty1.setString(to_string(c));
	//
	//float a = 50.0f, b = 50.0f;
	//int n = 0;
	//przypiszObraz(talia, &texture);
	//
	//bool isMove = false;
	//float dx = 0, dy = 0;
	//float* wskdx = &dx;
	//float* wskdy = &dy;
	//int* wskn = &n;
	//wczytajKarty(talia);
	//przetasujKarty(talia);
	//rozdanie(gracz1, gracz2, gracz3, talia, musik);

	//for (int i = 0; i < 7; i++) {
	//	gracz1[i].card.setPosition(sf::Vector2f(a, b));
	//	gracz3[i].card.setPosition(sf::Vector2f(a, b + 620.0f));
	//	a += 140.0f;
	//}
	//b = 75.0f;
	//for (int i = 0; i < 7; i++) {
	//	gracz2[i].card.setPosition(sf::Vector2f(1100.0f, b));
	//	b += 100.0f;
	//}

	//while (window.isOpen()) {
	//	sf::Event e;
	//	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	//	while (window.pollEvent(e)) {
	//		if (e.type == sf::Event::Closed)
	//			window.close();
	//		if (e.type == sf::Event::KeyPressed)
	//			if (e.key.code == sf::Keyboard::A)
	//				cout << "dziala";
	//	}
	//	

	//	//******movement***************//
	//	if (e.type == sf::Event::MouseButtonPressed)
	//		if (e.key.code == sf::Mouse::Left)
	//			for(int i=0;i<24;i++)
	//				if (gracz1[i].card.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
	//					isMove = true;
	//					n = i;
	//					dx = mousePosition.x - gracz1[i].card.getPosition().x;
	//					dy = mousePosition.y - gracz1[i].card.getPosition().y;
	//				}

	//	if (e.type == sf::Event::MouseButtonReleased)
	//		if (e.key.code == sf::Mouse::Left) {
	//			isMove = false;
	//		}
	//	if (isMove)
	//		gracz1[n].card.setPosition(mousePosition.x - dx, mousePosition.y - dy);

	//	window.clear();
	//	window.draw(plansza);
	//	window.draw(punkty1);
	//	for (int i = 0; i < 7; i++) {
	//		window.draw(gracz1[i].card);
	//		window.draw(gracz2[i].card);
	//		window.draw(gracz3[i].card);
	//	}
	//	window.display();
	//}
	return 0;
}

//****************TO-DO LIST*********************
/*   __________________________________________________________________________________________________
	|- przycisk R jako reset/ od poczatku - (resetuje punkty, ustawienie kart, przetasowuje karty)     |
	|	~reset musi nastepowac po kazdej wygranej rundzie, bez resetu punktow (jakis bool)			   |  <- Zrobione
	|__________________________________________________________________________________________________|
	-normalne liczenie punktow, po osiagnieciu w ktorejs turze 1000 pkt, sprawdzane jest, kto ma najwiecej, ten wygrywa (³adny napis)
	
	-zamiast zasady napisac instrukja |-> sterowanie (ruszajace sie strzalki, a[accept] do zatwierdzania, r[reset] do resetu od podstaw)
									  |-> zasady gry (rozdanie, licytacja, meldunki (moze jakies ilustracje), musik, przebieg, zakonczenie.
		
	-meldunek, tworzenie tablicy kroli i dam, jezeli sa w kartach, jezeli maja ten sam kolor to meldunek ++.
		|-> jezeli jest wiecej meldunkow to mozemy wybrac ktory meldunek chcemy
		|-> zameldowac mozna po kliknieciu M , jezeli ilosc meldunkow jest > 0 to funkcja sie wykona

		
*/

#include <iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "naglowek.h"
#include "Menu.h"
#include "Zasady.h"
#include "Licytacja.h"
using namespace std;

class Gra{
private:
	//Ogólne zmienna kart, talii, okna
	Karta talia[24], gracz1[9], gracz2[9], gracz3[9], musik[3];
	sf::RenderWindow window;
	sf::RectangleShape wybierzKarte;
	sf::Texture texture;
	sf::Texture texturePlansza;
	sf::Texture uzytyMusik;
	sf::View view;
	
	sf::Sprite plansza;
	
	//Zmienne licytacji, napisów
	sf::Font czcionka;
	sf::Text punkty1, punkty2, punkty3, wygrana, przegrana, aByKontynuowac;
	sf::Text lic[5];
	int pkt1, pkt2, pkt3, pkt1tmp, pkt2tmp, pkt3tmp;
	float a, b;
	float yLicytacja;

	//Menu
	Menu menu;
	int przejscieDo;

	//Zmienne mechaniki gry
	int ktoraKarta;
	int ktoraKartaMusik;
	int przyznanyMusik;
	int kartyUzyte[8];
	int kartyUzyteMusik[3];
	int doResetuZostalo;
	int wygrywaG1, wygrywaG2, wygrywaG3;
	int prowadziGracz;
	bool czyReset;
	bool czyMusik;
	bool musikCzyNie;
	bool wystawienie;
	pair<bool,bool> czyJaProwadze;
	int punkty;

	//Zmienne licytacji
	Licytacja licytacja;
	int licytacjaJa, licytacjaBot2, licytacjaBot3;
	bool czyLicytacja;

	//Zmienne meldunków, kart bota
	pair<pair<bool, int>,string> meldunekKrole[4];
	pair<pair<bool, int>,string> meldunekKrolowe[4];
	bool jakiMeldunek1[4];
	bool jakiMeldunek2[4];
	bool jakiMeldunek3[4];
	int ileMeldunkow;
	sf::Text meldunek[4];
	Karta wybranaPrzezBota;

	Karta* poprzednia1;
	Karta* poprzednia2;
	Karta* poprzednia3;

	Zasady zasady;
	
	//Funkcje okna
	void przebieg();
	void okno();
	void pozycjaKart();
	
	//Funkcje poruszania siê po oknie
	void wLewo();
	void wPrawo();
	void wDol();
	void wGore();
	void wystawKarte(Karta&);

	//Funkcje mechaniki
	int tmp, tmp2{};
	void pobieranieWartosci(Karta&, Karta&);
	Karta pobierzWartosciBot(Karta&, Karta[], float);
	Karta pobierzWartosciBot(Karta[], float);
	void pobierzAdresBot(Karta&);
	void wybierzMusik();
	void reset();
	void usunKarty();

	void czyMeldunek(Karta[], bool[]);
	void podajMeldunek(bool[]);

	void mechanika(Karta&, Karta&, Karta&);
	void podliczPunkty(string, string, string);

	void sprawdzLicytacje();

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
	pkt1tmp(0),
	pkt2tmp(0),
	pkt3tmp(0),
	a(630.0f),
	b(710.0f),
	yLicytacja(250.0f),
	czyReset(false),
	czyMusik(true),
	musikCzyNie(true),
	wystawienie(true),
	czyLicytacja(true),
	czyJaProwadze(true, true),
	poprzednia1(nullptr),
	poprzednia2(nullptr),
	poprzednia3(nullptr),
	przejscieDo(0),
	ktoraKarta(0),
	ktoraKartaMusik(0),
	przyznanyMusik(0),
	doResetuZostalo(0),
	ileMeldunkow(0),
	wygrywaG1(0),
	wygrywaG2(0),
	wygrywaG3(0),
	prowadziGracz(1),
	licytacjaJa(0),
	licytacjaBot2(0),
	licytacjaBot3(0),
	punkty(0),
	tmp(0)
{
	texture.loadFromFile("talia.png");
	texturePlansza.loadFromFile("plansza.png");
	uzytyMusik.loadFromFile("uzyta.png");
	
	plansza.setTexture(texturePlansza);

	view.setSize(sf::Vector2f(1200.0f, 800.0f));
	view.setCenter(sf::Vector2f(600.0f, 400.0f));

	czcionka.loadFromFile("sansation.ttf");
	punkty1.setFont(czcionka);
	punkty2.setFont(czcionka);
	punkty3.setFont(czcionka);
	punkty1.setFillColor(sf::Color::Red);
	punkty2.setFillColor(sf::Color::Red);
	punkty3.setFillColor(sf::Color::Red);
	punkty1.setString(to_string(pkt1));
	punkty2.setString(to_string(pkt2));
	punkty3.setString(to_string(pkt3));
	punkty1.setPosition(sf::Vector2f(100.0f, 160.0f));
	punkty2.setPosition(sf::Vector2f(100.0f, 600.0f));
	punkty3.setPosition(sf::Vector2f(900.0f, 400.0f));
	wygrana.setString("Wygrana!");
	wygrana.setFont(czcionka);
	wygrana.setFillColor(sf::Color::Red);
	wygrana.setPosition(sf::Vector2f(600.0f, 350.0f));
	przegrana.setString("Przegrana!");
	przegrana.setFont(czcionka);
	przegrana.setFillColor(sf::Color::Red);
	przegrana.setPosition(sf::Vector2f(600.0f, 350.0f));

	aByKontynuowac.setFont(czcionka);
	aByKontynuowac.setFillColor(sf::Color::Green);
	aByKontynuowac.setCharacterSize(36);
	aByKontynuowac.setString("Wcisnij A by kontynuowac!");
	aByKontynuowac.setPosition(sf::Vector2f(300.0f, 550.0f));
	
	wczytajKarty(talia);
	przypiszObraz(talia, &texture);

	wybierzKarte.setSize(sf::Vector2f(60.0f, 60.0f));
	wybierzKarte.setFillColor(sf::Color::Red);
	wybierzKarte.setOrigin(sf::Vector2f(wybierzKarte.getGlobalBounds().width / 2, wybierzKarte.getGlobalBounds().height / 2));


	for (int i = 0; i < 8; i++)
		kartyUzyte[i] = -1;
	for (int i = 0; i < 3; i++)
		kartyUzyteMusik[i] = -1;

	for (int i = 0; i < 4; i++) {
		meldunekKrole[i].first.second = 0;
		meldunekKrole[i].first.first = false;
		meldunekKrolowe[i].first.second = 0;
		meldunekKrolowe[i].first.first = false;
		jakiMeldunek1[i] = false;
		jakiMeldunek2[i] = false;
		jakiMeldunek3[i] = false;
		meldunek[i].setFont(czcionka);
		meldunek[i].setString("");
		meldunek[i].setPosition(sf::Vector2f(-100.0f, -100.0f));
	}
	meldunekKrole[0].second = "karo";
	meldunekKrolowe[0].second = "karo";
	meldunekKrole[1].second = "kier";
	meldunekKrolowe[1].second = "kier";
	meldunekKrole[2].second = "pik";
	meldunekKrolowe[2].second = "pik";
	meldunekKrole[3].second = "trefl";
	meldunekKrolowe[3].second = "trefl";
	meldunek[0].setString("karo");
	meldunek[1].setString("kier");
	meldunek[2].setString("pik");
	meldunek[3].setString("trefl");
}

void Gra::pozycjaKart() {
	for (int i = 0; i < 8; i++) {
		gracz1[i].card.setPosition(sf::Vector2f(120.0f + (110.0f * i), 80.0f));
		gracz3[i].card.setPosition(sf::Vector2f(1100.0f, 100.0f + (100.0f * i)));
		gracz2[i].card.setPosition(sf::Vector2f(120.0f + (110.0f * i), 700.0f));
	}
	for (int i = 0; i < 3; i++) {
		musik[i].card.setPosition(sf::Vector2f(450.0f + (110.0f * i), 450.0f));
	}
}


//**************Wybór kart******************//

void Gra::wLewo() {
	cout << "Lewo" << endl;
	if (doResetuZostalo == 7)
		wybierzKarte.setPosition(sf::Vector2f(gracz1[0].card.getPosition().x, gracz1[0].card.getPosition().y));
	if (!musikCzyNie) {
		ktoraKartaMusik = 0;
		if (ktoraKarta >= 0) {
			do {
				if (ktoraKarta == 0)
					ktoraKarta = 7;
				else
					ktoraKarta--;
			} while (ktoraKarta == kartyUzyte[ktoraKarta]);
			wybierzKarte.setPosition(sf::Vector2f(gracz1[ktoraKarta].card.getPosition().x, gracz1[ktoraKarta].card.getPosition().y));
		}
	}
	else {
		ktoraKarta = 0;
		if (ktoraKartaMusik >= 0) {
			do {
				if (ktoraKartaMusik == 0)
					ktoraKartaMusik = 2;
				else
					ktoraKartaMusik--;
			} while (ktoraKartaMusik == kartyUzyteMusik[ktoraKartaMusik]);
			wybierzKarte.setPosition(sf::Vector2f(musik[ktoraKartaMusik].card.getPosition().x, musik[ktoraKartaMusik].card.getPosition().y));
		}
	}
}
void Gra::wPrawo() {
	cout << "Prawo" << endl;
	if (doResetuZostalo == 7)
		wybierzKarte.setPosition(sf::Vector2f(gracz1[0].card.getPosition().x, gracz1[0].card.getPosition().y));
	if (!musikCzyNie) {
		ktoraKartaMusik = 0;
		if (ktoraKarta <= 7) {
			do {
				if (ktoraKarta == 7)
					ktoraKarta = 0;
				else
					ktoraKarta++;
			} while (ktoraKarta == kartyUzyte[ktoraKarta]);
			wybierzKarte.setPosition(sf::Vector2f(gracz1[ktoraKarta].card.getPosition().x, gracz1[ktoraKarta].card.getPosition().y));
		}
	}
	else {
		ktoraKarta = 0;
		if (ktoraKartaMusik <= 2) {
			do {
				if (ktoraKartaMusik == 2)
					ktoraKartaMusik = 0;
				else
					ktoraKartaMusik++;
			} while (ktoraKartaMusik == kartyUzyteMusik[ktoraKartaMusik]);
			wybierzKarte.setPosition(sf::Vector2f(musik[ktoraKartaMusik].card.getPosition().x, musik[ktoraKartaMusik].card.getPosition().y));
		}
	}
}
void Gra::wDol() {
	cout <<"yLicytacja: " << yLicytacja << endl;
	if (yLicytacja < 450.0f) {
		yLicytacja += 50.0f;
		wybierzKarte.setPosition(sf::Vector2f(150.0f, yLicytacja));
	}
	else {
		yLicytacja = 250.0f;
		wybierzKarte.setPosition(sf::Vector2f(150.0f, yLicytacja));
	}
}
void Gra::wGore() {
	cout << "yLicytacja: " << yLicytacja << endl;
	if (yLicytacja > 250.0f) {
		yLicytacja -= 50.0f;
		wybierzKarte.setPosition(sf::Vector2f(150.0f, yLicytacja));
	}
	else {
		yLicytacja = 450.0f;
		wybierzKarte.setPosition(sf::Vector2f(150.0f, yLicytacja));
	}
}
void Gra::wystawKarte(Karta& karta) {
	cout << "Wystawiam Karte" << endl;
	karta.card.setPosition(sf::Vector2f(550.0f, 400.0f));
	wybierzKarte.setPosition(sf::Vector2f(karta.card.getPosition().x, karta.card.getPosition().y));
}
void Gra::wybierzMusik() {
	cout << "Wybieram musik" << endl;
	sf::Vector2u textureSize = texture.getSize();
	textureSize.x /= 6;
	textureSize.y /= 4;
	int x = musik[ktoraKartaMusik].wartosc %6;
	int y = musik[ktoraKartaMusik].wartosc / 6;

	switch (przyznanyMusik) {
	case 0:
		gracz1[7].figura = musik[ktoraKartaMusik].figura;
		gracz1[7].kolor = musik[ktoraKartaMusik].kolor;
		gracz1[7].wartosc = musik[ktoraKartaMusik].wartosc;
		gracz1[7].card.setTexture(texture);
		gracz1[7].card.setTextureRect(sf::IntRect(textureSize.x * x, textureSize.y * y, textureSize.x, textureSize.y));
		gracz1[7].card.setPosition(sf::Vector2f(890.0f, 80.0f));
		gracz1[7].card.setOrigin(sf::Vector2f(gracz1[7].card.getGlobalBounds().width / 2, gracz1[7].card.getGlobalBounds().height / 2));
		musik[ktoraKartaMusik].card.setTexture(uzytyMusik);
		czyMeldunek(gracz1, jakiMeldunek1);
		break;
	case 1:
		gracz2[7].figura = musik[ktoraKartaMusik].figura;
		gracz2[7].kolor = musik[ktoraKartaMusik].kolor;
		gracz2[7].wartosc = musik[ktoraKartaMusik].wartosc;
		gracz2[7].card.setTexture(texture);
		gracz2[7].card.setTextureRect(sf::IntRect(textureSize.x * x, textureSize.y * y, textureSize.x, textureSize.y));
		gracz2[7].card.setPosition(sf::Vector2f(890.0f, 700.0f));
		gracz2[7].card.setOrigin(sf::Vector2f(gracz2[7].card.getGlobalBounds().width / 2, gracz2[7].card.getGlobalBounds().height / 2));
		musik[ktoraKartaMusik].card.setTexture(uzytyMusik);
		czyMeldunek(gracz2, jakiMeldunek2);
		break;
	case 2:
		gracz3[7].figura = musik[ktoraKartaMusik].figura;
		gracz3[7].kolor = musik[ktoraKartaMusik].kolor;
		gracz3[7].wartosc = musik[ktoraKartaMusik].wartosc;
		gracz3[7].card.setTexture(texture);
		gracz3[7].card.setTextureRect(sf::IntRect(textureSize.x * x, textureSize.y * y, textureSize.x, textureSize.y));
		gracz3[7].card.setPosition(sf::Vector2f(1100.0f, 750.0f));
		gracz3[7].card.setOrigin(sf::Vector2f(gracz3[7].card.getGlobalBounds().width / 2, gracz3[7].card.getGlobalBounds().height / 2));
		musik[ktoraKartaMusik].card.setTexture(uzytyMusik);
		przyznanyMusik = 0;
		wybierzKarte.setPosition(sf::Vector2f(gracz1[0].card.getPosition().x, gracz1[0].card.getPosition().y));
		musikCzyNie = false;
		czyMusik = false;
		czyMeldunek(gracz3, jakiMeldunek3);
		break;
	}

	przyznanyMusik++;

}

//*************Mechanika gry****************//
void Gra::usunKarty() {
	cout << "Usuwam karty" << endl;
	poprzednia1->card.setPosition(sf::Vector2f(-100.0f, -100.0f));
	poprzednia2->card.setPosition(sf::Vector2f(-100.0f, -100.0f));
	poprzednia3->card.setPosition(sf::Vector2f(-100.0f, -100.0f));
}
void Gra::czyMeldunek(Karta kartyGracza[], bool jakiMeldunek[]){
	cout << "Sprawdzam czy meldunek" << endl;
	for (int i = 0; i < 8; i++) {
		if (kartyGracza[i].figura == "K") {
			if (kartyGracza[i].kolor == "karo") {
				meldunekKrole[0].first.first = true;
				meldunekKrole[0].first.second = i;
			}
			else if (kartyGracza[i].kolor == "kier") {
				meldunekKrole[1].first.first = true;
				meldunekKrole[1].first.second = i;
			}
			else if (kartyGracza[i].kolor == "pik") {
				meldunekKrole[2].first.first = true;
				meldunekKrole[2].first.second = i;
			}
			else if (kartyGracza[i].kolor == "trefl") {
				meldunekKrole[3].first.first = true;
				meldunekKrole[3].first.second = i;
			}
		}
		else if (kartyGracza[i].figura == "Q") {
			if (kartyGracza[i].kolor == "karo") {
				meldunekKrolowe[0].first.first = true;
				meldunekKrolowe[0].first.second = i;
			}
			else if (kartyGracza[i].kolor == "kier") {
				meldunekKrolowe[1].first.first = true;
				meldunekKrolowe[1].first.second = i;
			}
			else if (kartyGracza[i].kolor == "pik") {
				meldunekKrolowe[2].first.first = true;
				meldunekKrolowe[2].first.second = i;
			}
			else if (kartyGracza[i].kolor == "trefl") {
				meldunekKrolowe[3].first.first = true;
				meldunekKrolowe[3].first.second = i;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (meldunekKrole[i].first.first == true && meldunekKrolowe[i].first.first == true) {
			jakiMeldunek[i] = true;
			ileMeldunkow++;
		}
	}
	podajMeldunek(jakiMeldunek);
	for (int i = 0; i < 4; i++) {
		meldunekKrole[i].first.first = false;
		meldunekKrolowe[i].first.first = false;
		meldunekKrole[i].first.second = 0;
		meldunekKrolowe[i].first.second = 0;
	}
}
void Gra::podajMeldunek(bool jakiMeldunek[]) {
	if (ileMeldunkow != 0) {
		for (int i = 0; i < 4; i++) {
			if (jakiMeldunek[i] == true) {
				cout << "Mamy meldunek gracza: " << meldunekKrole[i].second << endl ;
			}
		}
	}
}
void Gra::pobieranieWartosci(Karta& kartaGracza, Karta& kartaBota) {
	cout << "Pobieranie Wartosci" << endl;
	Karta kartaGracza2, kartaGracza3;
	if (czyJaProwadze.first == true && czyJaProwadze.second == true) {
		kartaGracza2 = pobierzWartosciBot(kartaGracza, gracz2, a);
		kartaGracza3 = pobierzWartosciBot(kartaGracza, gracz3, b);
		mechanika(kartaGracza, kartaGracza2, kartaGracza3);
	}
	else if (czyJaProwadze.first == false && czyJaProwadze.second == true) {
		kartaGracza3 = pobierzWartosciBot(kartaBota, gracz3, b);
		mechanika(kartaGracza, kartaBota, kartaGracza3);
	}
	else if (czyJaProwadze.first == true && czyJaProwadze.second == false) {
		kartaGracza2 = pobierzWartosciBot(kartaBota, gracz2, a);
		mechanika(kartaGracza, kartaGracza2, kartaBota);
	}
	poprzednia1 = &kartaGracza;
	tmp++;
	punkty1.setString(to_string(pkt1));
	punkty2.setString(to_string(pkt2));
	punkty3.setString(to_string(pkt3));
}

Karta Gra::pobierzWartosciBot(Karta kartaBota[], float x) {
	cout << "Pobieranie wartosci Bota" << endl;
	int indexKartyBota{};
	int wartosc{};
	if (wartosc == 0 && indexKartyBota == 0) {
		for (int i = 0; i < 8; i++) {
			if (kartaBota[i].uzyta == false) {
				indexKartyBota = i;
				break;
			}
		}
	}
	cout << "Bot wygywa i wybiera: " << kartaBota[indexKartyBota].figura << "   " << kartaBota[indexKartyBota].kolor << endl;
	kartaBota[indexKartyBota].uzyta = true;
	kartaBota[indexKartyBota].card.setPosition(sf::Vector2f(x, 400.0f));
	pobierzAdresBot(kartaBota[indexKartyBota]);
	return kartaBota[indexKartyBota];
}
Karta Gra::pobierzWartosciBot(Karta& kartaGracza, Karta kartaBota[], float x){
	cout << "Pobieranie wartosci Bota 2" << endl;
	int indexKartyBota{};
	int wartosc{};
	for (int i = 0; i < 8; i++) {
		if (kartaGracza.kolor == kartaBota[i].kolor && kartaBota[i].uzyta == false) {
			if (kartaBota[i].wartosc > wartosc) {
				wartosc = kartaBota[i].wartosc;
				indexKartyBota = i;
			}
		}
	}
	if (wartosc == 0 && indexKartyBota == 0) {
		for (int i = 0; i < 8; i++) {
			if (kartaBota[i].uzyta == false) {
				indexKartyBota = i;
				break;
			}
		}
	}
	cout << "Bot przegrywa i wybiera: " << kartaBota[indexKartyBota].figura << "   " << kartaBota[indexKartyBota].kolor << endl;
	kartaBota[indexKartyBota].uzyta = true;
	kartaBota[indexKartyBota].card.setPosition(sf::Vector2f(x, 400.0f));
	pobierzAdresBot(kartaBota[indexKartyBota]);
	return kartaBota[indexKartyBota];
}
void Gra::pobierzAdresBot(Karta& kartaBot){
	cout <<"Co sie kryje w adresie: " << kartaBot.figura << "  " << kartaBot.kolor << endl;
	if (tmp2 == 0) {
		poprzednia2 = &kartaBot;
		tmp2 = 1;
	}
	else {
		poprzednia3 = &kartaBot;
		tmp2 = 0;
	}
}	

void Gra::mechanika(Karta& kartaGracza, Karta& kartaGracza2, Karta& kartaGracza3) {

	cout << "Gracz1: " << kartaGracza.figura << "___" << kartaGracza.kolor << "___" << kartaGracza.wartosc << endl;
	cout << "Gracz2: " << kartaGracza2.figura << "___" << kartaGracza2.kolor << "___" << kartaGracza2.wartosc << endl;
	cout << "Gracz3: " << kartaGracza3.figura << "___" << kartaGracza3.kolor << "___" << kartaGracza3.wartosc << endl;
	if (czyJaProwadze.first == true && czyJaProwadze.second == true) {
		if (kartaGracza.kolor == kartaGracza2.kolor && kartaGracza.kolor == kartaGracza3.kolor) {
			if (kartaGracza.wartosc > kartaGracza2.wartosc && kartaGracza.wartosc > kartaGracza3.wartosc) {
				cout << "  Wygrywa gracz 1 (te same kolory)  " << endl;
				wygrywaG1 = 1;
				prowadziGracz = 1;
				czyJaProwadze.first = true;
				czyJaProwadze.second = true;
			}
			else if (kartaGracza2.wartosc > kartaGracza.wartosc && kartaGracza2.wartosc > kartaGracza3.wartosc) {
				cout << "  Wygrywa gracz 2 (te same kolory)  " << endl;
				wygrywaG2 = 1;
				prowadziGracz = 2;
				czyJaProwadze.first = false;
				czyJaProwadze.second = true;
			}
			else if (kartaGracza3.wartosc > kartaGracza.wartosc && kartaGracza3.wartosc > kartaGracza2.wartosc) {
				cout << "  Wygrywa gracz 3 (te same kolory)  " << endl;
				wygrywaG3 = 1;
				prowadziGracz = 3;
				czyJaProwadze.first = true;
				czyJaProwadze.second = false;
			}
		}
		else if (kartaGracza.kolor == kartaGracza2.kolor) {
			if (kartaGracza.wartosc > kartaGracza2.wartosc) {
				cout << "  Wygrywa gracz 1 (gracz 2 i 1 te same kolory)  " << endl;
				wygrywaG1 = 1;
				prowadziGracz = 1;
				czyJaProwadze.first = true;
				czyJaProwadze.second = true;
			}
			else {
				cout << "  Wygrywa gracz 2 (gracz 2 i 1 te same kolory)  " << endl;
				wygrywaG2 = 1;
				prowadziGracz = 2;
				czyJaProwadze.first = false;
				czyJaProwadze.second = true;
			}
		}
		else if (kartaGracza.kolor == kartaGracza3.kolor) {
			if (kartaGracza.wartosc > kartaGracza3.wartosc) {
				cout << "  Wygrywa gracz 1 (gracz 2 i 1 te same kolory)  " << endl;
				wygrywaG1 = 1;
				prowadziGracz = 1;
				czyJaProwadze.first = true;
				czyJaProwadze.second = true;
			}
			else {
				cout << "  Wygrywa gracz 3 (gracz 2 i 1 te same kolory)  " << endl;
				wygrywaG3 = 1;
				prowadziGracz = 3;
				czyJaProwadze.first = true;
				czyJaProwadze.second = false;
			}
		}
		else {
			cout << "  Wygrywa gracz 1 (brak kolorow)     " << endl;
			wygrywaG1 = 1;
			prowadziGracz = 1;
			czyJaProwadze.first = true;
			czyJaProwadze.second = true;
		}
	}
	else if (czyJaProwadze.first == false && czyJaProwadze.second == true) {
		if (kartaGracza2.kolor == kartaGracza.kolor && kartaGracza2.kolor == kartaGracza3.kolor) {
			if (kartaGracza2.wartosc > kartaGracza.wartosc && kartaGracza2.wartosc > kartaGracza3.wartosc) {
				cout << "  Wygrywa gracz 2 (te same kolory)  " << endl;
				wygrywaG2 = 1;
				prowadziGracz = 2;
				czyJaProwadze.first = false;
				czyJaProwadze.second = true;
			}
			else if (kartaGracza.wartosc > kartaGracza2.wartosc && kartaGracza.wartosc > kartaGracza3.wartosc) {
				cout << "  Wygrywa gracz 1 (te same kolory)  " << endl;
				wygrywaG1 = 1;
				prowadziGracz = 1;
				czyJaProwadze.first = true;
				czyJaProwadze.second = true;
			}
			else if (kartaGracza3.wartosc > kartaGracza2.wartosc && kartaGracza3.wartosc > kartaGracza.wartosc) {
				cout << "  Wygrywa gracz 3 (te same kolory)  " << endl;
				wygrywaG3 = 1;
				prowadziGracz = 3;
				czyJaProwadze.first = true;
				czyJaProwadze.second = false;
			}
		}
		else if (kartaGracza2.kolor == kartaGracza.kolor) {
			if (kartaGracza2.wartosc > kartaGracza.wartosc) {
				cout << "  Wygrywa gracz 2 (gracz 2 i 1 te same kolory)  " << endl;
				wygrywaG2 = 1;
				prowadziGracz = 2;
				czyJaProwadze.first = false;
				czyJaProwadze.second = true;
			}
			else {
				cout << "  Wygrywa gracz 1 (gracz 2 i 1 te same kolory)  " << endl;
				wygrywaG1 = 1;
				prowadziGracz = 1;
				czyJaProwadze.first = true;
				czyJaProwadze.second = true;
			}
		}
		else if (kartaGracza2.kolor == kartaGracza3.kolor) {
			if (kartaGracza2.wartosc > kartaGracza3.wartosc) {
				cout << "  Wygrywa gracz 2 (gracz 2 i 3 te same kolory)  " << endl;
				wygrywaG2 = 1;
				prowadziGracz = 2;
				czyJaProwadze.first = false;
				czyJaProwadze.second = true;
			}
			else {
				cout << "  Wygrywa gracz 3 (gracz 2 i 1 te same kolory)  " << endl;
				wygrywaG3 = 1;
				prowadziGracz = 3;
				czyJaProwadze.first = true;
				czyJaProwadze.second = false;
			}
		}
		else {
			cout << "  Wygrywa gracz 2 (brak kolorow)     " << endl;
			wygrywaG2 = 1;
			prowadziGracz = 2;
			czyJaProwadze.first = false;
			czyJaProwadze.second = true;
		}
	}
	else if (czyJaProwadze.first == true && czyJaProwadze.second == false) {
		if (kartaGracza3.kolor == kartaGracza2.kolor && kartaGracza3.kolor == kartaGracza.kolor) {
			if (kartaGracza3.wartosc > kartaGracza2.wartosc && kartaGracza3.wartosc > kartaGracza.wartosc) {
				cout << "  Wygrywa gracz 3 (te same kolory)  " << endl;
				wygrywaG3 = 1;
				prowadziGracz = 3;
				czyJaProwadze.first = true;
				czyJaProwadze.second = false;
			}
			else if (kartaGracza2.wartosc > kartaGracza3.wartosc && kartaGracza2.wartosc > kartaGracza.wartosc) {
				cout << "  Wygrywa gracz 2 (te same kolory)  " << endl;
				wygrywaG2 = 1;
				prowadziGracz = 2;
				czyJaProwadze.first = false;
				czyJaProwadze.second = true;
			}
			else if (kartaGracza.wartosc > kartaGracza3.wartosc && kartaGracza.wartosc > kartaGracza2.wartosc) {
				cout << "  Wygrywa gracz 1 (te same kolory)  " << endl;
				wygrywaG1 = 1;
				prowadziGracz = 1;
				czyJaProwadze.first = true;
				czyJaProwadze.second = true;
			}
		}
		else if (kartaGracza3.kolor == kartaGracza2.kolor) {
			if (kartaGracza3.wartosc > kartaGracza2.wartosc) {
				cout << "  Wygrywa gracz 3 (gracz 2 i 3 te same kolory)  " << endl;
				wygrywaG3 = 1;
				prowadziGracz = 3;
				czyJaProwadze.first = true;
				czyJaProwadze.second = false;
			}
			else {
				cout << "  Wygrywa gracz 2 (gracz 2 i 3 te same kolory)  " << endl;
				wygrywaG2 = 1;
				prowadziGracz = 2;
				czyJaProwadze.first = false;
				czyJaProwadze.second = true;
			}
		}
		else if (kartaGracza3.kolor == kartaGracza.kolor) {
			if (kartaGracza3.wartosc > kartaGracza.wartosc) {
				cout << "  Wygrywa gracz 3 (gracz 3 i 1 te same kolory)  " << endl;
				wygrywaG3 = 1;
				prowadziGracz = 3;
				czyJaProwadze.first = true;
				czyJaProwadze.second = false;
			}
			else {
				cout << "  Wygrywa gracz 1 (gracz 3 i 1 te same kolory)  " << endl;
				wygrywaG1 = 1;
				prowadziGracz = 1;
				czyJaProwadze.first = true;
				czyJaProwadze.second = true;
			}
		}
		else {
			cout << "  Wygrywa gracz 3 (brak kolorow)     " << endl;
			wygrywaG3 = 1;
			prowadziGracz = 3;
			czyJaProwadze.first = true;
			czyJaProwadze.second = false;
		}
	}
	podliczPunkty(kartaGracza.figura, kartaGracza2.figura, kartaGracza3.figura);
}
void Gra::podliczPunkty(string pierwszy, string drugi, string trzeci) {
	if (pierwszy == "A")
		punkty += 11;
	else if (pierwszy == "K")
		punkty += 4;
	else if (pierwszy == "Q")
		punkty += 3;
	else if (pierwszy == "J")
		punkty += 2;
	else if (pierwszy == "10")
		punkty += 10;

	if (drugi == "A")
		punkty += 11;
	else if (drugi == "K")
		punkty += 4;
	else if (drugi == "Q")
		punkty += 3;
	else if (drugi == "J")
		punkty += 2;
	else if (drugi == "10")
		punkty += 10;

	if(trzeci == "A")
		punkty += 11;
	else if (trzeci == "K")
		punkty += 4;
	else if (trzeci == "Q")
		punkty += 3;
	else if (trzeci == "J")
		punkty += 2;
	else if (trzeci == "10")
		punkty += 10;
	
	if (wygrywaG1 == 1) {
		pkt1tmp += punkty;
	}
	else if (wygrywaG2 == 1) {
		pkt2tmp += punkty;
	}
	else if (wygrywaG3 == 1) {
		pkt3tmp += punkty;
	}
	cout << pkt1tmp << "  |   " << pkt2tmp << "   |   " << pkt3tmp << endl;
	punkty = 0;
	if (doResetuZostalo == 7) {
		pkt1 += pkt1tmp;
		pkt2 += pkt2tmp;
		pkt3 += pkt3tmp;
		pkt1tmp = 0;
		pkt2tmp = 0;
		pkt3tmp = 0;
	}
}

void Gra::sprawdzLicytacje() {
	licytacjaJa = licytacja.getLicytujeZa();
	licytacjaBot2 = licytacja.licytujBot(gracz2);
	licytacjaBot3 = licytacja.licytujBot(gracz3);
}
void Gra::reset() {
	cout <<"====  Do resetu zostalo: "<< doResetuZostalo << endl;
	if (doResetuZostalo == 8 || czyReset == true) {
		cout << "RESET!" << endl;
		tmp = 0;
		przyznanyMusik = 0;
		czyReset = false;
		czyMusik = true;
		musikCzyNie = true;
		wystawienie = true;
		poprzednia1 = nullptr;
		poprzednia2 = nullptr;
		poprzednia3 = nullptr;
		czyJaProwadze.first = true;
		czyJaProwadze.second = true;
		gracz1[7].card.setPosition(sf::Vector2f(-100.0f, -100.0f));
		gracz2[7].card.setPosition(sf::Vector2f(-100.0f, -100.0f));
		gracz3[7].card.setPosition(sf::Vector2f(-100.0f, -100.0f));
		wybierzKarte.setPosition(sf::Vector2f(450.0f, 450.0f));
		doResetuZostalo = 0;
		prowadziGracz = 1;
		if (czyReset == true) {
			pkt1 = 0;
			pkt2 = 0;
			pkt3 = 0;
			pkt1tmp = 0;
			pkt2tmp = 0;
			pkt3tmp = 0;
			punkty1.setString(to_string(pkt1));
			punkty2.setString(to_string(pkt2));
			punkty3.setString(to_string(pkt3));
		}
		for (int i = 0; i < 8; i++) {
			kartyUzyte[i] = -1;
			gracz1[i].uzyta = false;
			gracz2[i].uzyta = false;
			gracz3[i].uzyta = false;
		}
		for (int i = 0; i < 3; i++)
			kartyUzyteMusik[i] = -1;
		for (int i = 0; i < 4; i++) {
			meldunekKrole[i].first.first = false;
			meldunekKrole[i].first.second = 0;
			meldunekKrolowe[i].first.first = false;
			meldunekKrolowe[i].first.second = 0;
			jakiMeldunek1[i] = false;
			jakiMeldunek2[i] = false;
			jakiMeldunek3[i] = false;
		}
		przetasujKarty(talia);
		rozdanie(gracz1, gracz2, gracz3, talia, musik);
		pozycjaKart();
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
						pozycjaKart();
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
					if (doResetuZostalo != 7)
						wLewo();
					break;
				case sf::Keyboard::Right:
					if (doResetuZostalo != 7)
						wPrawo();
					break;
				case sf::Keyboard::A:
					if (czyMusik) {
						wybierzMusik();
						kartyUzyteMusik[ktoraKartaMusik] = ktoraKartaMusik;
						if (ktoraKartaMusik != 2)
							wPrawo();
						else
							wLewo();
					}
					else {
						if (wystawienie) {
							if (prowadziGracz == 1) {
								wystawKarte(gracz1[ktoraKarta]);
								pobieranieWartosci(gracz1[ktoraKarta], wybranaPrzezBota);
								kartyUzyte[ktoraKarta] = ktoraKarta;
								if (doResetuZostalo != 7)
									wPrawo();
								wystawienie = false;
							}
							else if (prowadziGracz == 2) {
								wybranaPrzezBota = pobierzWartosciBot(gracz2, a);
								prowadziGracz = 1;
							}
							else if (prowadziGracz == 3) {
								wybranaPrzezBota = pobierzWartosciBot(gracz3, b);
								prowadziGracz = 1;
							}
						}
						else {
							usunKarty();
							wygrywaG1 = 0;
							wygrywaG2 = 0;
							wygrywaG3 = 0;
							wystawienie = true;
							doResetuZostalo++;
							reset();
						}
					}
					break;
				case sf::Keyboard::R:
					czyReset = true;
					reset();
					break;
				case sf::Keyboard::L:
					if (licytacja.getLicytacjaUzyte() == -1) {
						licytacja.licytuj();
						sprawdzLicytacje();
					}
					break;
				case sf::Keyboard::Down:
					if (czyLicytacja) {
						licytacja.wDol();
					}
					break;
				case sf::Keyboard::Up:
					if (czyLicytacja){
						licytacja.wGore();
					}
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
		if(!czyLicytacja)
			window.draw(wybierzKarte);
		for (int i = 0; i < 8; i++) {
			window.draw(gracz1[i].card);
			window.draw(gracz2[i].card);
			window.draw(gracz3[i].card);
		}
		if (czyMusik && !czyLicytacja) {
			for (int i = 0; i < 3; i++) {
				window.draw(musik[i].card);
			}
		}
		if (!wystawienie) {
			window.draw(aByKontynuowac);
			if (wygrywaG1 == 1) {
				window.draw(wygrana);
			}
			else {
				window.draw(przegrana);
			}
		}
		if (czyLicytacja) {
			licytacja.draw(window);
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
	return 0;
}

//****************TO-DO LIST*********************
/*   __________________________________________________________________________________________________
	|* przycisk R jako reset/ od poczatku - (resetuje punkty, ustawienie kart, przetasowuje karty)     |
	|	~reset musi nastepowac po kazdej wygranej rundzie, bez resetu punktow (jakis bool)			   |  <- Zrobione
	|__________________________________________________________________________________________________|
	*normalne liczenie punktow, po osiagnieciu w ktorejs turze 1000 pkt, sprawdzane jest, kto ma najwiecej, ten wygrywa (³adny napis)
	
	*zamiast zasady napisac instrukja |-> sterowanie (ruszajace sie strzalki, a[accept] do zatwierdzania, r[reset] do resetu od podstaw)
									  |-> zasady gry (rozdanie, licytacja, meldunki (moze jakies ilustracje), musik, przebieg, zakonczenie.
		
	*meldunek, tworzenie tablicy kroli i dam, jezeli sa w kartach, jezeli maja ten sam kolor to meldunek ++.
		Po wyrzuceniu damy badz króla, je¿eli owy meldunek mamy to ten kolor jest kolorem dominuj¹cym.
	
	 _____________________________________________________
	|*wypisywanie musiku i wybieranie go				  |
	|													  | <--- Zrobione
	|=====================================================|
	|	if wybrana z musiku								  |
	|		to dodaj do gracza							  |
	|	if not											  |
	|		wybrana dodaj do gracza tego innego niz ja    |
	|		zamien moja wybrana z musiskiem miejscami	  |
	|_____________________________________________________|

	*po wyjsciu z gry (escape) gra musi siê resetowaæ, póki co tylko wychodzi a nie resetuj¹ siê punkty.
	
*/ 

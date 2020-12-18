#include<iostream>
#include<iomanip>
#include<vector>
#include<random>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include"naglowek.h"

using namespace std;


void wczytajKarty(Karta tab[24]) {
	cout << "Wczytuje karty";
	string kolor[4] = { "pik", "trefl", "karo", "kier" }, figura[6] = { "9","10","J","Q","K","A" };
	int wart{};

	//Dodanie do talii kart, wszystkie potrzebne do gry 24 karty (wszystkie 4 kolory + karty od 9 do Asa)
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			tab[wart].figura = figura[j];		
			tab[wart].kolor = kolor[i];
			tab[wart].wartosc = wart;
			wart++;
		}
	}
}

//Do zrobienia problem 1 **
void przypiszObraz(Karta tab[24], sf::Texture* texture) {
	sf::Vector2u taliaSize = texture->getSize();
	taliaSize.x /= 6;
	taliaSize.y /= 4;
	float a = 0, b = 20.0f;
	int n = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 6; j++) {
			tab[n].card.setTexture(*texture);
			tab[n].card.setTextureRect(sf::IntRect(taliaSize.x * j, taliaSize.y * i, taliaSize.x, taliaSize.y));
			tab[n].card.setPosition(sf::Vector2f(a + 20.0f, b));
			tab[n].card.setOrigin(sf::Vector2f(tab[n].card.getGlobalBounds().width / 2, tab[n].card.getGlobalBounds().height / 2));
			a += 50.0f;
			n++;
			if (n % 6 == 0) {
				b += 60.0f;
				a = 0;
			}
		}
	}
} 

void przetasujKarty(Karta tab[24]) {
	cout << "Tasuje ";
	Karta tmp;
	random_device device;
	mt19937 generator(device());
	uniform_int_distribution<int> random(0, 23);
	int zamiana1{}, zamiana2{};
	for (int i = 0; i < 1000; i++) {
		zamiana1 = random(generator);            // Tworzenie zmiennych, które bêd¹ przyjmowaæ randomowe wartoœci, które losowo przetasuj¹ nasz¹ taliê
		zamiana2 = random(generator);			 // za ka¿dym razem w inny sposób. Dla dok³adnego przetasowanie 24 karty s¹ tasowane 1000 razy.
		tmp.figura = tab[zamiana1].figura;
		tmp.kolor = tab[zamiana1].kolor;
		tmp.wartosc = tab[zamiana1].wartosc;			//Do tasowania u¿ywamy pomocniczej zmiennej, która podmienia wartoœci w dwóch losowych kartach
		tmp.card = tab[zamiana1].card;
		tab[zamiana1].figura = tab[zamiana2].figura;
		tab[zamiana1].kolor = tab[zamiana2].kolor;
		tab[zamiana1].wartosc = tab[zamiana2].wartosc;
		tab[zamiana1].card = tab[zamiana2].card;
		tab[zamiana2].figura = tmp.figura;
		tab[zamiana2].kolor = tmp.kolor;
		tab[zamiana2].wartosc = tmp.wartosc;
		tab[zamiana2].card = tmp.card;
	}
}

void rozdanie(Karta gracz1[8], Karta gracz2[8], Karta gracz3[8], Karta talia[24], Karta musik[3]) {
	int licznik{}, iloscKartWRece{};
	for (int i = 0; i < 21; i++) {									//Rozdanie 21 spoœród 24 kart, po 7 dla ka¿dego gracza, 3 pozosta³e (ostanie) karty id¹ na tzw. "musik"
		if (i % 3 == 0) {
			gracz1[iloscKartWRece].figura = talia[i].figura;		//Rozdawane karty po kolei s¹ przypisywane graczom;
			gracz1[iloscKartWRece].kolor = talia[i].kolor;
			gracz1[iloscKartWRece].wartosc = talia[i].wartosc;
			gracz1[iloscKartWRece].card = talia[i].card;
			licznik++;
		}
		else if (i % 3 == 1) {
			gracz2[iloscKartWRece].figura = talia[i].figura;
			gracz2[iloscKartWRece].kolor = talia[i].kolor;
			gracz2[iloscKartWRece].wartosc = talia[i].wartosc;
			gracz2[iloscKartWRece].card = talia[i].card;
			licznik++;
		}
		else {
			gracz3[iloscKartWRece].figura = talia[i].figura;
			gracz3[iloscKartWRece].kolor = talia[i].kolor;
			gracz3[iloscKartWRece].wartosc = talia[i].wartosc;
			gracz3[iloscKartWRece].card = talia[i].card;
			licznik++;
		}
		if (licznik == 3) {
			iloscKartWRece++;
			licznik = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		musik[i].figura = talia[i + 21].figura;
		musik[i].kolor = talia[i + 21].kolor;
		musik[i].wartosc = talia[i + 21].wartosc;
		musik[i].card = talia[i + 21].card;
 	}
}


// ** - zrobione

/*
Problem 1 - naprawione (wskaznik do obrazka)
	Po przerzuceniu funkcji do osobnego pliku tekstura nie wczytuje siê, karty s¹ jako osobne byty, ale wszystkie s¹ jednolicie bia³e
	* Pêtla przechodzi bez problemu
	* W mainie dzia³a bez problemu
	
===================================
*/
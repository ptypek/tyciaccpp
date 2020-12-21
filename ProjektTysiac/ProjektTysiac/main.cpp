#include <iostream>
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "naglowek.h"
#include "Menu.h"
#include "Zasady.h"
using namespace std;

class Gra{
private:
	Karta talia[24], gracz1[9], gracz2[9], gracz3[9], musik[3];
	sf::RenderWindow window;
	sf::RectangleShape wybierzKarte;
	sf::Texture texture;
	sf::Texture texturePlansza;
	sf::Texture uzytyMusik;
	sf::View view;
	
	sf::Sprite plansza;
	
	sf::Font czcionka;
	sf::Text punkty1, punkty2, punkty3, wygrana, przegrana, aByKontynuowac;
	int pkt1, pkt2, pkt3;
	float a, b;


	Menu menu;
	int przejscieDo;

	int ktoraKarta;
	int kartaDoWyrzucenia;
	int ktoraKartaMusik;
	int przyznanyMusik;
	int kartyUzyte[8];
	int kartyUzyteMusik[3];
	int doResetuZostalo;
	int wygrywaG1, wygrywaG2, wygrywaG3;
	bool czyReset;
	bool czyMusik;
	bool musikCzyNie;
	bool wystawienie;

	Karta* poprzednia1;
	Karta* poprzednia2;
	Karta* poprzednia3;

	Zasady zasady;

	void przebieg();
	void okno();
	void pozycjaKart();
	
	void wLewo();
	void wPrawo();
	void wystawKarte(Karta&);

	int tmp, tmp2{};
	void pobieranieWartosci(Karta&);
	Karta pobierzWartosciBot(Karta&, Karta[], float);
	void pobierzAdresBot(Karta&);
	void wybierzMusik();
	void reset();
	void usunKarty();

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
	a(630.0f),
	b(710.0f),
	czyReset(false),
	czyMusik(true),
	musikCzyNie(true),
	wystawienie(true),
	poprzednia1(nullptr),
	poprzednia2(nullptr),
	poprzednia3(nullptr),
	przejscieDo(0),
	ktoraKarta(0),
	kartaDoWyrzucenia(0),
	ktoraKartaMusik(0),
	przyznanyMusik(0),
	doResetuZostalo(0),
	wygrywaG1(0),
	wygrywaG2(0),
	wygrywaG3(0),
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
	if (czyMusik) {
		wybierzKarte.setPosition(sf::Vector2f(450.0f, 450.0f));
	}
	else {
		wybierzKarte.setPosition(sf::Vector2f(gracz1[0].card.getPosition().x,gracz1[0].card.getPosition().y));
	}

	for (int i = 0; i < 8; i++)
		kartyUzyte[i] = -1;
	for (int i = 0; i < 3; i++)
		kartyUzyteMusik[i] = -1;
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
void Gra::wystawKarte(Karta& karta) {
	karta.card.setPosition(sf::Vector2f(550.0f, 400.0f));
	wybierzKarte.setPosition(sf::Vector2f(karta.card.getPosition().x, karta.card.getPosition().y));
}
void Gra::wybierzMusik() {
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
		break;
	}

	przyznanyMusik++;

}

//*************Mechanika gry****************//
void Gra::usunKarty() {
	/*poprzednia1->card.setPosition(sf::Vector2f(300.0f, 300.0f));
	poprzednia2->card.setPosition(sf::Vector2f(400.0f, 300.0f));
	poprzednia3->card.setPosition(sf::Vector2f(500.0f, 300.0f));
	cout << "usunelo sie" << endl;*/
}


void Gra::pobieranieWartosci(Karta& kartaGracza) {

	if (poprzednia1 != nullptr) {
		poprzednia1->card.setPosition(sf::Vector2f(300.0f,300.0f));
		poprzednia2->card.setPosition(sf::Vector2f(600.0f, 600.0f));
		poprzednia3->card.setPosition(sf::Vector2f(800.0f, 800.0f));
	}
	Karta kartaGracza2 = pobierzWartosciBot(kartaGracza, gracz2, a);
	Karta kartaGracza3 = pobierzWartosciBot(kartaGracza, gracz3, b);



	poprzednia1 = &kartaGracza;
	

	cout << "Gracz1: " << kartaGracza.figura << "___" << kartaGracza.kolor << "___" << kartaGracza.wartosc << endl;
	cout << "Gracz2: " << kartaGracza2.figura << "___" << kartaGracza2.kolor << "___" << kartaGracza2.wartosc << endl;
	cout << "Gracz3: " << kartaGracza3.figura << "___" << kartaGracza3.kolor << "___" << kartaGracza3.wartosc << endl;

	if (kartaGracza.kolor == kartaGracza2.kolor && kartaGracza.kolor == kartaGracza3.kolor) {
		if (kartaGracza.wartosc > kartaGracza2.wartosc && kartaGracza.wartosc > kartaGracza3.wartosc) {
			cout << "  Wygrywa gracz 1 (te same kolory)  "<<endl;
			pkt1 ++ ;
			wygrywaG1 = 1;
		}
		else if (kartaGracza2.wartosc > kartaGracza.wartosc && kartaGracza2.wartosc > kartaGracza3.wartosc) {
			cout << "  Wygrywa gracz 2 (te same kolory)  " << endl;
			pkt2++;
			wygrywaG2 = 1;
		}
		else if (kartaGracza3.wartosc > kartaGracza.wartosc && kartaGracza3.wartosc > kartaGracza2.wartosc) {
			cout << "  Wygrywa gracz 3 (te same kolory)  " << endl;
			pkt3++;
			wygrywaG3 = 1;
		}
	}
	else if (kartaGracza.kolor == kartaGracza2.kolor) {
		if (kartaGracza.wartosc > kartaGracza2.wartosc) {
			cout << "  Wygrywa gracz 1 (gracz 2 i 1 te same kolory)  " << endl;
			pkt1++;
			wygrywaG1 = 1;
		}
		else {
			cout << "  Wygrywa gracz 2 (gracz 2 i 1 te same kolory)  " << endl;
			pkt2++;
			wygrywaG2 = 1;
		}
	}
	else if (kartaGracza.kolor == kartaGracza3.kolor) {
		if (kartaGracza.wartosc > kartaGracza3.wartosc) {
			cout << "  Wygrywa gracz 1 (gracz 2 i 1 te same kolory)  " << endl;
			pkt1++;
			wygrywaG1 = 1;
		}
		else {
			cout << "  Wygrywa gracz 3 (gracz 2 i 1 te same kolory)  " << endl;
			pkt3++;
			wygrywaG3 = 1;
		}
	}
	else {
		cout << "  Wygrywa gracz 1 (brak kolorow)     " << endl;
		pkt1++;
		wygrywaG1 = 1;
	}
	tmp++;
	reset(); 
	punkty1.setString(to_string(pkt1));
	punkty2.setString(to_string(pkt2));
	punkty3.setString(to_string(pkt3));
}

Karta Gra::pobierzWartosciBot(Karta& kartaGracza, Karta kartaBota[], float x){
	int indexKartyBota{};
	int wartosc{};
	for (int i = 0; i < 8; i++) {
		if (kartaGracza.kolor == kartaBota[i].kolor && kartaBota[i].uzyta == false) {
			if (kartaBota[i].wartosc > wartosc) {
				wartosc = kartaBota[i].wartosc;
				indexKartyBota = i;
				cout << "    if wart > wart     ";
			}
			cout << "  if kol == kol    ";
		}
		cout << "1forek" << "    ";
	}
	cout <<endl<< indexKartyBota << " <-- index karty | wartosc ---->" << wartosc << endl;
	if (wartosc == 0 && indexKartyBota == 0) {
		for (int i = 0; i < 8; i++) {
			if (kartaBota[i].uzyta == false) {
				indexKartyBota = i;
				break;
			}
		}
	}
	cout << indexKartyBota << " <-- index karty2 | wartosc2 ---->" << wartosc << endl;
	kartaBota[indexKartyBota].uzyta = true;
	kartaBota[indexKartyBota].card.setPosition(sf::Vector2f(x, 400.0f));
	pobierzAdresBot(kartaBota[indexKartyBota]);
	return kartaBota[indexKartyBota];
}

void Gra::pobierzAdresBot(Karta& kartaBot){
	if (tmp2 == 0) {
		poprzednia2 = &kartaBot;
		tmp2 = 1;
	}
	else {
		poprzednia3 = &kartaBot;
		tmp2 = 0;
	}
}	




void Gra::reset() {
	if (doResetuZostalo == 7 || czyReset == true) {
		cout << "RESET!" << endl;
		tmp = 0;
		przyznanyMusik = 0;
		przetasujKarty(talia);
		rozdanie(gracz1, gracz2, gracz3, talia, musik);
		pozycjaKart();
		doResetuZostalo = 0;
		if (czyReset == true) {
			pkt1 = 0;
			pkt2 = 0;
			pkt3 = 0;
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
		czyReset = false;
		czyMusik = true;
		musikCzyNie = true;
		wystawienie = true;
		poprzednia1 = nullptr; 
		poprzednia2 = nullptr;
		poprzednia3 = nullptr;
		gracz1[7].card.setPosition(sf::Vector2f(-100.0f, -100.0f));
		gracz2[7].card.setPosition(sf::Vector2f(-100.0f, -100.0f));
		gracz3[7].card.setPosition(sf::Vector2f(-100.0f, -100.0f));
		wybierzKarte.setPosition(sf::Vector2f(450.0f, 450.0f));
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
					if (doResetuZostalo != 8)
						wLewo();
					break;
				case sf::Keyboard::Right:
					if (doResetuZostalo != 8)
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
							wystawKarte(gracz1[ktoraKarta]);
							pobieranieWartosci(gracz1[ktoraKarta]);
							kartyUzyte[ktoraKarta] = ktoraKarta;
							kartaDoWyrzucenia = ktoraKarta;
							if (doResetuZostalo != 8)
								wPrawo();
							wystawienie = false;
						}
						else {
							usunKarty();
							wygrywaG1 = 0;
							wygrywaG2 = 0;
							wygrywaG3 = 0;
							wystawienie = true;
							doResetuZostalo++;
						}
					}
					break;
				case sf::Keyboard::R:
					czyReset = true;
					reset();
					break;
				case sf::Keyboard::Down:
					//wDol();
					break;
				case sf::Keyboard::Up:
					//wGore();
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
		if (czyMusik) {
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
		|-> jezeli jest wiecej meldunkow to mozemy wybrac ktory meldunek chcemy
		|-> zameldowac mozna po kliknieciu M , jezeli ilosc meldunkow jest > 0 to funkcja sie wykona
	
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
	
*/ 

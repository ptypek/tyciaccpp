#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "naglowek.h"
#include "Menu.h"
using namespace std;

class Gra{
private:
	Karta talia[24], gracz1[7], gracz2[7], gracz3[7], musik[3];
	sf::RenderWindow window;
	sf::RectangleShape porownanie;
	sf::Texture texture;
	sf::Texture texturePlansza;
	
	sf::Sprite plansza;
	
	sf::Font czcionka;
	sf::Text punkty1, punkty2, punkty3, wygrana, przegrana;
	int pkt1{}, pkt2{}, pkt3{};

	sf::Vector2i pozycjaMyszy;
	bool przemieszczanieKarty;
	float dx, dy, x, y;

	Menu menu;
	int przejscieDo;

	void przebieg();
	void okno();
	
public:
	Gra();
	void rozpocznij();
};

Gra::Gra()
	:window(sf::VideoMode(1200, 800), "Gra w tysiaca", sf::Style::Close | sf::Style::Resize),
	menu(window.getSize().x,window.getSize().y),
	pkt1(0),
	pkt2(0),
	pkt3(0),
	dx(0),
	dy(0),
	x(50.0f),
	y(50.0f),
	przemieszczanieKarty(false),
	przejscieDo(0)
{
	texture.loadFromFile("talia.png");
	texturePlansza.loadFromFile("plansza.png");
	
	plansza.setTexture(texturePlansza);

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
	wygrana.setString("Przegrana!");
	wczytajKarty(talia);
	przypiszObraz(talia, &texture);
	pozycjaMyszy = sf::Mouse::getPosition();
}
//(gracz1[i].card.getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)

void Gra::przebieg() {
	sf::Event zdarzenie;
	while (window.pollEvent(zdarzenie)) {
		switch (zdarzenie.type) {
		case sf::Event::Closed:
			cout << "Zamykam";
			window.close();
			break;
		case sf::Event::KeyReleased:
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
					break;
				case 1:
					przejscieDo = 2;
					break;
				case 2:
					window.close();
					break;
				}
			}
			break;
		default:
			break;
		}
	}
}

void Gra::okno() {
	window.clear();
	if (przejscieDo==0) {
		menu.draw(window);
	}
	else if (przejscieDo == 1) {
		window.draw(plansza);
	}
	else if (przejscieDo == 2) {
		//do pomocy
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



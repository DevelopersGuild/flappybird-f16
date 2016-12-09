#ifndef CREDITSCREEN_H
#define CREDITSCREEN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourcePath.h"

/****************************************
this function will draw the credit screen with developers
and return to the game if user press space bar
*****************************************/
void drawCredit(sf::RenderWindow &window, bool &isCredit)
{
	sf::Font font;
	if (!font.loadFromFile(resourcePath() + "assets/MostlyMono.ttf"))
		std::cout << "Error: File not found" << std::endl;

	sf::Text title;
	sf::Text names;
	sf::Text prompt;

	sf::Texture ftexture;
	ftexture.loadFromFile(resourcePath() + "assets/fbirdsprite.png");

	//Sprite for the background
	sf::Sprite background(ftexture);
	background.setTextureRect(sf::IntRect(0, 0, 291, 513));

	//This is where the title appears
	title.setFont(font);
	title.setString("Developers: ");
	title.setCharacterSize(45);
	title.setFillColor(sf::Color(135, 8, 75));
	title.setStyle(sf::Text::Bold);
	title.setPosition(20, 50);

	//names of developers
	names.setFont(font);
	names.setString("Ye-Eun Myung\nJian Li\nSara");
	names.setCharacterSize(36);
	names.setFillColor(sf::Color(135, 8, 75));
	names.setStyle(sf::Text::Bold);
	names.setPosition(40, 175);

	//ask if user want to replay
	prompt.setFont(font);
	prompt.setString("Press Space to replay");
	prompt.setCharacterSize(24);
	prompt.setFillColor(sf::Color(135, 8, 75));
	prompt.setStyle(sf::Text::Bold);
	prompt.setPosition(15, 425);

	//Draws background first and then the texts
	window.clear();
	window.draw(background);
	window.draw(title);
	window.draw(names);
	window.draw(prompt);
	window.display();

	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			isCredit = false;
	}
}

#endif
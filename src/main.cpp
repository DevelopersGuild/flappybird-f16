//SFML C++ Simple Game - Flappy Bird Clone
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream> //for convert string
#include "ResourcePath.h"
#include "Overlap.h"
#include "CreditScreen.h"

struct Block
{
	sf::Sprite tBlock;
	sf::Sprite bBlock;
	bool isPassed;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(288, 513), "Flappy Bird");
	window.setVerticalSyncEnabled(true);

	//Sprite Sheet has all the sprites we need
	sf::Texture ftexture;
	ftexture.loadFromFile(resourcePath() + "assets/fbirdsprite.png");
	
	//Sprite for the background
	sf::Sprite background(ftexture);
	background.setTextureRect(sf::IntRect(0, 0, 291, 513));
	//Sprite for the ground at the bottom of the window
	sf::Sprite box(ftexture);
	box.setTextureRect(sf::IntRect(292, 0, 307, 111));
	box.setPosition(0, 402);

	//Sprite for the Bird (aka main character)
	sf::Sprite bird(ftexture);
	bird.setTextureRect(sf::IntRect(527, 127, 35, 25));
	bird.setPosition(20, 190);
	
	//this is the only font we use in the game
	sf::Font fonttexture;
	fonttexture.loadFromFile(resourcePath() + "assets/MostlyMono.ttf");
	//Text for the prompt
	sf::Text score("point: ", fonttexture, 50);
	score.setPosition(20, 10);
	score.setFillColor(sf::Color(135, 8, 75)); //setColor is deprecated
	score.setStyle(sf::Text::Bold);
	//Text for the number of points
	sf::String numPoint;
	sf::Text point(numPoint, fonttexture, 50);
	point.setPosition(155, 10);
	point.setFillColor(sf::Color(135, 8, 75));
	//Text for gameover
	sf::Text overText("GAME OVER", fonttexture, 50);
	overText.setFillColor(sf::Color(135, 8, 75));
	overText.setPosition(30, 100);
	overText.setStyle(sf::Text::Bold);
	//Text for gameover prompt
	sf::Text againText(" Press C to View Credit\nPress Space to Play Again", fonttexture, 20);
	againText.setFillColor(sf::Color(135, 8, 75));
	againText.setStyle(sf::Text::Bold);
	againText.setPosition(25, 425);

	//Sprite for the top block
	sf::Sprite topS(ftexture);
	topS.setTextureRect(sf::IntRect(600, 0, 58,270));
	topS.setOrigin(topS.getGlobalBounds().width, topS.getGlobalBounds().height);
	//Sprite for the bottom block
	sf::Sprite bottomS(ftexture);
	bottomS.setTextureRect(sf::IntRect(658, 0, 58, 270));
	bottomS.setOrigin(bottomS.getGlobalBounds().width, 0);
	//Create a struct and assign corresponding sprite
	Block blocks;
	blocks.tBlock = topS;
	blocks.bBlock = bottomS;
	
	int numScore = 0; //counter for the number of point/score

	//This creates the vector of blocks. (in this case there are 4)  number can be changed any time
	std::vector<Block> vectorBlock;

	srand((unsigned int)time(NULL));

	//create 4 pairs of blocks
	for (int i = 0; i < 4; ++i) //4 pairs of pipe
	{
		vectorBlock.push_back(blocks);
		vectorBlock[i].bBlock.setPosition((float)((i+1) * 300), (float)(rand() % (351-150) + 150));
		float number = vectorBlock[i].bBlock.getPosition().y - 100;//the space between two pipes
		vectorBlock[i].tBlock.setPosition((float)((i + 1) * 300), number);
		vectorBlock[i].isPassed = false;
	}

	bool gamestart = false; //indicates gamestart
	bool gameover = false; //indicates game over
	bool flyAnimation = false; //for flying animation (flapping)
	bool endAnimation = true; // for end animation before game over prompt
	bool moving = true; //blocks moving is true
	bool isCredit = false; //for credit screen
	int counter = 0; //counter for flapping animation

	//for moving the bird sprite
	sf::Vector2f velocity;
	velocity.x = 0.0;
	velocity.y = 0.0;

	//clock for gravity
	sf::Clock clock1;

	//main game loop
	while (window.isOpen())
	{
		sf::Time time1 = clock1.getElapsedTime();
		float sec = time1.asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			//close when close button is pressed
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//if gamestart is false then change to true if space is pressed
		if (!gamestart)
		{
			//press space bar to start the game
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				gamestart = true;
			}
			numScore = 0;
		}

		//if game has started and game is not over
		if (gamestart && !gameover)
		{
			//Pressing Space bar jumps the Bird Sprite and if it is not pressed, then the gravitfy works
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				velocity.y = -5.5;
				time1 = clock1.restart();
				flyAnimation = true;
			}
			else
			{
				float gravity = (float)(0.5 * 9.8 * pow(sec, 2));
				velocity.y = gravity;
			}



			//checks for the boundary of y
			if (bird.getPosition().y < 0)//because the origin of the bird sprite is (0,0)
			{
				bird.setPosition(bird.getPosition().x, 0); //reset the bird position to y = 0
				velocity.y = 0.0; //stop moving
			}
			else if ((bird.getPosition().y + bird.getGlobalBounds().height) > (window.getSize().y - 111))
			{
				//if the bird reaches ground than game over
				bird.setPosition(bird.getPosition().x, 402 - bird.getGlobalBounds().height);
				moving = false;
				gameover = true;//game over
			}

			//move bird spirte according to the velocity. // velocity.x should be = 0
			bird.move(velocity); 

			//to move the pair pipes from right to left
			for (unsigned int i = 0; i < vectorBlock.size(); ++i)
			{
				if (moving)
				{
					vectorBlock[i].bBlock.move(sf::Vector2f(-3.0, 0));
					vectorBlock[i].tBlock.move(sf::Vector2f(-3.0, 0));
				}
			}
			
			for (unsigned int i = 0; i < vectorBlock.size(); i++)
			{
				//set new position for the blocks that got out of the window
				if (vectorBlock[i].bBlock.getPosition().x < 0)
				{
					vectorBlock[i].bBlock.setPosition((float)(1200), (float)(rand() % (351 - 150) + 150));
					float other = vectorBlock[i].bBlock.getPosition().y - 100;//the space between two pipes
					vectorBlock[i].tBlock.setPosition((float)(1200), other);
					vectorBlock[i].isPassed = false;
				}
				//if the bird overlap with any block then stop moving the block and game is over
				if ((overlap(bird, vectorBlock[i].bBlock)) || (overlap(bird, vectorBlock[i].tBlock)))
				{
					moving = false;//stops moving the blocks
					gameover = true;
				}
				//if the bird passes between blocks then give onen point
				if ((bird.getPosition().y < vectorBlock[i].bBlock.getPosition().y) && (bird.getPosition().y > vectorBlock[i].tBlock.getPosition().y)) //if between blocks
				{
					if ((bird.getPosition().x > vectorBlock[i].bBlock.getPosition().x) && (bird.getPosition().x < (vectorBlock[i].bBlock.getPosition().x + vectorBlock[i].bBlock.getGlobalBounds().width)))
					{
						if (!vectorBlock[i].isPassed)
						{
							vectorBlock[i].isPassed = true;
							numScore++; //earn point
						}
					}
				}
			}//end for loop
		}

		//convert numScore to string to display
		std::stringstream convertToString;
		convertToString << numScore;
		point.setString(convertToString.str());

		window.clear();
		window.draw(background);
		//draw blocks
		for (unsigned int k = 0; k < vectorBlock.size(); k++)
		{
			window.draw(vectorBlock[k].bBlock);
			window.draw(vectorBlock[k].tBlock);
		}


		//this kinda works for flying animation
		while(flyAnimation)
		{
			if (counter == 0)
			{
				counter++;
				bird.setTextureRect(sf::IntRect(527, 127, 35, 25));
				break;
			}
			if (counter == 1)
			{
				counter++;
				bird.setTextureRect(sf::IntRect(527, 179, 35, 25));
				break;
			}
			if (counter == 2)
			{
				counter++;
				bird.setTextureRect(sf::IntRect(445, 247, 39, 25));
				break;
			}
			if (counter >= 3)
			{
				counter = 0;
				flyAnimation = false;
			}
		}//end while loop
		
		window.draw(bird);
		window.draw(box);
		window.draw(score);
		window.draw(point);
		
		if (gameover && endAnimation)
		{
			float gravity1 =(float)(0.5 * 9.8 * pow(sec, 2));
			bird.move(0, gravity1);
			if (gameover && (bird.getPosition().y > (window.getSize().y - 111)))
			{
				bird.setPosition(bird.getPosition().x, 402 - bird.getGlobalBounds().height);
				endAnimation = false;
			}
		}

		//after the gameover asks user to play again.
		if(gameover && !endAnimation)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				isCredit = true;
			window.draw(overText);
			window.draw(againText);
			while(isCredit)
			{
				drawCredit(window, isCredit);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				gameover = false;
				gamestart = false;
				moving = true;
				endAnimation = true;
				bird.setPosition(20, 190);
				for (int i = 0; i < 4; ++i)
				{
					vectorBlock[i].bBlock.setPosition((float)((i + 1) * 300), (float)(rand() % (351 - 150) + 150));
					float number = vectorBlock[i].bBlock.getPosition().y - 100;//the space between two pipes
					vectorBlock[i].tBlock.setPosition((float)((i + 1) * 300), number);
					vectorBlock[i].isPassed = false;
				}
			}
		}
		window.display();
	}

	return 0;
}

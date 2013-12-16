#include "breakout.h"
#include "engine.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <cerrno>
#include <sstream>
#include <unistd.h>

using namespace std;
using namespace rapidxml;

enum MODE { GAME, MENU, TRANSITION };

bool isClickInRect(sf::Vector2i click, sf::Rect<int> box){
	if(
			click.x >= box.left && click.x < box.left+box.width && 
			click.y >= box.top && click.y < box.top+box.height)
		return true;
	return false;
}

void breakoutGame(){
	MODE mode=MENU;
	int tmpScore=0,tmpBalls=-1;
	sf::Font font;
	font.loadFromFile("fnt/Arcade.ttf");
	
	sf::RenderWindow window(sf::VideoMode(600, 400), "Breakout!");
	
	Game game;
	sf::Text ballsText("Score: 0", font);
	sf::Text scoreText("Score: 0", font);
	sf::Text playText("Play!", font);
	sf::Text loadText("Load Game", font);

	sf::Clock clock;
	float tmpTime,aggTime;
	float totalTime=0;
	int loopCount=0;
	stringstream scoreStr,ballsStr;
	sf::Rect<int> start;
	sf::Rect<int> load;

	//Define the buttons as rects for easy access later on
	start.left=100; start.top=100;
	start.width=120; start.height=50;

	load.left=100; load.top=160;
	load.width=190; load.height=50;

	playText.setPosition(start.left+10,start.top+10);
	playText.setCharacterSize(30);
	playText.setStyle(sf::Text::Bold);
	playText.setColor(sf::Color::Red);
	
	loadText.setPosition(load.left+10,load.top+10);
	loadText.setCharacterSize(30);
	loadText.setStyle(sf::Text::Bold);
	loadText.setColor(sf::Color::Red);

	sf::RectangleShape startButton(sf::Vector2f(start.width, start.height));
	startButton.setPosition(start.left,start.top);
	
	sf::RectangleShape loadButton(sf::Vector2f(load.width, load.height));
	loadButton.setPosition(load.left,load.top);
	
	sf::RectangleShape loadingBG(sf::Vector2f(600,400));
	sf::Texture loadingTex;
	loadingTex.loadFromFile("img/loading.jpg");
	loadingBG.setTexture(&loadingTex,false);
	loadingBG.setPosition(0,0);

	string file="";
	
	while (window.isOpen())
	{
		if(mode==MENU){
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					sf::Vector2i pos=sf::Mouse::getPosition(window);
					if(isClickInRect(pos,start)){
						file="Default.bgm";
						mode=TRANSITION;
					}
					if(isClickInRect(pos,load)){
						system("python file.py");
						ifstream in;
						in.open("~LoadFilePath");
						while (!in.eof()) {
							in >> file;
						}
						in.close();
						remove("~LoadFilePath");
						mode=TRANSITION;
					}
				}
			}
			window.clear();
			window.draw(startButton);
			window.draw(loadButton);
			window.draw(loadText);
			window.draw(playText);
			window.display();
		}
		if(mode==TRANSITION){
			window.clear();
			window.draw(loadingBG);
			window.display();
			sleep(1.5);
			
			game.loadGame(file);
			scoreText.setCharacterSize(30);
			scoreText.setStyle(sf::Text::Bold);
			scoreText.setColor(game.getTextColor());
			scoreText.setPosition(10,530);

			ballsText.setCharacterSize(30);
			ballsText.setStyle(sf::Text::Bold);
			ballsText.setColor(game.getTextColor());
			ballsText.setPosition(10,550);

			window.create(sf::VideoMode(game.getWidth(), game.getHeight()), "Breakout!");
			mode=GAME;
			
		}
		if(mode==GAME){
			//Update score text
			if(game.getScore()!=tmpScore){
				scoreStr.str(string());
				scoreStr << "Score: " << game.getScore();
				scoreText.setString(scoreStr.str());
				tmpScore=game.getScore();
			}

			//Update ball text
			if(game.getBalls()!=tmpBalls){
				ballsStr.str(string());
				ballsStr << "Balls: " << game.getBalls();
				ballsText.setString(ballsStr.str());
				tmpBalls=game.getBalls();
			}
			tmpTime=clock.restart().asSeconds();
			aggTime += tmpTime;
			//Get events
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (game.getStart() && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					sf::Vector2i pos=sf::Mouse::getPosition(window);
					game.startFromClick(pos.x,pos.y);
					game.startGame();
				}
			}

			//Draw if it has been more than 1/60 of a second
			if(aggTime > 0.03333){
				aggTime=0;
				window.clear();
				game.drawBG(&window);
				game.drawBlocks(&window);
				game.drawPaddle(&window);
				game.drawBall(&window);
				window.draw(scoreText);
				window.draw(ballsText);
				window.display();
			}
			totalTime+=tmpTime;
			loopCount++;

			//Update
			game.updatePaddle(window);
			game.updateBall();
			game.checkBlocks();
			game.moveBall(tmpTime);
			
			//If the blocks are all gone jump to the next level
			if(!game.blocksLeft()){
				game.stopGame();
				if(!game.nextLevel()){
					mode=MENU;
				}
			}
			else if(game.getBalls()<0){
				mode=MENU;
			}
		}
	}
}


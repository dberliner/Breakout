#include "breakout.h"
#include "engine.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "QUnit.hpp"
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <cerrno>

using namespace std;
using namespace rapidxml;
using namespace QUnit;
void breakoutGame(){
	QUnit::UnitTest qunit(std::cerr, QUnit::normal);
	Game game;
	game.loadGame("TestGame1.xml");
	vector<Block*>* blocks=game.getBlocks();
	Block block=*blocks->back();

	//see if the blocks got created correctly
	QUNIT_IS_EQUAL(blocks->size(),1);
	QUNIT_IS_EQUAL(block.x(),50);
	QUNIT_IS_EQUAL(block.y(),10);
	QUNIT_IS_EQUAL(block.w(),80);
	QUNIT_IS_EQUAL(block.h(),17);

	//See if the window attributes are correct
	QUNIT_IS_EQUAL(game.getWidth(),800);
	QUNIT_IS_EQUAL(game.getHeight(),600);
	QUNIT_IS_TRUE(game.getTextColor()==sf::Color(255,255,255));

	//See if the paddle is the correct width
	QUNIT_IS_EQUAL(game.getPaddle()->w(),150);

	game.loadGame("TestGame2.xml");
	game.getBall()->changeSpeed(300);	
	game.startGame();
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::Clock clock;
	float tmpTime;
	float totTime=0;
	int stage=0;
	cout << "-------Running Realtime Tests----------" << endl;
	while (window.isOpen() && totTime < 11)
	{

		//Check the side walls in stage 2
		if(stage==0 && totTime == 0){
			stage++;
			game.getBall()->startFromClick(floor(game.getBall()->x()),floor(game.getBall()->y())+100);
		}
		if(stage==1 && totTime > 4.01){
			stage++;
			game.getBall()->startFromClick(game.getBall()->x()+100,game.getBall()->y());
		}
		if(stage==2 && totTime > 8){
			stage++;
			game.getBall()->startFromClick(floor(game.getBall()->x()),floor(game.getBall()->y())-100);
		}

		tmpTime=clock.restart().asSeconds();
		totTime += tmpTime;
		sf::Event event;
		while (window.pollEvent(event))
		{
		    if (event.type == sf::Event::Closed)
			window.close();
		}
		window.clear();
		game.drawBlocks(&window);
		game.drawPaddle(&window);
		game.drawBall(&window);
		window.display();

		game.updateBall();
		game.checkBlocks();
		//Make sure the ball never goes below the paddle or outside the screen
		QUNIT_IS_TRUE(game.getBall()->y()<game.getHeight()-game.getBall()->h());
		QUNIT_IS_TRUE(game.getBall()->y()>=0);
		QUNIT_IS_TRUE(game.getBall()->x()<=game.getWidth()-game.getBall()->w());
		QUNIT_IS_TRUE(game.getBall()->x()>=0);
		//game.updatePaddle(window);
		game.moveBall(tmpTime);
		
	}
	QUNIT_IS_EQUAL(game.getBalls(),2);
	QUNIT_IS_EQUAL(game.getScore(),200);
	QUNIT_IS_EQUAL(blocks->size(),0);
	cout << "----------Ending Runtime Tests-----------" << endl;

}


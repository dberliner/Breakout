#include <time.h>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "breakout.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
using namespace std;
using namespace rapidxml;

Block::Block(){
	score=100;
}

/* -----------------
Function name: Ball
Purpose: Initializes the ball, also seeds random
Paramaters:	int sp The speed in pix/sec of the ball
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
Ball::Ball(int sp){
	speed=sp;
	srand(time(NULL));
	xvel=0;
	yvel=0;
}
/* -----------------
Function name: Ball
Purpose: Initializes the ball with a default speed of 300
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/

Ball::Ball(){
	speed=700;
	srand(time(NULL));
	xvel=0;
	yvel=0;
}

/* -----------------
Function name: startFromClick
Purpose: Starts the ball in a direction indicated by a click of the mouse at the given speed
Paramaters:	
	int clickx The x position of the click
	int clicky The y position of the click
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Ball::startFromClick(int clickx, int clicky){
	double theta=atan((ypos-clicky)/(xpos-clickx));	
	//Floating point errors are noticable for directly vertical and horozintal

	//The cmath library works in a different qudrent making the 
	//value of theta incorrect when you click to the right
	if(theta >0){
		theta+=M_PI;
	}
	unitx=cos(theta);
	unity=sin(theta);
	changeVelocityAbsolute(unitx*speed,unity*speed);
}

/* -----------------
Function name: startRandom
Purpose: Randomizes the unit vector within allowable parameters
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Ball::startRandom(){
	//Get a random nuber between 10 and 80 degrees
	float deg=rand()%70+10;
	//Assign that offset to a random quadrent
	deg=deg+90*(rand()%4);

	//Convert to radians
	float pi=3.14159; //Close enough
	deg=deg*pi/180;

	//Change that degree to a unit vector
	unitx=cos(deg);
	unity=sin(deg);

	//Change the speed to the appropiate values
	changeVelocityAbsolute(unitx*speed,unity*speed);
}

/* -----------------
Function name: reverseX
Purpose: reverses the X direction
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Ball::reverseX(){
	unitx=-unitx;
	changeVelocityAbsolute(unitx*speed,unity*speed);
}

/* -----------------
Function name: reverseY
Purpose: Reverses the Y direciton of the ball
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core 
---------------------*/
void Ball::reverseY(){
	unity=-unity;
	changeVelocityAbsolute(unitx*speed,unity*speed);
}

/* -----------------
Function name: changeSpeed
Purpose: Changes the speed of the ball without altering the direciton
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core 
---------------------*/
void Ball::changeSpeed(int sp){
	speed=sp;
	changeVelocityAbsolute(unitx*speed,unity*speed);
}



/* -----------------
Function name: collideWall
Purpose: Checks whether the ball has collided with a wall
Paramaters:	
Returns: Whether the ball hit the bottom wall
Version: 1.0
Level: Game Core
---------------------*/
bool Ball::collideWall(int windowW, int windowH){
	if(xpos < 0 || xpos > windowW-width){
		reverseX();
		if(xpos <= 0)
			xpos=0;
		else
			xpos=windowW-width;
	}

	if(ypos < 0 || ypos > windowH-height){
		reverseY();
		if(ypos <= 0){
			ypos=0;
		}
		else{
			ypos=windowH-height;
			return true;
		}
	}
	return false;
}

float Ball::lineX(float m, float y, float b){
	return (y-b)/m; 
}

float Ball::lineY(float m, float x, float b){
	return m*x+b;
}

/* -----------------
Function name: CollideWith
Purpose: Handles collisions with an arbitrary rectangle, does not work with 
	rectangles that are otherwise orientated.
Paramaters:	
	ObjectProperties *other The other rectangle
	bool memory=false A helper so the Y direction only inverts once on double hits
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
bool Ball::collideWith(ObjectProperties *other,bool memory=false){
	float intersectX,intersectY;
	float centerx=xpos+width/2;
	float centery=ypos+height/2;
	float B=centery-(unity/unitx)*centerx;
	float M=unity/unitx;
	if(unity<0){
		//Find where the ball will be when it is as high as the block
		intersectX=lineX(M,other->y()+other->h(),B);
		if(intersectX >= other->x()-width/2 && intersectX < other->x()+other->w()+width/2){
			if( sqrt( pow(centerx-intersectX,2) + pow(centery-(other->y()+other->h()),2) <= width/2 ) ){
				reverseY();
				return true;
			}
		}
	}
	else{
		//Find where the ball will be when it is as high as the block
		intersectX=lineX(M,other->y(),centery-(unity/unitx)*centerx);
		if(intersectX >= other->x()-width/2 && intersectX < other->x()+other->w()+width/2)
			if( sqrt( pow(centerx-intersectX,2) + pow(centery-other->y(),2) ) <= width/2 ){
				reverseY();
				return true;
			}
	}

	if(unitx>0){ //If we are moving left
		//Center
		intersectY=lineY(M,other->x()+other->w(),B);
		//See if the line from the center 
		if(intersectY > other->y()-(width/2) && intersectY < other->y()+other->h()+(width/2)){
			if( sqrt( pow(centery-intersectY,2) + pow(centerx-(other->x()),2) ) <= width/2 ){
				reverseX();
				return true;
			}
		}
	}
	else{
		//Find the Y position that the line intersects with the left wall
		intersectY=lineY(M,other->x(),B);
		if(intersectY > other->y()-(width/2) && intersectY < other->y()+other->h()+(width/2)){
			if( sqrt( pow(centery-intersectY,2) + pow(centerx-(other->x()+other->w()),2) ) <= width/2 ){
				reverseX();
				return true;
			}
		}
	}
	return false;
	
}

/* -----------------
Function name: Ball::collideWithPaddle
Purpose: For collision with the paddle only
Paramaters:	ObjectProperties *other The paddle
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
bool Ball::collideWithPaddle(ObjectProperties *other){
	collideWith(other);	
	return false;
}

/* -----------------
Function name: startGame
Purpose: Starts the game
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::startGame(){
	isStart=false;
}
/* -----------------
Function name: sopGame
Purpose: Starts the game and sets the ball to the rest position
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::stopGame(){
	isStart=true;
	ball.changeVelocityAbsolute(0,0);
	ball.changePostitionAbsolute(300,475);
	paddle.changeXAbsolute(220);
}

/* -----------------
Function name: getWidth
Purpose: Returns the width of the window as specified by the game file
Paramaters:	
Returns: int width The width in pixels
Version: 1.0
Level: Game Core
---------------------*/
int Game::getWidth(){
	return w;
}

/* -----------------
Function name: getHeight
Purpose: Returns the height as specified by the game file
Paramaters:	
Returns: int height The height in pixels
Version: 1.0
Level: Game Core
---------------------*/
int Game::getHeight(){
	return h;
}

/* -----------------
Function name: Game
Purpose: Initializes the game, does not load a level
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
Game::Game(){
	score=0;
	ballsLeft=0;
	isStart=true;
	levelNumber=-1;
	ball.loadPartImg((string)"img/pieces.png", 0,0,0, 15,128,17,17);
	ball.changePostitionAbsolute(300,475);
	paddle.loadPartImg("img/pieces.png",0,0,0,384,176,463-383,191-175);
	paddle.changePostitionAbsolute(200,500);
	boop.loadFromFile("sound/boop.ogg");
	boopSound.setBuffer(boop);
}

/* -----------------
Function name: getSCore
Purpose: Returns the score that the user has accumulated in the given level
Paramaters:	
Returns: int score The score
Version: 1.0
Level: Game Core
---------------------*/
int Game::getScore(){
	return score;
}

/* -----------------
Function name: getTextColor
Purpose: Returns the sf::Color that the level file specifies for text
Paramaters:	
Returns: sf::Color The color that the file specified
Version: 1.0
Level: Game Core
---------------------*/
sf::Color Game::getTextColor(){
	return textColor;
}

/* -----------------
Function name: Game::nextLevel
Purpose: Loads the next level in the last file used
Paramaters:	none
Returns: none
Version: 1.0
Level: Game Core
Notes: Will draw from the most recent file used.
---------------------*/
bool Game::nextLevel(){
	return loadGame(currentFile,++levelNumber);
}

/* -----------------
Function name: Game::loadGame
Purpose: Loads a game from an XML file
Paramaters:	string file The XML file that contains the game definitions
Returns: True if level is found, otherwise false
Version: 1.0
Level: Game Core
Notes: Stores the file used and the level number for future use.
---------------------*/
bool Game::loadGame(string file, int levelNum /* =0 */){
	levelNumber=levelNum;
	currentFile=file;
	
	//Clear any existing blocks
	if(blocks.size()!=0){
		for(vector<Block*>::iterator it=blocks.begin(); it!=blocks.end();){
			it=blocks.erase(it);
		}
	}

	//Load the file
	rapidxml::file<> xmlFile(file.c_str()); // Default template is char
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<> *pRoot = doc.first_node();
	

	//Holder of the level
	xml_node<> *level=pRoot->first_node("level");
	for(int i=0;i<levelNum;i++){
		level=level->next_sibling();
	}
	if(!level)
		return false;

	//Set some single variabls for the level
	ballsLeft=atoi(level->first_node("balls")->value());
	w=atoi(level->first_node("width")->value());
	h=atoi(level->first_node("height")->value());
	paddle.w(atoi(level->first_node("paddleW")->value()));
	bg.loadImg(level->first_node("background")->value());

	//The background always displays at 0,0
	bg.changePostitionAbsolute(0,0);
	
	//Set the text color
	textColor=sf::Color(
			atoi(level->first_node("textg")->value()),
			atoi(level->first_node("textr")->value()),
			atoi(level->first_node("textb")->value())
			);
	
	//Set up the blocks for this level
	for(xml_node<>* n = level->first_node("blocks")->first_node("block"); n; n = n->next_sibling()){
		addBlock(
			n->first_attribute("img")->value(),
			atoi(n->first_attribute("x")->value()),
			atoi(n->first_attribute("y")->value()),
			atoi(n->first_attribute("w")->value()),
			atoi(n->first_attribute("h")->value()),
			atoi(n->first_attribute("xpos")->value()),
			atoi(n->first_attribute("ypos")->value())
		);
		
	}

	//Still hard coded on this version
	ball.changePostitionAbsolute(300,475);
	return true;
}

/* -----------------
Function name: Game::addBlock
Purpose: Adds an arbitrary block from an image file
Paramaters: 
	string img The image file to get the block from
	int x The starting X position on the image
	int y The starting Y position on the image
	int w The width of the image
	int h The height of the image
	int xpos The starting X position on the screen
	int ypos The starting Y position on the screen
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::addBlock(string img, int x, int y, int w, int h, int xpos, int ypos){
	Block *block=new Block;
	block->loadPartImg(
			img,
			0,0,0,
			x,y,w,h
	);
	block->changePostitionAbsolute(xpos,ypos);

	blocks.push_back(block);
}

/* -----------------
Function name: addScore
Purpose: Adds to the game score when a block is removed
Paramaters:	Block block The block that is being removed
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::addScore(Block *block){
	score += block->score;
}

/* -----------------
Function name: drawBall
Purpose: Draws the ball on to a screen
Paramaters:	sf::RenderWindow *window The window to draw on
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::drawBall(sf::RenderWindow *window){
	ball.draw(window);
}

/* -----------------
Function name: drawBG
Purpose: Draws the given background for the game
Paramaters:	sf::RenderWindow *window The window to draw to
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::drawBG(sf::RenderWindow *window){
	bg.draw(window);
}

/* -----------------
Function name: drawPaddle
Purpose: drawPaddle
Paramaters:	sf::RenderWindow *window The window to draw on
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::drawPaddle(sf::RenderWindow *window){
	paddle.draw(window);
}

/* -----------------
Function name: DrawBlocks
Purpose: Draws all active blocks on to the screen
Paramaters:	sf::RenderWindow *window The window to draw on
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::drawBlocks(sf::RenderWindow *window){
	vector<Block*>::iterator it=blocks.begin();
	while(it!=blocks.end()){
		(*it)->draw(window);
		it++;
	}
}

/* -----------------
Function name: checkBlocks
Purpose: Checks to see if the ball is overlapping any blocks, acts accordingly if a block is being hit
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::checkBlocks(){
	vector<Block*>::iterator it=blocks.begin();
	bool memory=false;
	while(it!=blocks.end()){
		if(ball.collideWith(*it,memory)==true){
			boopSound.play();
			memory=true;
			score += (*it)->getScore();
			it=blocks.erase(it);
		}else{
			it++;
		}
	}


}

/* -----------------
Function name: moveBall
Purpose: Moves the ball
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::moveBall(float time){
	ball.applyVelocity(time);
}

/* -----------------
Function name: updatePaddle
Purpose: Updates the paddle within the screen
Paramaters:	sf::RenderWindow &window The window to get the mouse position from
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::updatePaddle(sf::RenderWindow &window){
	if(isStart)
		return;
	sf::Vector2i lpos = sf::Mouse::getPosition(window);
	if(lpos.x < 0)
		lpos.x=0;
	else if(lpos.x>(w-paddle.w()))
		lpos.x=w-paddle.w();

	paddle.changeXAbsolute(lpos.x);
}

bool Game::blocksLeft(){
	return (blocks.begin() != blocks.end());
}

/* -----------------
Function name: getStart
Purpose: Returns if the game is started or not
Paramaters:	
Returns: bool isStart If the game has been started
Version: 1.0
Level: Game Core
---------------------*/
bool Game::getStart(){
	return isStart;
}

/* -----------------
Function name: getBalls
Purpose: Gets the number of balls the user has left
Paramaters:	
Returns: int ballsLeft The number of balls the user has left
Version: 1.0
Level: Game Core
---------------------*/
int Game::getBalls(){
	return ballsLeft;
}

/* -----------------
Function name: startFromClick
Purpose: Starts the ball from a given click
Paramaters:	
	int px The x position of the click
	int py The y position of the click
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::startFromClick(int px, int py){
	ball.startFromClick(px,py);
}
/* -----------------
Function name: Game::updateBall
Purpose: checks collision with the paddle and walls
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
void Game::updateBall(){
	ball.collideWithPaddle(&paddle);	
	if(ball.collideWall(800,600)){
		stopGame();
		ballsLeft--;
	}
}

/* -----------------
Function name: getBlocks
Purpose: Gives a pointer to the blocks, for unit testing only
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
vector<Block*>* Game::getBlocks(){
	return &blocks;
}

/* ----------------
Function name: Game::getBall
Purpose: Gives a pointer to the ball, for unit testing only
Paramaters:	
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
Ball *Game::getBall(){
	return &ball;
}

/* -----------------
Function name: Ball
Purpose: Gives a pointer to the paddle, for unit testing only
Paramaters: 
Returns: none
Version: 1.0
Level: Game Core
---------------------*/
Paddle *Game::getPaddle(){
	return &paddle;
}

/* -----------------
Function name: getScore
Purpose: Returns the score that the user has gotten
Paramaters:	
Returns: int score The score
Version: 1.0
Level: Game Core
---------------------*/
int Block::getScore(){
	return score;
}

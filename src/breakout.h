#include "engine.h"
#include <SFML/Audio.hpp>
#ifndef __BREAKOUT_H
#define __BREAKOUT_H 1

using namespace Engine;

class Block : public ObjectProperties{
	public:
	Block();
	int score;
	int getScore();
};

class Paddle : public ObjectProperties{

};

class Ball : public ObjectProperties{
	int speed;
	float unitx, unity;
	float lineX(float m, float y, float b);
	float lineY(float m, float x, float b);
	public:
	Ball(int sp),Ball();
	bool collideWithPaddle(ObjectProperties *other);
	bool collideWith(ObjectProperties *other,bool memory);
	void startFromClick(int clickx, int clicky);
	bool collideWall(int windowW, int windowH);
	void startRandom();
	void reverseX(),reverseY();
	void changeSpeed(int sp);
};

class Game{
	int levelNumber;
	string currentFile;
	int score;
	Paddle paddle;
	Ball ball;
	vector<Block*> blocks;
	int ballsLeft;
	bool isStart;
	sf::SoundBuffer boop;
	sf::Sound boopSound;
	int w,h;
	ObjectProperties bg;
	sf::Color textColor;
public:
	int getWidth(), getHeight();
	bool getStart();
	Ball *getBall();
	Paddle *getPaddle();
	vector<Block*>* getBlocks();
	bool loadGame(string file, int levelNum=0), nextLevel();
	Game();
	sf::Color getTextColor();
	void addScore(Block *);
	void checkBlocks();
	void drawBG(sf::RenderWindow*);
	void drawBlocks(sf::RenderWindow*), drawPaddle(sf::RenderWindow*), drawBall(sf::RenderWindow*);
	void addBlock(string img, int x, int y, int w, int h, int xpos, int ypos);
	void updatePaddle(sf::RenderWindow&);
	void moveBall(float ms),updateBall();
	void startFromClick(int px, int py);
	void startGame();
	int getScore();
	void stopGame();
	int getBalls();
	bool blocksLeft();
};

#endif

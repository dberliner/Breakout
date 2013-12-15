#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "engine.h"
using std::vector;
using std::string;
using std::pair;
using std::cout;
using std::endl;

/*---------------------
Function name: doesFileExist
Purpose: Detects if a given file can be accessed
Paramaters: string file
Returns: true if it has access, false if it does not
Version: 2.3
Level: Core 
NOTE: 
	This function can not discern access issues and the file not existing. 
	A false return simply means that the file can not be accessed and 
	from the program's perspective its all the same because it will cause 
	the same crash.
---------------------*/
bool Engine::doesFileExist(std::string file)
{
	std::fstream fHnd;
	fHnd.open(file.c_str(),std::fstream::in);
	return !fHnd.fail();
}
long double Engine::ObjectProperties::xv(){
	return xvel;
}
long double Engine::ObjectProperties::yv(){
	return yvel;
}
long double Engine::ObjectProperties::ltx(){
	return xpos-width;
}
long double Engine::ObjectProperties::lty(){
	return ypos-height;
}
long double Engine::ObjectProperties::x(){
	return xpos;
}

long double Engine::ObjectProperties::y(){
	return ypos;
}

long double Engine::ObjectProperties::h(){
	return height;
}

long double Engine::ObjectProperties::w(){
	return width;
}

void Engine::ObjectProperties::w(int w){
	long double coef=w;
	coef=coef/width;
	face.scale(coef,1);
	width=w;
}
void Engine::ObjectProperties::h(int h){
	long double coef=h/height;
	face.scale(1,coef);
	height=h;
}
void Engine::ObjectProperties::scale(long double s){
	face.scale(s,s);
}

void Engine::ObjectProperties::setEffectiveW(int w){
	width=w;
}

void Engine::ObjectProperties::setEffectiveH(int h){
	height=h;
}


/*---------------------
Function name: ObjectProperties
Purpose: Constructor for class ObjectProperties
Paramaters: None
Version: 1.0
Level: Core 
---------------------*/
Engine::ObjectProperties::ObjectProperties()
{
	xpos   = 0;
	ypos   = 0;
	width  = 0;
	height = 0;
}

/*---------------------
Function name: ObjectProperties::changeYRelitave
Purpose: Addes the Y position by the number specified
Paramaters: long double change (the value we will add)
Returns: Nothing.
Version: 1.0
Level: Core 
---------------------*/
void Engine::ObjectProperties::changeYRelitave(long double change)
{
	ObjectProperties::ypos += change;
	face.setPosition(xpos,ypos);
	return;
}

/*---------------------
Function name: ObjectProperties::changeYRelitave
Purpose: Addes the X position by the number specified
Paramaters: long double change (the value we will add)
Returns: Nothing.
Version: 1.0
Level: Core 
---------------------*/
void Engine::ObjectProperties::changeXRelitave(long double change)
{
	ObjectProperties::xpos += change;
	face.setPosition(xpos,ypos);
	return;
}

/*---------------------
Function name: ObjectProperties::changeYRelitave
Purpose: Addes the X and Y position by the number specified
Paramaters: long double X(the value we will add to X), float Y(The value we will add to y)
Returns: Nothing.
Version: 1.0
Level: Core 
---------------------*/
void Engine::ObjectProperties::changePositionRelitave(long double X,float Y)
{
	ObjectProperties::xpos += X;
	ObjectProperties::ypos += Y;
	face.setPosition(xpos,ypos);
	return;
}


/*---------------------
Function name: ObjectProperties::changeVelocityRelative
Purpose: Addes the X and Y velocity by the number specified
Paramaters: long double X(the value we will add to X), float Y(The value we will add to y)
Returns: Nothing.
Version: 1.0
Level: Core 
---------------------*/
void Engine::ObjectProperties::changeVelocityRelative(long double x, float y){
	xvel+=x;
	yvel+=y;
	face.setPosition(xpos,ypos);
}

/*---------------------
Function name: ObjectProperties::changeYAbsolute
Purpose: Changes the Y position to the specified location
Paramaters: long double change (the value we assign Y to)
Returns: Nothing.
Version: 1.0
Level: Core 
---------------------*/
void Engine::ObjectProperties::changeYAbsolute(long double Y)
{
	ObjectProperties::ypos = Y;
	face.setPosition(xpos,ypos);
	return;
}

/*---------------------
Function name: ObjectProperties::changeXAbsolute
Purpose: Changes the X position to the specified location
Paramaters: long double change (the value we assign X to)
Returns: Nothing.
Version: 1.0
Level: Core 
---------------------*/
void Engine::ObjectProperties::changeXAbsolute(long double X)
{
	ObjectProperties::xpos = X;
	face.setPosition(xpos,ypos);
	return;
}

/*---------------------
Function name: ObjectProperties::changePostitionAbsolute
Purpose: Changes the X abd Y position to the specified locations
Paramaters: long double X(the value we will change xpos to), float Y(The value we will change ypos to)
Returns: Nothing.
Version: 1.0
Level: Core 
NOTE: This object is redefined in snake
---------------------*/
void Engine::ObjectProperties::changePostitionAbsolute(long double X,float Y)
{
	ObjectProperties::xpos = X;
	ObjectProperties::ypos = Y;
	face.setPosition(xpos,ypos);
	return;
}

/*---------------------
Function name: ObjectProperties::changeVelocityRelative
Purpose: Addes the X and Y velocity by the number specified
Paramaters: long double X(the value we will add to X), float Y(The value we will add to y)
Returns: Nothing.
Version: 1.0
Level: Core 
---------------------*/
void Engine::ObjectProperties::changeVelocityAbsolute(long double x, float y){
	xvel=x;
	yvel=y;
	face.setPosition(xpos,ypos);
	return;
}

/*---------------------
Function name: ObjectProperties::loadFullImg
Purpose: Loads a complete image and sets the width and height to the total of the image
Paramaters: const char *path (the path of the BMP)
Returns: Returns the class' face
Version: 1.0
Level: Core 
NOTE: Works with the following image types:
		BMP, GIF, JPG, PNG, TIF, PNM, XPM, XCF, PCX, LBM 
Wrappers: LoadFullBMP(const char *path,int r,int g,int b)
---------------------*/
void Engine::ObjectProperties::loadImg(string path)
{
	if(Engine::doesFileExist(path))
	{
		img.loadFromFile(path);
		face.setTexture(img);
		
		sf::Vector2u tmpSize;

		tmpSize = img.getSize();
		width = tmpSize.x;
		height = tmpSize.y;

		drawStartX = 0;
		drawStartY = 0;
		return;
	}
}

/*---------------------
Function name: ObjectProperties::LoadFullBMP
Purpose: Loads a complete BMP with ObjectProperties::LoadFullBMP(const char *path) and adds RGB transparacy
Paramaters: const char *path (the path of the BMP) int r, int g, int b (the R G and B transparacy values)
Returns: The objects face.
Version: 1.0
Level: Core 
NOTE: None
---------------------*/
void Engine::ObjectProperties::loadImg(string path,int r,int g,int b)
{
	if(Engine::doesFileExist(path))
	{
		sf::Image tmp;
		tmp.loadFromFile(path);
		tmp.createMaskFromColor(sf::Color(r,g,b));
		img.loadFromImage(tmp);
		face.setTexture(img);

		sf::Vector2u tmpSize;
		tmpSize = img.getSize();
		width = tmpSize.x;
		height = tmpSize.y;

		drawStartX = 0;
		drawStartY = 0;
		return;
	}
}

void Engine::ObjectProperties::loadPartImg(string path, int r, int g, int b, int sx, int sy, int dw, int dh){
	if(Engine::doesFileExist(path))
	{
		sf::Image tmp;
		tmp.loadFromFile(path);
		tmp.createMaskFromColor(sf::Color(r,g,b));
		img.loadFromImage(tmp,sf::IntRect(sx,sy,dw,dh));
		face.setTexture(img);

		width = dw;
		height = dh;

		drawStartX = 0;
		drawStartY = 0;
		return;
	}
}


/*---------------------
Function name: ObjectProperties::placeOnMap
Purpose: Places an object at a given point on the map
Paramaters: long double xp, float yp (the x and y points to place the item on)
Returns: Nothing
Version: 1.0
Level: Core 
NOTE: Should probably be removed
---------------------*/
void Engine::ObjectProperties::placeOnMap(long double xp, float yp)
{
	ObjectProperties::xpos = xp;
	ObjectProperties::ypos = yp;
	
	face.setPosition(xpos,ypos);
	return;
}

/*---------------------
Function name: ObjectProperties::Draw
Purpose: Draws the object's face on the screen
Paramaters: SDL_Surface *screen (the screen to draw it on)
Returns: Nothing
Version: 1.0
Level: Core 
NOTE: 
---------------------*/
void Engine::ObjectProperties::draw(sf::RenderWindow* const screen)
{
	screen->draw(face);
	return;
}

/*---------------------
Function name: ObjectProperties::Redefine
Purpose: Redefines the size data of the image
Paramaters: int startx, int starty, int w, int h (the start x,y of the image, the width and height of the image)
Returns: Nothing
Version: 1.0
Level: Core 
NOTE: 
---------------------*/
void Engine::ObjectProperties::redefine(int startx, int starty, int w, int h)
{
	ObjectProperties::drawStartX = startx;
	ObjectProperties::drawStartY = starty;
	ObjectProperties::width  = w;
	ObjectProperties::height = h;
	return;
}

/*---------------------
Function name: applyVelocity
Purpose: Applies the velocity variables in the object
Paramaters:	
	long double time
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::ObjectProperties::applyVelocity(long double time)
{
	ObjectProperties::changePositionRelitave(xvel*time,yvel*time);
	return;
}

/*---------------------
Function name: Window::Window
Purpose: Readies the window
Paramaters: 
    Width of screen (in px)
    Height of screen (in px)
    Bits Per Pixel (usually 32)
    Additional Flags
Returns: Nothing
Version: 1.0
Level: Core 
NOTE: 
---------------------*/
Engine::Window::Window(int w, int h, int bpp, string title)
{
    Window::w=w;
    Window::h=h;
	Window::title=title;
	screen=new sf::RenderWindow(sf::VideoMode(w,h),title);

}
Engine::Window::~Window(){
	delete screen;
}
Engine::Window::Window(const Window &other){
	Window::h=other.h;
	Window::w=other.w;
	Window::screen=new sf::RenderWindow(sf::VideoMode(other.w,other.h),other.title);
}
void Engine::Sprite::forceSequence(string sequenceID)
{
	forceSequence(sequenceID,active->name);
}
void Engine::Sprite::forceSequence(string sequenceID, string after)
{
	if(!isForced){
		afterForced=after;
		setActiveSequence(sequenceID);
		isForced=true;
	}
}
/*---------------------
Function name: changeObject
Purpose: Changes the position and dimensions of the sprite
Paramaters:	
	SpriteCoords pCoords
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::changeObject(SpriteCoords pCoords)
{
	Sprite::redefine(pCoords.x,pCoords.y,pCoords.w,pCoords.h);
	return;
}


/*---------------------
Function name: addSequence
Purpose: Adds a sequence to the library
Paramaters:	
	string reference, 
	bool loop
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::addSequence(string reference, long double delay, bool loop)
{
	SpriteSequence tmp;
	tmp.name=reference;
	tmp.loop=loop;
	tmp.coef=1;
	tmp.delay=delay;
	Sprite::sequences.push_back(tmp);
	return;
}

void Engine::Sprite::setIsomatricScale(long double scale, int height){
	isometricScale=scale;
	isometricScaling=true;
	mapHeight=height;
}

/*---------------------
Function name: getSequence
Purpose: Points paramater pIt to the referenced sprite
Paramaters:	
	string sequenceID,
	vector<Engine::SpriteSequence>::iterator &pIt)
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::getSequence(string sequenceID, vector<Engine::SpriteSequence>::iterator &pIt)
{
	//Start by pointing at the beginning in case there is no match
	pIt=Sprite::sequences.begin();

	//Go though every sequence and find one with the given name
	for(pIt = Sprite::sequences.begin(); 
        pIt != Sprite::sequences.end();
        pIt++)
	{
		if((*pIt).name==sequenceID)
		{
			return;
		}
	}
	return;
}

/*---------------------
Function name: applyVelocity
Purpose: Applies the velocity variables in the object
Paramaters:	
	long double time
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::applyVelocity(long double time)
{
	ObjectProperties::changePositionRelitave(xvel*time,yvel*time);

	if(isometricScaling){
		long double scale=isometricScale+(ypos/mapHeight)*(1-isometricScale);
		face.setScale(scale,scale);
	}
	return;
}


/*---------------------
Function name: deleteSequence
Purpose: Removes a sprite from the library
Paramaters:	
	string sequenceID
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::deleteSequence(string sequenceID)
{
	vector<Engine::SpriteSequence>::iterator tmpIt;
	Sprite::getSequence(sequenceID,tmpIt);
	Sprite::sequences.erase(tmpIt);
	return;
}

/*---------------------
Function name: addCoords
Purpose: Adds another set of coordinates to the end of the sprite sequence
Paramaters:	
	string sequenceID, 
	SpriteCoords coords
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::addCoords(string sequenceID, SpriteCoords coords)
{
	vector<Engine::SpriteSequence>::iterator tmpIt;
	Sprite::getSequence(sequenceID,tmpIt);
	(*tmpIt).coords.push_back(coords);
	return;
}


/*---------------------
Function name: removeFromSequence
Purpose: Removes a frame from a sprite sequence. 
Paramaters:	
	SpriteSequence seq The sequence to remove from
	int place The place of the frame to remove
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::removeFromSequence(SpriteSequence &seq, int place)
{
	int i=0;
	vector<SpriteCoords>::iterator it;
	for(it=seq.coords.begin();(it!=seq.coords.end() && i++!=place);++it){
	}
	seq.coords.erase(it);
	return;
}

/*---------------------
Function name: setACtiveSequence
Purpose: Sets the sequence of motions this sprite should be using
Paramaters:	
	string sequenceID
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::setActiveSequence(string sequenceID)
{
	if(!isForced){
		//If we are running for the first time we can't make comparisons with an unset iterator
		if(hasSequenceBeenSet==false){
			hasSequenceBeenSet=true;
			Sprite::getSequence(sequenceID,Sprite::active);
			Sprite::frame=(*Sprite::active).coords.begin();
		}
		else
		{
			vector<Engine::SpriteSequence>::iterator tmpIt;
			Sprite::getSequence(sequenceID,tmpIt);
			//Only change the sequence if it is different
			if(tmpIt != Sprite::active){
				Sprite::active=tmpIt;
				Sprite::frame=(*Sprite::active).coords.begin();
			}
		}
	}
	return;
}

/*---------------------
Function name: addCoords
Purpose: Iterates the sprite to the next frame in the active sequence
Paramaters:	
	none
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::nextFrame(long double time)
{
	//Move our delay counter up
	delayMarker+=time;

	//If we are ready to actually change the sprite
	if(delayMarker >= active->delay){
		//Iterate the frame by the by the amount we should
		Sprite::frame += (*Sprite::active).coef;
		//If we got to the end we need to decide if we are going in reverse or if we are starting over
		if((Sprite::frame==(*Sprite::active).coords.end() && (*Sprite::active).coef==1) || 
				(Sprite::frame==(*Sprite::active).coords.begin() && (*Sprite::active).coef==-1))
		{
			//If we are looping
			if((*Sprite::active).loop==false){
			
				if(isForced){
					isForced=false;
					setActiveSequence(afterForced);
					afterForced="";
				}
				Sprite::frame=(*Sprite::active).coords.begin();
			}
			//Otherwise reverse
			else
			{
				(*Sprite::active).coef *= -1;
				Sprite::nextFrame(0);
			}
		}
		delayMarker=0;
	}
	return;
}

//Reset the active frame to the first one in the sequence
void Engine::Sprite::resetFrame(){
	frame=active->coords.begin();
}
/*---------------------
Function name: draw
Purpose: Draws the face to the screen
Paramaters:	
	SDL_Surface *screen
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::draw(sf::RenderWindow* const screen)
{
	const sf::IntRect tmpRect(
		(*Sprite::frame).x,
		(*Sprite::frame).y,
		(*Sprite::frame).w,
		(*Sprite::frame).h
	);
	face.setTextureRect(tmpRect);

	screen->draw(face);
	return;
}

/*---------------------
Function name: getCoord
Purpose: Sets a SpriteCoord object based off of the other paramaters given. Used for automatically generating coords in uniformly rectangular sprites
Paramaters:	
	SpriteCoords &coords, 
	int width, 
	int height, 
	int imgWidth, 
	int imgHeight, 
	int initialOffset, 
	int tileNum
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::getCoord(SpriteCoords &coords, int width, int height, int imgWidth, int imgHeight, int initialOffset, int tileNum)
{
	int xGridPos=(tileNum+initialOffset) % (imgWidth/width);
	int yGridPos=(int)floor((long double)(tileNum+initialOffset)/(imgWidth/width));
	coords.w=width;
	coords.h=height;
	coords.x=xGridPos*width;
	coords.y=yGridPos*height;
	return;
}

/*---------------------
Function name: genSequence
Purpose: Generates a sprite, used for automatically generating large amounts of uniformly rectangular sprites.
Paramaters:	
	string sequenceID The string name of the sequence
	int width The width of each tile
	int height The height of each tile
	int imgWidth The spritesheet width
	int imgHeight The spritesheet height
	int initialOffset The border around the left and top parts of the spritsheet (often zero)
	int tilestart The numeric tile to start at within the sheet
	int tileCount How many tiles to generate
	long double delay The delay between image switches in seconds
	bool loop Whether we should loop or alternate back and forth

Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::genSequence(string sequenceID, int width, int height, int imgWidth, 
		int imgHeight, int initialOffset, int tileStart, int tileCount, long double delay, bool loop)
{
	SpriteCoords tmpCoords;
	Sprite::addSequence(sequenceID, delay,loop);
	for(int i=(tileStart); i<(tileStart+tileCount); i++)
	{
		getCoord(tmpCoords,width,height,imgWidth,imgHeight,initialOffset,i);
		Sprite::addCoords(sequenceID,tmpCoords);
	}
	Sprite::width=width;
	Sprite::height=height;
	return;
}


/*---------------------
Function name: loadImg
Purpose: Loads an image to a texture. 
Paramaters:	
	string path The path relative to the executible of the image
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::loadImg(string path)
{
	if(Engine::doesFileExist(path))
	{
		img.loadFromFile(path);
		face.setTexture(img);
		
		sf::Vector2u tmpSize;

		tmpSize = img.getSize();

		drawStartX = 0;
		drawStartY = 0;
		return;
	}
}

/*---------------------
Function name: loadImg
Purpose: Loads an image and sets an rgb transparacy
Paramaters:	
	string path The ath relative to the executible of the image
	int r The red transparacy (0-255)
	int g The green transparacy (0-255) 
	int b The blue transparacy (0-255)
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::Sprite::loadImg(string path,int r,int g,int b)
{
	if(Engine::doesFileExist(path))
	{
		sf::Image tmp;
		tmp.loadFromFile(path);
		tmp.createMaskFromColor(sf::Color(r,g,b));
		img.loadFromImage(tmp);
		face.setTexture(img);

		sf::Vector2u tmpSize;
		tmpSize = img.getSize();

		drawStartX = 0;
		drawStartY = 0;
		return;
	}
}

/*---------------------
Function name: addTop
Purpose: Adds a new sprite frame to the top of the order
Paramaters:	
	ObjectProperties *spr The object to add
	string id The sprite to add the frame to
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::DrawingOrder::addTop(ObjectProperties *spr, string id){
	list.push_back(make_pair(spr,id));
}

/*---------------------
Function name: addBottom
Purpose: Adds a sprite to the drawing order at the bottom
Paramaters:	
	ObjectProperties spr The sprite to add
	string id The name of the sprite in the drawing order
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::DrawingOrder::addBottom(ObjectProperties *spr, string id){
	list.insert(list.begin(),make_pair(spr,id));
}



/*---------------------
Function name: moveOrder
Purpose: Moves sprite A over sprite B in the drawing order
Paramaters:	
	string a 
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::DrawingOrder::moveOver(string a, string b)
{
	//Holders for the two items
	vector<pair<ObjectProperties*,string>>::iterator ita;
	pair<ObjectProperties*,string> tmp;
	int count=0;

	//Find the two items
	for(vector<pair<ObjectProperties*,string>>::iterator i=list.begin();i!=list.end();++i)
	{
		if(i->second==a)
		{
			ita=i;
			count++;
			tmp=make_pair(i->first,i->second);
			i=list.erase(i);
			break;
		}
	}
	for(vector<pair<ObjectProperties*,string>>::iterator i=list.begin();i!=list.end();++i)
	{
		//Move A over B
		if(i->second==b && count > 0)
		{
			list.insert(++i,tmp);
			break;
		}
	}
}

/*---------------------
Function name: moveUnder
Purpose: Moves sprite a under sprite b in the drawing order
Paramaters:	
	string a The first sprite
	string b The second sprite
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::DrawingOrder::moveUnder(string a, string b)
{
	//Holders for the two items
	vector<pair<ObjectProperties*,string>>::iterator ita;
	pair<ObjectProperties*,string> tmp;
	int count=0;

	//Find the two items
	for(vector<pair<ObjectProperties*,string>>::iterator i=list.begin();i!=list.end();++i)
	{
		if(i->second==a)
		{
			ita=i;
			count++;
			tmp=make_pair(i->first,i->second);
			i=list.erase(i);
			break;
		}
	}
	for(vector<pair<ObjectProperties*,string>>::iterator i=list.begin();i!=list.end();++i)
	{
		if(i->second==b && count > 0)
		{
			list.insert(i,tmp);
			break;
		}
	}
}

/*---------------------
Function name: moveToTop
Purpose: Moves a sprite to the top of the drawing order
Paramaters:	
	string id The ID of the sprite we want to move to the top
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::DrawingOrder::moveToTop(string id)
{
	if(list.end()->second==id)
		return;

	for(vector<pair<ObjectProperties*,string>>::iterator i=list.begin();i!=list.end();++i)
	{
		if(i->second==id){
			list.push_back(make_pair(i->first,i->second));
			list.erase(i);
		}
	}
}

/*---------------------
Function name: moveToBottom
Purpose: Moves a sprite to the bottom of the drawing order
Paramaters:	
	string id The ID of the sprite to move to the bottom
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::DrawingOrder::moveToBottom(string id){
	if(list.begin()->second==id)
		return;

	for(vector<pair<ObjectProperties*,string>>::iterator i=list.begin();i!=list.end();++i)
	{
		if(i->second==id){
			list.insert(list.begin(),make_pair(i->first,i->second));
			i=list.erase(i);
		}
	}
}

/*---------------------
Function name: draw
Purpose: Draws the drawing order to the given screen
Paramaters:	
	sf::RenderWindow *screen The screen to draw on.
Returns: none
Version: 1.3
Level: Core 
---------------------*/
void Engine::DrawingOrder::draw(sf::RenderWindow *screen){
	
	for(vector<pair<ObjectProperties*,string>>::iterator i=list.begin();i!=list.end();++i)
	{
			i->first->draw(screen);
	}
}

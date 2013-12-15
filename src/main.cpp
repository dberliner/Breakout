 #define _CRT_SECURE_NO_WARNINGS 1

#include <math.h>
#include <iostream>
#include <sstream>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "engine.h"
#include "breakout.h"
#include "breakout_script.h"
#include "test_script.h"
using namespace std;
using namespace Engine;
#define _DEBUG 1
int main(int argc, char ** argv)
{
	if(argc==2 && (string)argv[0]=="-t"){
		//testScript();
	}
	else{
		breakoutGame();		
	}
	return 0;
}

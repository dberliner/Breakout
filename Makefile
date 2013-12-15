CC = g++
INC = -I SFML-2.1/include
FLAGS = -Wall -Wextra -Werror -Wno-unused -g -Wno-reorder -std=c++0x
LIBS= -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
LIB = -L SFML-2.1/lib
DIR = ./src/
all: game-out

main.o: $(DIR)main.cpp $(DIR)engine.h $(DIR)breakout.h $(DIR)breakout_script.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $@


breakout.o: $(DIR)breakout.cpp $(DIR)breakout.h $(DIR)engine.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $@

breakout_script.o: $(DIR)breakout_script.cpp $(DIR)breakout.h $(DIR)engine.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $@

engine.o: $(DIR)engine.cpp $(DIR)engine.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $@

unitTest.o: $(DIR)unitTest.cpp $(DIR)breakout.h $(DIR)engine.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $@
	
game-out: main.o engine.o breakout.o breakout_script.o
	$(CC) $(LIB) $(FLAGS) $^ -o $@ $(LIBS)
	mv game-out bin
	rm *.o

clean:
	rm -rf *.o game-out test-out doc/html

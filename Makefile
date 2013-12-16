SRC_DIR = src/
BIN_DIR = bin/
CC = g++
INC = -I SFML-2.1/include
FLAGS = -Wall -Wextra -Werror -Wno-unused -g -Wno-reorder -std=c++0x
LIBS= -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
LIB = -L SFML-2.1/lib

all: game-out removeos
	@echo "\033[1;36m---- This program requires python to be installed to load custom games ----\033[0m"

main.o: $(SRC_DIR)main.cpp $(SRC_DIR)engine.h $(SRC_DIR)breakout.h $(SRC_DIR)breakout_script.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $(BIN_DIR)$@


breakout.o: $(SRC_DIR)breakout.cpp $(SRC_DIR)breakout.h $(SRC_DIR)engine.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $(BIN_DIR)$@

breakout_script.o: $(SRC_DIR)breakout_script.cpp $(SRC_DIR)breakout.h $(SRC_DIR)engine.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $(BIN_DIR)$@

engine.o: $(SRC_DIR)engine.cpp $(SRC_DIR)engine.h
	$(CXX) -c $(INC) $(FLAGS) $< -o $(BIN_DIR)$@

removeos: game-out
	rm $(BIN_DIR)*.o

game-out: main.o engine.o breakout.o breakout_script.o
	mkdir -p $(BIN_DIR)
	$(CC) $(LIB) $(FLAGS) $(BIN_DIR)main.o $(BIN_DIR)breakout.o $(BIN_DIR)breakout_script.o $(BIN_DIR)engine.o -o $(BIN_DIR)$@ $(LIBS)

clean:
	rm -rf $(BIN_DIR)


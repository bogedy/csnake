output: main.cpp utils.cpp SnakeGame.hpp Snake.hpp board.hpp
	clang++ -std=c++11 main.cpp utils.cpp -lncurses -o  output

all:
	g++ -std=c++11 -pthread -o coder -march=native -O3 -pipe -Wall coder.cpp

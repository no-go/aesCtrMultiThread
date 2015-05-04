all:
	g++ -std=c++11 -pthread -o ctr -march=native -O3 -pipe -Wall ctr.cpp

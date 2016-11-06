all: test

test: main.o
	c++ -o test main.o

main.o: main.cc blockmatr.h
	c++ -O -std=c++11 -c main.cc


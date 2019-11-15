all: parse.h productions.cpp state.h lab3.cpp
	clear
	g++ -c parse.h
	g++ -c parse.cpp
	g++ -c state.h
	g++ -c productions.cpp
	g++ -c lab3.cpp
	g++ parse.o state.o productions.o lab3.o -o lab3
	./lab3

c: parse.h productions.cpp state.h lab3.cpp
	g++ -c parse.h
	g++ -c parse.cpp
	g++ -c state.h
	g++ -c productions.cpp
	g++ -c lab3.cpp
	g++ parse.o state.o productions.o lab3.o -o lab3

r:
	./lab3
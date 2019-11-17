all: parse.h productions.cpp state.h lab4.cpp
	clear
	g++ -c parse.h
	g++ -c parse.cpp
	g++ -c state.h
	g++ -c productions.cpp
	g++ -c lab4.cpp
	g++ parse.o state.o productions.o lab4.o -o lab4
	./lab4

c: parse.h productions.cpp state.h lab4.cpp
	g++ -c parse.h
	g++ -c parse.cpp
	g++ -c state.h
	g++ -c productions.cpp
	g++ -c lab4.cpp
	g++ parse.o state.o productions.o lab4.o -o lab4

r:
	./lab4
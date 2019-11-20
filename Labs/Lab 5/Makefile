all: parse.h productions.cpp state.h lab5.cpp
	clear
	g++ -c parse.h
	g++ -c parse.cpp
	g++ -c state.h
	g++ -c productions.cpp
	g++ -c lab5.cpp
	g++ parse.o state.o productions.o lab5.o -o lab5
	./lab5

c: parse.h productions.cpp state.h lab5.cpp
	g++ -c parse.h
	g++ -c parse.cpp
	g++ -c state.h
	g++ -c productions.cpp
	g++ -c lab5.cpp
	g++ parse.o state.o productions.o lab5.o -o lab5

r:
	./lab5
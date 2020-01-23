FLAGS = -std=c++11 -g -Wall

all: build test docker

build: main.cpp
	g++ $(FLAGS) -o sorer main.cpp

test: tests/test_main.cpp
	g++ $(FLAGS) -o test tests/test_main.cpp
	./test

docker: main.cpp
	g++ $(FLAGS) main.cpp

run:
	./a.out -f data.sor

clean:
	rm -f sorer test a.out

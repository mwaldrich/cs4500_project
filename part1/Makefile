FLAGS = -std=c++11 -g -Wall

main: main.cpp
	g++ $(FLAGS) -o main main.cpp

test: tests/test_main.cpp
	g++ $(FLAGS) -o test_main tests/test_main.cpp

clean:
	rm -f main test_main

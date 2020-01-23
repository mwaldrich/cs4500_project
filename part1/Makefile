FLAGS = -std=c++11 -g -Wall

all: build test docker

build: main.cpp
	g++ $(FLAGS) -o sorer main.cpp

test: build
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 0
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 1
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 2
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 3
	./sorer -f "data.sor" -from 0 -len 100 -is_missing_idx 2 0
	./sorer -f "data.sor" -from 0 -len 100 -is_missing_idx 2 1
	./sorer -f "data.sor" -from 0 -len 100 -is_missing_idx 2 2
	./sorer -f "data.sor" -from 0 -len 100 -print_col_idx 2 0
	./sorer -f "data.sor" -from 5 -len 100 -print_col_idx 1 0

unit-test: tests/test_main.cpp
	g++ $(FLAGS) -o test tests/test_main.cpp
	./test

docker: main.cpp
	g++ $(FLAGS) main.cpp

run:
	./a.out -f data.sor

clean:
	rm -f sorer test a.out

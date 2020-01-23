FLAGS = -std=c++11 -g -Wall

sorer: main.cpp
	g++ $(FLAGS) -o sorer main.cpp

test_main: tests/test_main.cpp
	g++ $(FLAGS) -o test_main tests/test_main.cpp

test:
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 0
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 1
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 2
	./sorer -f "data.sor" -from 0 -len 100 -print_col_type 3
	./sorer -f "data.sor" -from 0 -len 100 -is_missing_idx 2 0
	./sorer -f "data.sor" -from 0 -len 100 -is_missing_idx 2 1
	./sorer -f "data.sor" -from 0 -len 100 -is_missing_idx 2 2
	./sorer -f "data.sor" -from 0 -len 100 -print_col_idx 2 0
	./sorer -f "data.sor" -from 5 -len 100 -print_col_idx 1 0

clean:
	rm -f main test_main

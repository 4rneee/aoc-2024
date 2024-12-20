FLAGS=-std=c++20
DEBUG_FLAGS=-Wall -Wextra -g
FILES=../util.cpp main.cpp

.PHONY: all main debug clean
all: main
main: $(FILES)
	g++ -o $@.out $(FLAGS) -O3 $^

debug: $(FILES)
	g++ -o $@.out $(FLAGS) $(DEBUG_FLAGS) $^

clean:
	rm -f main.out debug.out

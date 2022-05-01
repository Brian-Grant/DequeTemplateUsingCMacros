

all: Deque

Deque: test.o
	g++ -O0 -g -Wall -Wextra -pedantic -std=c++11  test.o -o Deque -ldl

test.o: test.cpp
	g++ -O0 -g -Wall -Wextra -pedantic  -std=c++11 -c test.cpp

clean:
	rm -f *.o Deque

gdb: Deque
	gdb ./Deque

val: Deque
	valgrind --leak-check=full ./Deque

valv: Deque
	valgrind --leak-check=full ./Deque

pre: Deque
	g++ -O0 -g -Wall -std=c++11 -E -c test.cpp

test: Deque
	./Deque

CC = g++

Headers = expr.h exprtree.h tree.h node.h

CompileParms = -c -std=c++11 -Wall -Wextra

OBJS = main.o Automaton.o

Autom: $(OBJS)
	$(CC) $(OBJS) -o autom

main.o: main.cpp
	$(CC) $(CompileParms)  main.cpp

Automaton.o: Automaton.cpp $(Headers)
	$(CC) $(CompileParms)  Automaton.cpp

clean:
	rm -f autom *.o *.h.gch

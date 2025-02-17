
debug: debug.o BitBoard.o
	gcc -Wall -g debug.o BitBoard.o -o debug -lm

debug.o: debug.c debug.h BitBoard.h
	gcc -Wall -g -c debug.c -o debug.o

BitBoard.o: BitBoard.c BitBoard.h
	gcc -Wall -g -c BitBoard.c -o BitBoard.o

clean:
	rm *.o debug

cleantxt:
	rm RookAttackDebug.txt
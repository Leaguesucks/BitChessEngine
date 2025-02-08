
Chess: ChessGame.o
	gcc -Wall -g ChessGame.o -o chess -lm

ChessGame.o: ChessGame.c ChessGame.h BitBoard.h
	gcc -Wall -g -c ChessGame.c -o ChessGame.o

clean:
	rm *.o chess BitBoard
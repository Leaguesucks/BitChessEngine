
ChessGame: ChessGame.o
	gcc -Wall -g ChessGame.o -o ChessGame -lm

ChessGame.o: ChessGame.c BitBoard.h Physical_Board.h
	gcc -Wall -g -c ChessGame.c -o ChessGame.o

clean:
	rm *.o ChessGame
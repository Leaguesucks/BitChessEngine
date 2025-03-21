# I know it looks unprofessional but I aim for simplicity and 
# understandability

CC = gcc
BFLAGS = $(CFLAGS) -I$(BPATH)
CFLAGS = -Wall -g
BPATH = srcs/Board
TPATH = Test
OPATH = bin
RPATH = resources
LDFLAGS = -lm
TARGETS = $(OPATH)/magic $(OPATH)/debug
DEBUGOBJ = $(OPATH)/debug.o $(OPATH)/BitManipulation.o $(OPATH)/DataBase.o $(OPATH)/MoveGen.o $(OPATH)/ChessGame.o

all: odir $(TARGETS)

odir:
	mkdir -p $(OPATH)

$(OPATH)/magic: $(OPATH)/magic.o $(OPATH)/BitManipulation.o $(OPATH)/DataBase.o
	$(CC) $(CFLAGS) $(OPATH)/magic.o $(OPATH)/BitManipulation.o $(OPATH)/DataBase.o -o $(OPATH)/magic $(LDFLAGS)

$(OPATH)/magic.o: $(BPATH)/MagicGen.c $(BPATH)/MagicGen.h
	$(CC) $(CFLAGS) -c $(BPATH)/MagicGen.c -o $(OPATH)/magic.o

$(OPATH)/debug: $(DEBUGOBJ)
	$(CC) $(CFLAGS) $(DEBUGOBJ) -o $(OPATH)/debug $(LDFLAGS)

$(OPATH)/debug.o: $(TPATH)/debug.c $(TPATH)/debug.h $(BPATH)/BitBoard.h
	$(CC) $(CFLAGS) -c $(TPATH)/debug.c -o $(OPATH)/debug.o

$(OPATH)/ChessGame.o: $(BPATH)/ChessGame.c $(BPATH)/ChessGame.h $(BPATH)/BitBoard.h
	$(CC) $(CFLAGS) -c $(BPATH)/ChessGame.c -o $(OPATH)/ChessGame.o

$(OPATH)/MoveGen.o: $(BPATH)/MoveGen.c $(BPATH)/MoveGen.h $(BPATH)/BitBoard.h
	$(CC) $(CFLAGS) -c $(BPATH)/MoveGen.c -o $(OPATH)/MoveGen.o

$(OPATH)/DataBase.o: $(BPATH)/DataBase.c $(BPATH)/DataBase.h $(BPATH)/BitBoard.h
	$(CC) $(CFLAGS) -c $(BPATH)/DataBase.c -o $(OPATH)/DataBase.o

$(OPATH)/BitManipulation.o: $(BPATH)/BitManipulation.c $(BPATH)/BitManipulation.h $(BPATH)/BitBoard.h
	$(CC) $(CFLAGS) -c $(BPATH)/BitManipulation.c -o $(OPATH)/BitManipulation.o

clean:
	rm $(OPATH)/*.o $(TARGETS)

cleantxt:
	rm  $(RPATH)/*.txt
# I know it looks unprofessional but I aim for simplicity and 
# understandability

CC = gcc
BFLAGS = $(CFLAGS) -I$(BPATH)
CFLAGS = -Wall -g
BPATH = srcs/Board
OPATH = bin
RPATH = resources
LDFLAGS = -lm
TARGETS = $(OPATH)/magic $(OPATH)/debug
DEBUGOBJ = $(OPATH)/debug.o $(OPATH)/BitManipulation.o $(OPATH)/DataBase.o $(OPATH)/MoveGen.o $(OPATH)/ChessGame.o

all: odir $(TARGETS)

odir:
	mkdir -p $(OPATH)

$(OPATH)/magic: $(OPATH)/magic.o $(OPATH)/BitManipulation.o $(OPATH)/DataBase.o
	$(CC) $(BFLAGS) $(OPATH)/magic.o $(OPATH)/BitManipulation.o $(OPATH)/DataBase.o -o $(OPATH)/magic $(LDFLAGS)

$(OPATH)/magic.o: $(BPATH)/MagicGen.c $(BPATH)/MagicGen.h
	$(CC) $(BFLAGS) -c $(BPATH)/MagicGen.c -o $(OPATH)/magic.o

$(OPATH)/debug: $(DEBUGOBJ)
	$(CC) $(BFLAGS) $(DEBUGOBJ) -o $(OPATH)/debug $(LDFLAGS)

$(OPATH)/debug.o: $(BPATH)/debug.c $(BPATH)/debug.h
	$(CC) $(BFLAGS) -c $(BPATH)/debug.c -o $(OPATH)/debug.o

$(OPATH)/ChessGame.o: $(BPATH)/ChessGame.c $(BPATH)/ChessGame.h
	$(CC) $(BFLAGS) -c $(BPATH)/ChessGame.c -o $(OPATH)/ChessGame.o

$(OPATH)/MoveGen.o: $(BPATH)/MoveGen.c $(BPATH)/MoveGen.h
	$(CC) $(BFLAGS) -c $(BPATH)/MoveGen.c -o $(OPATH)/MoveGen.o

$(OPATH)/DataBase.o: $(BPATH)/DataBase.c $(BPATH)/DataBase.h
	$(CC) $(BFLAGS) -c $(BPATH)/DataBase.c -o $(OPATH)/DataBase.o

$(OPATH)/BitManipulation.o: $(BPATH)/BitManipulation.c $(BPATH)/BitManipulation.h
	$(CC) $(BFLAGS) -c $(BPATH)/BitManipulation.c -o $(OPATH)/BitManipulation.o

clean:
	rm $(OPATH)/*.o $(TARGETS)

cleantxt:
	rm  $(RPATH)/*.txt
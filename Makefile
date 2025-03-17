# I know it looks unprofessional but I aim for simplicity and 
# understandability

CC = gcc
BFLAGS = $(CFLAGS) -I$(BPATH)
CFLAGS = -Wall -g
BPATH = srcs/Board
LDFLAGS = -lm
TARGETS = bin/magic bin/debug

all: bin $(TARGETS)

bin:
	mkdir -p bin

bin/magic: bin/magic.o bin/BitManipulation.o bin/DataBase.o
	$(CC) $(BFLAGS) bin/magic.o bin/BitManipulation.o bin/DataBase.o -o bin/magic $(LDFLAGS)

bin/magic.o: $(BPATH)/MagicGen.c $(BPATH)/MagicGen.h
	$(CC) $(BFLAGS) -c $(BPATH)/MagicGen.c -o bin/magic.o

bin/debug: debug.o BitManipulation.o DataBase.o
	$(CC) $(BFLAGS) debug.o BitManipulation.o DataBase.o -o debug $(LDFLAGS)

bin/debug.o: debug.c debug.h
	$(CC) $(BFLAGS) -c debug.c -o debug.o

bin/DataBase.o: DataBase.c DataBase.h
	$(CC) $(BFLAGS) -c DataBase.c -o DataBase.o

bin/BitManipulation.o: BitManipulation.c BitManipulation.h
	$(CC) $(BFLAGS) -c BitManipulation.c -o BitManipulation.o

clean:
	rm bin/*.o $(TARGETS)

cleantxt:
	rm -f *.txt
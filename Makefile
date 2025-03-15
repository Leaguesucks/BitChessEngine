all: magic debug

magic: magic.o BitManipulation.o DataBase.o
	gcc -Wall -g magic.o BitManipulation.o DataBase.o -o magic -lm

magic.o: MagicGen.c MagicGen.h
	gcc -Wall -g -c MagicGen.c -o magic.o

debug: debug.o BitManipulation.o DataBase.o
	gcc -Wall -g debug.o BitManipulation.o DataBase.o -o debug -lm

debug.o: debug.c debug.h
	gcc -Wall -g -c debug.c -o debug.o

DataBase.o: DataBase.c DataBase.h
	gcc -Wall -g -c DataBase.c -o DataBase.o

BitManipulation.o: BitManipulation.c BitManipulation.h
	gcc -Wall -g -c BitManipulation.c -o BitManipulation.o

clean:
	rm *.o debug magic

cleantxt:
	rm *.txt
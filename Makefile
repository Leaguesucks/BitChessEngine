CC = gcc
CFLAGS = -Wall -g -mbmi -mlzcnt $(shell sdl2-config --cflags)
BPATH = srcs/Board
GPATH = srcs/GUI
TPATH = Test
OPATH = bin
RPATH = resources
LDFLAGS = -lm $(shell sdl2-config --libs) -lSDL2_image

# List of object files
OBJS = $(OPATH)/BitManipulation.o $(OPATH)/DataBase.o $(OPATH)/MoveGen.o $(OPATH)/ChessGame.o
DEBUGOBJ = $(OPATH)/debug.o $(OBJS) $(OPATH)/ChessGUI.o
MAGICOBJ = $(OPATH)/magic.o $(OBJS)

# Targets
all: $(OPATH) $(OPATH)/magic $(OPATH)/debug

$(OPATH):
	mkdir -p $(OPATH)

$(OPATH)/magic: $(MAGICOBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OPATH)/debug: $(DEBUGOBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Pattern rule for object files

$(OPATH)/ChessGUI.o: $(GPATH)/ChessGUI.c $(GPATH)/ChessGUI.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile all header files in Board directory
$(OPATH)/%.o: $(BPATH)/%.c $(BPATH)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OPATH)/debug.o: $(TPATH)/debug.c $(TPATH)/debug.h $(BPATH)/BitBoard.h $(GPATH)/ChessGUI.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OPATH)/magic.o: $(BPATH)/MagicGen.c $(BPATH)/MagicGen.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OPATH)/*.o $(OPATH)/magic $(OPATH)/debug

cleantxt:
	rm -f $(RPATH)/*.txt

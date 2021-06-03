DIRINCLUDE=./include
DIROBJETO=./obj
CC=g++
CFLAGS=-I$(DIRINCLUDE) -g -lSDL2 -std=c++2a

_DEPEND = worldmap.hpp map.hpp
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = worldmap.o map.o validate_map_creator.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.cpp $(DEPEND)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

all: validate_map_creator.out

validate_map_creator.out: ./obj/worldmap.o ./obj/validate_map_creator.o
	$(CC) -o ./bin/$@ $^ $(CFLAGS)

clean:
	rm -f ./bin/*
	rm -f ./obj/*

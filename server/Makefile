DIRINCLUDE=./include
DIROBJETO=./obj
CC=g++
CFLAGS=-I$(DIRINCLUDE) -g -lcurlpp -lcurl -std=c++2a

_DEPEND = auth_comm.hpp
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = auth_comm.o main.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.cpp $(DEPEND)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

all: server.out

server.out: ./obj/auth_comm.o ./obj/main.o
	@mkdir -p bin
	$(CC) -o ./bin/$@ $^ $(CFLAGS)

clean:
	rm -f ./bin/*
	rm -f ./obj/*

DIRINCLUDE=./include
DIROBJ=./obj
DIRSRC=./src
CC=g++
CFLAGS=-I$(DIRINCLUDE) -g -lcurlpp -lcurl -std=c++2a

_DEPEND = auth_comm.hpp
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJ = auth_comm.o socket_addr_factory.o user_comm.o main.o
OBJ = $(patsubst %,$(DIROBJ)/%,$(_OBJ))

$(DIROBJ)/%.o: $(DIRSRC)/%.cpp $(DEPEND)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

all: server.out

server.out: $(OBJ)
	@mkdir -p bin
	$(CC) -o ./bin/$@ $^ $(CFLAGS)

clean:
	rm -f ./bin/*
	rm -f ./obj/*

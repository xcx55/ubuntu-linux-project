BIN=main.exe
SRC=$(wildcard *.cxx)
OBJ=$(SRC:%.cxx=%.o)
CC=g++
$(BIN):$(OBJ)
	$(CC) -o $@ $^
%.o:%.cxx
	$(CC) -c $< -o $@
.PHONY:clean
clean:
	rm -rf $(BIN) $(OBJ)
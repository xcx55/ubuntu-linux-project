BIN=main.exe
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
CC=gcc
$(BIN):$(OBJ)
	$(CC) -o $@ $^
%.o:%.c
	$(CC) -c $< -o $@
.PHONY:clean
clean:
	rm -rf $(BIN) $(OBJ)
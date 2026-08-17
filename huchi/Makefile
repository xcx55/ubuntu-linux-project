SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
CC=g++
want=main

$(want):$(OBJ)
	$(CC) -o $@ $^
%.o:%.cc
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm -rf $(OBJ) $(want)
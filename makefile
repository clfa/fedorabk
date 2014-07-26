CC = gcc
OPT = -Wall -o
fedorabk: main.c fedorabk.c fedorabk.h
	@$(CC) $(OPT) $@ $^

.PHONY: clean
clean:
	@rm -rf fedorabk

CC = gcc
CFLAGS = -Wall -g
LIBS = -lgmp

# Compilar qualquer ficheiro .c para executável
%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)
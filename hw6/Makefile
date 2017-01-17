CC = gcc
CFLAGS = -I.
PROGRAM = banker

all: banker.c
	$(CC) -o $(PROGRAM) banker.c $(CFLAGS)

run: $(PROGRAM)
	./$(PROGRAM) $(INPUT)

clean:
	rm $(PROGRAM)

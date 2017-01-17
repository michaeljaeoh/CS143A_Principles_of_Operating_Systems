CC = gcc
CFLAGS = -I.
PROGRAM = handle_signals

all: handle_signals.c
	$(CC) -o $(PROGRAM) handle_signals.c $(CFLAGS)

run: $(PROGRAM)
	./$(PROGRAM) $(INPUT)

clean:
	rm $(PROGRAM)


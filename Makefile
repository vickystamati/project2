CC = gcc
OBJECTS = main.o list.o hash.o
CFLAGS = -lm

lsh: $(OBJECTS)
	$(CC) -o lsh $(OBJECTS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c $(CFLAGS)

list.o: list.c
	$(CC) -c list.c $(CFLAGS)

hash.o: hash.c
	$(CC) -c hash.c

.PHONY: clean

clean:
	rm -f lsh $(OBJECTS)

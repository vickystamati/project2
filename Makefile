CC = gcc
OBJECTS = main.o list.o hash.o update.o lsh.o initialise.o clara.o assignment.o execute.o
CFLAGS = -lm

lsh: $(OBJECTS)
	$(CC) -o medoids $(OBJECTS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c 

list.o: list.c
	$(CC) -c list.c 

hash.o: hash.c
	$(CC) -c hash.c
	
update.o: update.c
	$(CC) -c update.c

lsh.o: lsh.c
	$(CC) -c lsh.c

initialise.o: initialise.c
	$(CC) -c initialise.c

clara.o: clara.c
	$(CC) -c clara.c

assignment.o: assignment.c
	$(CC) -c assignment.c

execute.o: execute.c
	$(CC) -c execute.c

.PHONY: clean

clean:
	rm -f medoids $(OBJECTS)

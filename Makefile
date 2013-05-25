CC = gcc
#CFLAGS = -D_XOPEN_SOURCE --std=c99

all: heracles hydra

hydra: hydra.c linked_list.o

heracles: heracles.c

linked_list.o: linked_list.c
	gcc -c -std=c99 linked_list.c

clean:
	-rm -f *.o *.d heracles hydra

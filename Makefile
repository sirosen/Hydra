CC = gcc
CFLAGS = -D_POSIX_C_SOURCE -D_SVID_SOURCE --std=c99

all: heracles hydra

hydra: hydra.c linked_list.c

heracles: heracles.c

clean:
	-rm -f *.o *.d heracles hydra

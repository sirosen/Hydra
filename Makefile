CC = gcc
CFLAGS = -g3 -Wall -std=c99 -MMD -MPl -D_SVID_SOURCE

all: heracles hydra

hydra: hydra.c

heracles: heracles.c

clean:
	-rm -f *.o *.d heracles hydra

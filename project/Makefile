CC = gcc
CFLAGS = -std=c11

SOURCES = main.c aux.c fsm.c iom361_r4.c report.c sensor.c store.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: dishwasher

dishwasher: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) dishwasher
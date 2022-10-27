.PHONY: all, clean, rebuild

CC = gcc
CFLAGS = -c

BUILD_PATH = ../build/
SOURCES =  main.c
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = MAIN

all: $(OBJECTS)
	$(CC) $(BUILD_PATH)$(OBJECTS) -o $(BUILD_PATH)$(EXECUTABLE)


rebuild: clean all

%.o: %.c
	$(CC) $(CFLAGS) $< -o $(BUILD_PATH)$@

clean:
	-rm  $(BUILD_PATH)*
# Top-level Makefile for enumfactory_test
CC=gcc
CFLAGS=-I./enumfactory/include -Wall -Wextra -O2
TARGET=enumfactory_test
SRC=test/enumfactory_test.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all test clean

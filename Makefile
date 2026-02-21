# Top-level Makefile for enumfactory static library and tests
CC=gcc
AR=ar
CFLAGS=-I./include -Wall -Wextra -O2
LDFLAGS=-L. -lenumfactory

SRCS=src/enumfactory.c
OBJS=$(SRCS:.c=.o)
LIB=libenumfactory.a

TEST_SRC=tests/enumfactory_test.c
TEST_BIN=enumfactory_test

all: $(LIB) $(TEST_BIN)

$(LIB): $(OBJS)
	$(AR) rcs $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SRC) $(LIB)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $@ $(LDFLAGS)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(OBJS) $(LIB) $(TEST_BIN)

.PHONY: all test clean

# Makefile for building the enumfactory library
LIB_NAME=enumfactory
LIB_TYPE=SHARED   # change to STATIC for static library
CC=gcc
CFLAGS=-I../include -Wall -O2
SRC=src/enumfactory.c

ifeq ($(LIB_TYPE),SHARED)
	OUT=lib$(LIB_NAME).so
	LINK_FLAGS=-shared
else
	OUT=lib$(LIB_NAME).a
	LINK_FLAGS=
endif

# 'all' compiles the library.
all: $(OUT)

# Build rule for the library.
$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(LINK_FLAGS) -o $(OUT) $(SRC)

# 'clean' removes the built library file.
clean:
	rm -f $(OUT)

# ...existing rules...

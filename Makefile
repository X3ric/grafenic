# Variables
CC = gcc -O2 -g # -Wextra 
CFLAGS = -Wall -I./include
LDFLAGS = -lglfw -lGL -lGLEW -lm
TARGET = ./grafenic
SOURCES = ./src/*.c
HEADERS = ./src/*/*.h
# Default target
all: $(TARGET)
# Build rules
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
# Clean rule
clean:
	rm -f $(TARGET)
# Run rule
run:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
	$(TARGET)
# Run with debugger rule
gdbrun:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
	gdb $(TARGET)
# Overlay rule
.PHONY: clean

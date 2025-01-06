# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

# Target executable
TARGET = sentiment_analysis

# Object files
OBJS = main.o vaderSentiment.o

# Build target
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile main.c
main.o: main.c utility.h
	$(CC) -c main.c -o main.o $(CFLAGS)

# Compile vaderSentiment.c
vaderSentiment.o: vaderSentiment.c utility.h
	$(CC) -c vaderSentiment.c -o vaderSentiment.o $(CFLAGS)

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)
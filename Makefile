# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Output executable
TARGET = webserver

# Source and object files
SRC = main.c GetRequest.c ClientRequest.c MakeHeader.c Routing.c
OBJ = $(SRC:.c=.o)

# Default rule
all: $(TARGET)

# Linking step
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilation step for each .c file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(TARGET)
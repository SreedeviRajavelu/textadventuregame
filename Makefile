# Compiler options
CC = gcc
CFLAGS = -std=c99 -O3 -march=native
LIBS = -lm -lregex

# Source files
SRCS = gamefile.c pbPlots.c supportLib.c
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = gamefile

# Default target
all: $(TARGET)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

# Run the executable with user-defined arguments
run:
	./$(TARGET).exe startingfile.txt $(CSVFILE)

# Clean generated files
clean:
	rm -f $(OBJS) $(TARGET)

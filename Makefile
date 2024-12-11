# Variables
CC = gcc                    # The compiler
CFLAGS = -Wall -g           # Compiler flags: show warnings (-Wall), include debug info (-g)
SRCS = $(wildcard *.c)      # Find all .c files in the current directory
OBJS = $(SRCS:.c=.o)        # Convert .c file list to .o object files

# Rule to build the final executable (a.out)
a.out: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

# Rule to build each .o file from .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove all compiled files
clean:
	rm -f $(OBJS) a.out

# Run the executable
run: a.out
	./a.out

# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread

# Source and build directories
SRCDIR = .
BUILDDIR = build

# Source files
SRCS = $(SRCDIR)/main.c $(SRCDIR)/linkedlist_serial.c $(SRCDIR)/linkedlist_mutex.c $(SRCDIR)/linkedlist_rwlock.c
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

# Output executable
TARGET = $(BUILDDIR)/LinkedListProject.exe

# Default rule to build the project
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

# Rule to compile source files into object files in the build directory
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean up object files and executable
clean:
	@if [ -d $(BUILDDIR) ]; then rm -rf $(BUILDDIR); fi


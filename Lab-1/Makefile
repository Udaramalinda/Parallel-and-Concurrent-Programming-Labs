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
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

# Rule to compile source files into object files in the build directory
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean up object files and executable
clean:
	@if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)

# # -----------------make Udara-----------------
# # Compiler and flags
# CC = gcc
# CFLAGS = -Wall -pthread -lm

# # Source and build directories
# SRCDIR = .
# BUILDDIR = build

# # Source files
# SRCS = $(SRCDIR)/main.c $(SRCDIR)/linkedlist_serial.c $(SRCDIR)/linkedlist_mutex.c $(SRCDIR)/linkedlist_rwlock.c
# OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

# # Output executable
# TARGET = $(BUILDDIR)/LinkedListProject.exe

# # Default rule to build the project
# all: $(TARGET)

# # Rule to link object files and create the executable
# $(TARGET): $(OBJS)
# 	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
# 	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

# # Rule to compile source files into object files in the build directory
# $(BUILDDIR)/%.o: $(SRCDIR)/%.c
# 	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# # Clean up object files and executable
# clean:
# 	@if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)

# # ==============================plot the graph make file============================

# # Compiler and flags
# CC = gcc
# CFLAGS = -Wall -pthread -lm

# # Source and build directories
# SRCDIR = .
# BUILDDIR = build

# # Source files
# SRCS = $(SRCDIR)/main.c $(SRCDIR)/linkedlist_serial.c $(SRCDIR)/linkedlist_mutex.c $(SRCDIR)/linkedlist_rwlock.c
# OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

# # Output executable
# TARGET = $(BUILDDIR)/LinkedListProject.exe

# # Python script
# PYTHON_SCRIPT = plot_serial_times.py

# # Default rule to build the project
# all: $(TARGET) run_python

# # Rule to link object files and create the executable
# $(TARGET): $(OBJS)
# 	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
# 	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

# # Rule to compile source files into object files in the build directory
# $(BUILDDIR)/%.o: $(SRCDIR)/%.c
# 	@if not exist $(BUILDDIR) mkdir $(BUILDDIR)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# # Rule to run the compiled program and create CSV file
# run_c_program: $(TARGET)
# 	@echo "Running the C program..."
# 	@./$(TARGET)

# # Rule to run the Python script after the C program
# run_python: run_c_program
# 	@echo "Running the Python script to plot the graph..."
# 	python3 $(PYTHON_SCRIPT)

# # Clean up object files and executable
# clean:
# 	@if exist $(BUILDDIR) rmdir /s /q $(BUILDDIR)

# -----------------make sandil-----------------
CC = gcc
CFLAGS = -Wall -pthread -lm

# Source and build directories
SRCDIR = .
BUILDDIR = build
ASSEMBLYDIR = assembly

# Source files
SRCS = $(SRCDIR)/main.c $(SRCDIR)/linkedlist_serial.c $(SRCDIR)/linkedlist_mutex.c $(SRCDIR)/linkedlist_rwlock.c
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))
ASSEMBLY = $(patsubst $(SRCDIR)/%.c, $(ASSEMBLYDIR)/%.s, $(SRCS))

# Output executable
TARGET = $(BUILDDIR)/LinkedListProject.exe

# Default rule to build the project (both object files and assembly code)
all: $(TARGET) $(ASSEMBLY)

# Rule to link object files and create the executable
$(TARGET): $(OBJS)
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

# Rule to compile source files into object files in the build directory
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@if [ ! -d $(BUILDDIR) ]; then mkdir -p $(BUILDDIR); fi
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to generate assembly code
$(ASSEMBLYDIR)/%.s: $(SRCDIR)/%.c
	@if [ ! -d $(ASSEMBLYDIR) ]; then mkdir -p $(ASSEMBLYDIR); fi
	$(CC) -S -o $@ $< $(CFLAGS)

# Clean up object files, executable, and assembly code
clean:
	@if [ -d $(BUILDDIR) ]; then rm -rf $(BUILDDIR); fi
	@if [ -d $(ASSEMBLYDIR) ]; then rm -rf $(ASSEMBLYDIR); fi

run: $(TARGET)
	./$(TARGET) 0.99 0.005 0.005 4

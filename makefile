# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Iinclude

LIBS = -lpcap

# Source directory
SRCDIR = src

# Object directory
OBJDIR = obj

# Default target name
TARGET ?= main

# Find all C source files
SOURCES = $(shell find $(SRCDIR) -name '*.c')

# Automatically generate object files list
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Run code
run: $(TARGET)
	wine $(TARGET)

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJDIR)/*.o

.PHONY: all clean

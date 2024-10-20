# Compiler and Linking Variables
CC = gcc
CFLAGS = -Wall -fPIC -pthread 
LIB_NAME = libmemory_manager.so

# Source and Object Files
SRC = memory_manager.c linked_list.c test_linked_list.c test_memory_manager.c
OBJ = $(SRC:.c=.o)

# Default target
all: mmanager list test_mmanager test_list

# Rule to create the dynamic library
$(LIB_NAME): memory_manager.o
	$(CC) -shared -o $@ $(OBJ)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build the memory manager
mmanager: $(LIB_NAME)

# Build the linked list
list: linked_list.o

# Test target to run the memory manager test program
test_mmanager: $(LIB_NAME)
	$(CC) -o test_memory_manager test_memory_manager.o -L. -lmemory_manager -pthread  

# Test target to run the linked list test program
test_list: $(LIB_NAME) linked_list.o
	$(CC) -o test_linked_list test_linked_list.o -L. -lmemory_manager -pthread 

# Run tests
run_tests: run_test_mmanager run_test_list

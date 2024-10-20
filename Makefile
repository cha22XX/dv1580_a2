# Compiler and Linking Variables
CC = gcc
CFLAGS = -Wall -fPIC -pthread 
LIB_NAME = libmemory_manager.so

# Source and Object Files
SRC = memory_manager.c linked_list.c test_linked_list.c test_memory_manager.c
OBJ = $(SRC:.c=.o)

# Default target: Build everything
all: $(LIB_NAME) test_mmanager test_list

# Rule to create the dynamic library
$(LIB_NAME): memory_manager.o linked_list.o
	$(CC) -shared -o $@ memory_manager.o linked_list.o

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Test target to build the memory manager test program
test_mmanager: test_memory_manager.o $(LIB_NAME)
	$(CC) -o $@ test_memory_manager.o -L. -lmemory_manager -pthread -lm

# Test target to build the linked list test program
test_list: test_linked_list.o $(LIB_NAME)
	$(CC) -o $@ test_linked_list.o -L. -lmemory_manager -pthread -lm 

# Run tests
run_tests: run_test_mmanager run_test_list
 
# Run test cases for the memory manager
run_test_mmanager:
	./test_mmanager

# Run test cases for the linked list
run_test_list:
	./test_list

# Clean target to clean up build files
clean:
	rm -f $(OBJ) $(LIB_NAME) test_mmanager test_list

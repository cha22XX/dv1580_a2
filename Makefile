CC = gcc
CFLAGS = -Wall -fPIC -pthread
LIB_NAME = libmemory_manager.so

SRC_MM = memory_manager.c
SRC_LL = linked_list.c
SRC_TEST_MM = test_memory_manager.c
SRC_TEST_LL = test_linked_list.c

OBJ_MM = memory_manager.o
OBJ_LL = linked_list.o
OBJ_TEST_MM = test_memory_manager.o
OBJ_TEST_LL = test_linked_list.o

all: mmanager list test_mmanager test_list

$(LIB_NAME): $(OBJ_MM)
    $(CC) -shared -o $@ $(OBJ_MM)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

mmanager: $(LIB_NAME)

list: $(OBJ_LL)

test_mmanager: $(LIB_NAME) $(OBJ_TEST_MM)
    $(CC) -o test_memory_manager $(OBJ_TEST_MM) -L. -lmemory_manager -pthread

test_list: $(LIB_NAME) $(OBJ_LL) $(OBJ_TEST_LL)
    $(CC) -o test_linked_list $(OBJ_LL) $(OBJ_TEST_LL) -L. -lmemory_manager -pthread

run_tests: run_test_mmanager run_test_list

run_test_mmanager:
    ./test_memory_manager

run_test_list:
    ./test_linked_list

clean:
    rm -f $(OBJ_MM) $(OBJ_LL) $(OBJ_TEST_MM) $(OBJ_TEST_LL) $(LIB_NAME) test_memory_manager test_linked_list

CC = gcc
CFLAGS = -Wall -fPIC

OBJS_STATIC = fred.o john.o
OBJS_SHARED = bill.o sam.o
OBJ_PROGRAM = program.o
OBJ_WRAP_LINK = interpose_link.o

LIB_STATIC = libstatic.a
LIB_SHARED = libshared.so
LIB_WRAP_LOAD = libwrap_load.so

all: $(LIB_STATIC) $(LIB_SHARED) $(LIB_WRAP_LOAD) program

$(LIB_STATIC): $(OBJS_STATIC)
	ar crv $@ $^

$(LIB_SHARED): $(OBJS_SHARED)
	$(CC) -shared -o $@ $^ -lm

$(LIB_WRAP_LOAD): interpose_load.c
	$(CC) $(CFLAGS) -shared -o $@ $<

program: $(OBJ_PROGRAM) $(OBJ_WRAP_LINK)
	$(CC) -o $@ $^ -L. -lstatic -L. -lshared -lm -Wl,-rpath,.

%.o: %.c
	$(CC) $(CFLAGS) -c $<

run: program
	@echo "Running with LD_PRELOAD..."
	LD_PRELOAD=./$(LIB_WRAP_LOAD) ./program

clean:
	rm -f *.o *.a *.so program

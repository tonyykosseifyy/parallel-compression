CC=gcc
CFLAGS=-Iinclude
DEPS = include/headers.h
OBJ = obj/main.o obj/utils.o

# Rule to generate object files; depends on source files and header files
obj/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# Rule to link object files into an executable
bin/program: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

# Clean command to remove object files and the executable
clean:
	rm -f obj/*.o bin/program

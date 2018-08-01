COMPILER = gcc
FLAGS = -g -Wall -Wextra -std=c11 -O2 -pedantic-errors

TARGET = bin/wbvm

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

$(TARGET): $(OBJ)
	@mkdir bin
	$(COMPILER) $(FLAGS) -o $(TARGET) $(OBJ)
obj/%.o: src/%.c
	@mkdir -p obj/
	$(COMPILER) $(FLAGS) -c $< -o $@ $(LIBS)
clean:
	$(RM) -rf obj
	$(RM) -rf bin
	$(RM) $(TARGET)

.PHONY: depend clean 

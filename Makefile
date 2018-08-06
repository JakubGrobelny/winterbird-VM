COMPILER = clang
FLAGS = -g -Wall -Wextra -std=c11 -O0 -pedantic-errors

TARGET = bin/wbvm

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

$(TARGET): $(OBJ)
	$(RM) -rf bin
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

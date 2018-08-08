COMPILER = gcc
FLAGS = -shared 

TARGET = testlib.so
SRC = testlib.c

make:
	$(COMPILER) $(FLAGS) -o $(TARGET) -fPIC $(SRC)
clean:
	$(RM) $(TARGET)
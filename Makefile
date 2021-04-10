
CC = g++
LIBS = -lGL -lGLU -lGLEW -lglut -lglfw -lpthread
OPS = -O3


FILE_EJEMPLO = src/shaderEjemplo.cpp
FILE_TEST = src/test.cpp
OUTPUT = -o shader.o

main: $(FILES)
	$(CC) $(OUTPUT) $(FILE_EJEMPLO) $(LIBS) $(OPS)

test:
	$(CC) $(OUTPUT) $(FILE_TEST) $(LIBS) $(OPS)

.PHONY: clean
clean:
	rm -rf bin/*.o

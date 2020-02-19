
CC = g++
LIBS = -lGL -lGLU -lGLEW -lglut -lglfw -lpthread
OPS = -O3


FILE_EJEMPLO = src/shaderEjemplo.cpp
FILE_TEST = src/test.cpp
OUTPUT_TEST = -o bin/test_gl.o

main: $(FILES)
	$(CC) $(OUTPUT) $(FILE_EJEMPLO) $(LIBS) $(OPS)

test:
	$(CC) $(OUTPUT_TEST) $(FILE_TEST) $(LIBS) $(OPS)

.PHONY: clean
clean:
	rm -rf bin/*.o

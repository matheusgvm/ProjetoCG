CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL -lSOIL -lSDL -lSDL_mixer
TODOS_ARQUIVOS_OBJ = main.o

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o main $^ $(OPCOES_COMPILADOR)

run: all
	./main

clean:
	rm *.o main

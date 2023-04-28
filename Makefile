CC = g++

BREW = $(shell brew --prefix)

CFLAGS = -g -std=c++11 -Wno-deprecated-declarations
INCFLAGS = -Iinclude -I$(BREW)/include
LDFLAGS = -framework OpenGL -L$(BREW)/lib -lglfw

RM = /bin/rm -f
all: menv
menv: main.o Camera.o Cube.o Shader.o SpringDamper.o Grid.o Window.o
	$(CC) -o menv main.o Camera.o Cube.o Shader.o SpringDamper.o Grid.o Window.o $(LDFLAGS)
main.o: main.cpp include/Window.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
Camera.o: src/Camera.cpp include/Camera.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Camera.cpp
Cube.o: src/Cube.cpp include/Cube.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Cube.cpp
Shader.o: src/Shader.cpp include/Shader.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Shader.cpp
SpringDamper.o: src/SpringDamper.cpp include/SpringDamper.h include/Particle.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/SpringDamper.cpp
Grid.o: src/Grid.cpp include/Grid.h include/Triangle.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Grid.cpp
Window.o: src/Window.cpp include/Window.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Window.cpp

clean: 
	$(RM) *.o menv

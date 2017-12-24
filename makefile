CFILES=$(shell find src -name '*.cpp')
OFILES=$(patsubst src/%.cpp,build/%.o,$(CFILES))
TARGET=build/final

OS := $(shell uname)
ifeq ($(OS),Darwin)
FLAGS=-framework OpenGL -lpthread -lsfml-system -lsfml-window -lsfml-graphics -lGLEW -L/usr/local/lib
CC_FLAGS=-std=c++11 -I/usr/local/include -g -march=native -mtune=native
else
FLAGS=-lGL -lpthread -lsfml-system -lsfml-window -lsfml-graphics -lGLEW
-CC_FLAGS=-std=c++11 -g -march=native -mtune=native
endif

$(TARGET): $(OFILES)
	g++ -g -o $(TARGET) $(OFILES) $(FLAGS)

build/%.o: src/%.cpp
	g++ $(CC_FLAGS) -o $@ -c $<

clean:
	rm -rf $(TARGET) $(TARGET).dSYM $(OFILES)

INC = -I/usr/local/Cellar/glm/0.9.8.3/include -I/usr/local/Cellar/freetype/2.7.1/include/freetype2 -I//Users/Kawa/Documents/GitHub/puzzlebooble/puzzleBobble/2DGame/puzzleBobble/02-Bubble/include
FLAGS = -Wall -std=c++11 
LIBS = -framework OpenGL -framework GLUT -lglew -lSOIL -lfreetype -lirrklang
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

game: $(OBJ_FILES)
	g++ $(FLAGS) $(LIBS) -o $@ $^

obj/%.o: %.cpp
	g++ $(FLAGS) -c -o $@ $< $(INC)

clean: game
	rm -f game $(OBJ_FILES)
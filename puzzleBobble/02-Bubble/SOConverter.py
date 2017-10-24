import os
import fileinput
import glob
import sys

def replaceInFile(file,replace,final):
	mod = False
	for line in fileinput.input(file, inplace=True): 
		print(line.rstrip().replace(replace, final))

if (len(sys.argv) == 3 and sys.argv[2] != 'w') or len(sys.argv) < 2 or len(sys.argv) > 3:
	print("usage:\n\tadapting to mac:\t" + sys.argv[0] + " path\n\tadapting to windows:\t" + sys.argv[0] + " path w");
else:
	os.chdir(sys.argv[1])

	final = ["<GLUT/glut.h","<OpenGL/gl.h","glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | "] #MAC includes 
	replace = ["<GL/glut.h","<GL/gl.h","glutInitDisplayMode("] #WINDOWS includes
	length = len(replace)

	if len(sys.argv) > 2 and sys.argv[2] == "w":
		final,replace = replace,final
	else:
		if not os.path.isfile("Makefile"):
			make = open("Makefile", "w+")
			make.write("INC = -I/usr/local/Cellar/glm/0.9.8.3/include -I/usr/local/Cellar/freetype/2.7.1/include/freetype2\n" + 
				"FLAGS = -Wall\n" + "LIBS = -framework OpenGL -framework GLUT -lglew -lSOIL -lfreetype\n" +
				"CPP_FILES := $(wildcard *.cpp)\n" + "OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))\n\n" +
				"game: $(OBJ_FILES)\n" + "\tg++ $(FLAGS) $(LIBS) -o $@ $^\n\n" + "obj/%.o: %.cpp\n" +
				"\tg++ $(FLAGS) -c -o $@ $< $(INC)\n\n" + "clean: game\n" + "\trm -f game $(OBJ_FILES)")
			print("Makefile created")

		if not os.path.isdir("obj"):
			os.mkdir("obj")
			print("\'obj\' directory created")


	allFiles = glob.glob('*.cpp') + glob.glob('*.h')
	for path in allFiles:
		file = open(path)
		for i in range(length):
			replaceInFile(path,replace[i],final[i])
	print("Changes applied successfully")







#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Text.h"


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Instructions
{

public:
	Instructions();
	~Instructions();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	int accion;
	int contador;
	Texture instructions, instructions1, instructions2, instructions3, instructions4, instructions5, instructions6, instructions7, instructions8;
	TexturedQuad *fondo, *cursor;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	bool rPressed, rReleased, lPressed, lReleased;
	Text text;

};


#endif // _MENU_INCLUDE

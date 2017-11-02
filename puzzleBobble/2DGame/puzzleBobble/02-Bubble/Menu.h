#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include <irrKlang.h>
#include "Text.h"
#if defined(WIN32)
#include <conio.h>
#else
#include "conio.h"
#endif

#define SCREEN_WIDTH 640.f
#define SCREEN_HEIGHT 480.f


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();
	void setSound(irrklang::ISoundEngine* eng);

private:
	void initShaders();


private:

	ShaderProgram texProgram, simpleProgram;
	float currentTime, angle;
	glm::mat4 projection;
	Quad *quad;
	TexturedQuad *texQuad;
	Texture texs;
	string lvlNumber;
	Text text;

	irrklang::ISoundEngine* engine;
};


#endif // _SCENE_INCLUDE


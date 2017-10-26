#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Arrow.h"

#define SCREEN_WIDTH 640.f
#define SCREEN_HEIGHT 480.f


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Arrow *arrow;
	ShaderProgram texProgram, simpleProgram;
	float currentTime, angle;
	glm::mat4 projection;
	Quad *quad,*techo;
	TexturedQuad *texQuad, *textecho;
	Texture texs,texturetecho;
};


#endif // _SCENE_INCLUDE


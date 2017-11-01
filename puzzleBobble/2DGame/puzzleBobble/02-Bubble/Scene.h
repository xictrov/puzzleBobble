#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Arrow.h"
#include "BolaMapa.h"
#include <irrKlang.h>
#if defined(WIN32)
#include <conio.h>
#else
#include "conio.h"
#endif

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
	void setSound(irrklang::ISoundEngine* eng);

private:
	void initShaders();
	void renderSprites();
	void updateSprites(int deltatime);
	void cleanSprites();
	void compruebaMapa();
	void setNewLvl(int lvl);
	void checkColors();

private:
	TileMap *map;
	Player *player, *playernext;
	Arrow *arrow;
	ShaderProgram texProgram, simpleProgram;
	float currentTime, angle;
	glm::mat4 projection;
	Quad *quad, *techo, *superior;
	TexturedQuad *texQuad, *textecho, *texsuperior;
	Texture texs, texturetecho, texturesuperior;
	vector<BolaMapa *> *mapa;
	string lvlNumber;
	bool winlvl;
	vector<int> ballColors;

	irrklang::ISoundEngine* engine;
};


#endif // _SCENE_INCLUDE


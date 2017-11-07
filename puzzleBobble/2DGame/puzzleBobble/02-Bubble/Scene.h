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
#include "Dino.h"
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


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void Boom();
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
	ArrowMachine *arrow;
	Dino *dino;
	ShaderProgram texProgram, simpleProgram;
	float currentTime, angle;
	glm::mat4 projection;
	Quad *quad, *techo, *superior, *losewin;
	TexturedQuad *texQuad, *textecho, *texsuperior,*texlosewin, *texgameover;
	Texture texs, texturetecho, texturesuperior, texturewin, texturelose, texturegameover;
	vector<BolaMapa *> *mapa;
	string lvlNumber, background, superiorMap, ceiling;
	bool winlvl;
	vector<int> ballColors;
	Text text;
	int lifes;

	irrklang::ISoundEngine* engine;
};


#endif // _SCENE_INCLUDE


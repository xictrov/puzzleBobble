#ifndef _BOLAMAPA_INCLUDE
#define _BOLAMAPA_INCLUDE


#include "Sprite.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class BolaMapa
{

public:

	int color;
	~BolaMapa();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int colorbola);
	void update(int deltaTime, float angle);
	void render();

	void setPosition(const glm::vec2 &pos);

	void explode();

	int getAnimRepetitions();

private:
	bool bJumping, explota;
	glm::ivec2 tileMapDispl, posBolaMapa;
	glm::fvec2 posBolaMapaF;
	int jumpAngle, startY;
	Texture spritesheet, explosionsheet;
	Sprite *sprite, *explosionSprite;

};


#endif // _PLAYER_INCLUDE

#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:

	int color;
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int colorbola, bool &gameover);
	void update(int deltaTime, float angle, bool &cambio, bool &acaba, bool &gameover);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setColor(int col);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	glm::fvec2 posPlayerF;
	int jumpAngle, startY;
	Texture spritesheet, explosionsheet;
	Sprite *sprite, *explosionSprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE

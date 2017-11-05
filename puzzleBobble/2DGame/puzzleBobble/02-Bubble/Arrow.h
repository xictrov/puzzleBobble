#ifndef _ARROWMACHINE_INCLUDE
#define _ARROWMACHINE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class ArrowMachine
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float angle);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posArrow;
	int jumpAngle, startY;
	Texture arrowSpritesheet, gearSpritesheet, wheelSpritesheet, platformSpritesheet, bagSpritesheet;
	Sprite *spriteArrow, *spriteWheel, *spriteGear, *spritePlatform, *spriteBag;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE

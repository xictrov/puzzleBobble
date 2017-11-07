#ifndef _DINO_INCLUDE
#define _DINO_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Dino
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float angle);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	void moveWheel();

private:
	glm::ivec2 tileMapDispl, posArrow;
	Texture dinoSpritesheet;
	Sprite *spriteDinoLeft, *spriteDinoRight;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include "Arrow.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	ARROW
};


void Arrow::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/arrowBig.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(44, 128), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(ARROW, 0);
	sprite->addKeyframe(ARROW, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));
}

void Arrow::update(int deltaTime, float angle)
{
	sprite->update(deltaTime, angle);
}

void Arrow::render()
{
	sprite->render();
}

void Arrow::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Arrow::setPosition(const glm::vec2 &pos)
{
	posArrow = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));
}

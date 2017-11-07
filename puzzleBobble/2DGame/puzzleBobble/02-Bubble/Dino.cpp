#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Dino.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum DinoLeftAnims
{
	STAND_LEFT, MOVE_BALL, BLOW
};

enum DinoRightAnims {
	STAND_RIGHT, MOVE_WHEEL
};


void Dino::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	dinoSpritesheet.loadFromFile("images/dino.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteDinoLeft = Sprite::createSprite(glm::ivec2(46, 38), glm::vec2(1.f/8.f, 1.f/6.f), &dinoSpritesheet, &shaderProgram);
	spriteDinoLeft->setNumberAnimations(1);

	spriteDinoLeft->setAnimationSpeed(STAND_LEFT, 5);
	spriteDinoLeft->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.0f));
	spriteDinoLeft->addKeyframe(STAND_LEFT, glm::vec2(0.0f, 0.0f));
	spriteDinoLeft->addKeyframe(STAND_LEFT, glm::vec2(1.f / 8.f, 0.0f));
	spriteDinoLeft->addKeyframe(STAND_LEFT, glm::vec2(2.f / 8.f, 0.0f));
	spriteDinoLeft->addKeyframe(STAND_LEFT, glm::vec2(2.f / 8.f, 0.0f));
	spriteDinoLeft->addKeyframe(STAND_LEFT, glm::vec2(3.f / 8.f, 0.0f));

	spriteDinoLeft->changeAnimation(0);
	tileMapDispl = tileMapPos;
	spriteDinoLeft->setPosition(glm::vec2(float(260.f), float(420.f)));


	spriteDinoRight = Sprite::createSprite(glm::ivec2(46, 38), glm::vec2(1.f / 8.f, (1.f / 6.f)), &dinoSpritesheet, &shaderProgram);
	spriteDinoRight->setNumberAnimations(2);

	spriteDinoRight->setAnimationSpeed(STAND_RIGHT, 5);
	spriteDinoRight->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, (1.f / 6.f)));
	spriteDinoRight->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, (1.f / 6.f)));
	spriteDinoRight->addKeyframe(STAND_RIGHT, glm::vec2(1.f / 8.f, (1.f / 6.f)));
	spriteDinoRight->addKeyframe(STAND_RIGHT, glm::vec2(2.f / 8.f, (1.f / 6.f)));
	spriteDinoRight->addKeyframe(STAND_RIGHT, glm::vec2(2.f / 8.f, (1.f / 6.f)));
	spriteDinoRight->addKeyframe(STAND_RIGHT, glm::vec2(3.f / 8.f, (1.f / 6.f)));

	spriteDinoRight->setAnimationSpeed(MOVE_WHEEL, 30);
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(0.0f, (2.f / 6.f)));
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(1.f / 8.f, (2.f / 6.f)));
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(2.f / 8.f, (2.f / 6.f)));
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(3.f / 8.f, (2.f / 6.f)));
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(4.f / 8.f, (2.f / 6.f)));
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(5.f / 8.f, (2.f / 6.f)));
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(6.f / 8.f, (2.f / 6.f)));
	spriteDinoRight->addKeyframe(MOVE_WHEEL, glm::vec2(7.f / 8.f, (2.f / 6.f)));


	spriteDinoRight->changeAnimation(0);
	spriteDinoRight->setPosition(glm::vec2(float(360.f), float(420.f)));

}

void Dino::update(int deltaTime, float angle)
{
	spriteDinoLeft->update(deltaTime, angle, true, true);
	spriteDinoRight->update(deltaTime, angle, true, true);
}

void Dino::render()
{
	spriteDinoLeft->render();
	spriteDinoRight->render();
}

void Dino::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Dino::setPosition(const glm::vec2 &pos)
{
	posArrow = pos;
	spriteDinoLeft->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));
}

void Dino::moveWheel()
{
	spriteDinoRight->changeAnimation(MOVE_WHEEL);
}
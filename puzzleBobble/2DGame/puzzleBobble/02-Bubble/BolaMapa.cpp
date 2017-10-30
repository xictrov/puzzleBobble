#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "BolaMapa.h"
#include "Game.h"



#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum PlayerAnims
{
	COLOR, EXPLOSION
};

enum explosionAnims {
	ONE, TWO, THREE, FOUR, FIVE
};

BolaMapa::~BolaMapa() {
	delete sprite;
}

void BolaMapa::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int colorbola)
{
	color = colorbola-1;
	bJumping = false;
	explota = false;
	spritesheet.loadFromFile("images/sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.166666, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(COLOR, 0);
	sprite->addKeyframe(COLOR, glm::vec2(0.f, color / 8.f));

	sprite->setAnimationSpeed(EXPLOSION, 10);
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.166666f, color / 8.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.166666f*2.f, color / 8.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.166666f*3.f, color / 8.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.166666f*4.f, color / 8.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.166666f*5.f, color / 8.f));
	sprite->addKeyframe(EXPLOSION, glm::vec2(0.166666f*6.f, color / 8.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	posBolaMapa = glm::vec2(0.0f);

	posBolaMapaF = glm::vec2(0.0f);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

}

void BolaMapa::update(int deltaTime, float angle)
{
	sprite->update(deltaTime, angle);
}

void BolaMapa::render()
{
	sprite->render();
}

void BolaMapa::setPosition(const glm::vec2 &pos)
{
	posBolaMapa = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBolaMapa.x), float(tileMapDispl.y + posBolaMapa.y)));
}

void BolaMapa::explode()
{
	for (int i=0;i<6;++i){
		sprite->changeAnimation(EXPLOSION);
	}
}

int BolaMapa::getAnimRepetitions()
{
	return sprite->getAnimRepetitions();
}

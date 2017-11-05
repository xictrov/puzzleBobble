#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Arrow.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum ArrowAnims
{
	ARROW
};

enum WheelAnims
{
	WHEEL
};

enum GearAnims
{
	GEAR
};

enum PlatformAnims
{
	PLATFORM
};

enum bagAnims
{
	BAG
};


void ArrowMachine::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	arrowSpritesheet.loadFromFile("images/arrowBig.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteArrow = Sprite::createSprite(glm::ivec2(44, 128), glm::vec2(1, 1), &arrowSpritesheet, &shaderProgram);
	spriteArrow->setNumberAnimations(1);

	spriteArrow->setAnimationSpeed(ARROW, 0);
	spriteArrow->addKeyframe(ARROW, glm::vec2(0.0f, 0.0f));

	spriteArrow->changeAnimation(0);
	tileMapDispl = tileMapPos;
	spriteArrow->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));

	
	wheelSpritesheet.loadFromFile("images/rueda.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteWheel = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125f, 1.0f), &wheelSpritesheet, &shaderProgram);
	spriteWheel->setNumberAnimations(1);

	spriteWheel->setAnimationSpeed(WHEEL, 30);
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f, 1.0f));
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f * 2, 1.0f));
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f * 3, 1.0f));
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f * 4, 1.0f));
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f * 5, 1.0f));
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f * 6, 1.0f));
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f * 7, 1.0f));
	spriteWheel->addKeyframe(WHEEL, glm::vec2(0.125f * 8, 1.0f));

	spriteWheel->changeAnimation(0);
	spriteWheel->setPosition(glm::vec2(float(352.f), float(425.f)));

	gearSpritesheet.loadFromFile("images/engranaje.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteGear = Sprite::createSprite(glm::ivec2(112, 48), glm::vec2(56.f / 672.f, 1.0f), &gearSpritesheet, &shaderProgram);
	spriteGear->setNumberAnimations(1);

	spriteGear->setAnimationSpeed(GEAR, 30);
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f, 1.0f));
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f * 2, 1.0f));
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f * 3, 1.0f));
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f * 4, 1.0f));
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f * 5, 1.0f));
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f * 6, 1.0f));
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f * 7, 1.0f));
	spriteGear->addKeyframe(GEAR, glm::vec2(56.f / 672.f * 8, 1.0f));


	spriteGear->changeAnimation(0);
	spriteGear->setPosition(glm::vec2(float(255.f), float(410.f)));

	platformSpritesheet.loadFromFile("images/platform.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritePlatform = Sprite::createSprite(glm::ivec2(26, 22), glm::vec2(1.0f, 1.0f), &platformSpritesheet, &shaderProgram);
	spritePlatform->setNumberAnimations(1);

	spritePlatform->setAnimationSpeed(PLATFORM, 0);
	spritePlatform->addKeyframe(PLATFORM, glm::vec2(1,1));


	spritePlatform->changeAnimation(0);
	spritePlatform->setPosition(glm::vec2(float(305.f), float(422.f)));

	bagSpritesheet.loadFromFile("images/bag.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteBag = Sprite::createSprite(glm::ivec2(112, 48), glm::vec2(1.0f, 1.0f), &bagSpritesheet, &shaderProgram);
	spriteBag->setNumberAnimations(1);

	spriteBag->setAnimationSpeed(BAG, 0);
	spriteBag->addKeyframe(BAG, glm::vec2(1, 1));


	spriteBag->changeAnimation(0);
	spriteBag->setPosition(glm::vec2(float(180.f), float(410.f)));
}

void ArrowMachine::update(int deltaTime, float angle)
{
	spriteArrow->update(deltaTime, angle, false, true);
	float angleAux = (angle) * (180 / M_PI);
	if (angleAux > -80 && angleAux < 80) {
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			spriteGear->update(deltaTime, 0.0f, true, true);
			spriteWheel->update(deltaTime, 0.0f, true, true);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			spriteGear->update(deltaTime, 0.0f, true, false);
			spriteWheel->update(deltaTime, 0.0f, true, false);
		}
	}
}

void ArrowMachine::render()
{
	spriteArrow->render();
	spriteGear->render();
	spriteWheel->render();
	spritePlatform->render();
	spriteBag->render();
}

void ArrowMachine::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void ArrowMachine::setPosition(const glm::vec2 &pos)
{
	posArrow = pos;
	spriteArrow->setPosition(glm::vec2(float(tileMapDispl.x + posArrow.x), float(tileMapDispl.y + posArrow.y)));
}

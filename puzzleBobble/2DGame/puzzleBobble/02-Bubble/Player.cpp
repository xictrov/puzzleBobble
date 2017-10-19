#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	BLUE, GREY, RED, YELLOW
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/Bolas.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(BLUE, 0);
		sprite->addKeyframe(BLUE, glm::vec2(0.0f, 0.25f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime, 0.0f);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		posPlayer.x -= 2;
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		posPlayer.x += 2;
	}

	if(Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		posPlayer.y -= 2;
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		posPlayer.y += 2;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}





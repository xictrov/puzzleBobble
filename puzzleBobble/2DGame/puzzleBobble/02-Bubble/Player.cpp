#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

bool izq = true;
enum PlayerAnims
{
	COLOR
};

enum explosionAnims {
	ONE, TWO, THREE, FOUR, FIVE
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int colorbola, bool &gameover)
{
	color=colorbola;
	bJumping = false;
	if(!gameover){

		spritesheet.loadFromFile("images/sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	else{

		spritesheet.loadFromFile("images/escalagrises.png", TEXTURE_PIXEL_FORMAT_RGBA);
	}
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.16666, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(COLOR, 0);
	sprite->addKeyframe(COLOR, glm::vec2(0.f, color/8.f));
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	posPlayer = glm::vec2(0.0f);

	posPlayerF = glm::vec2(0.0f);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));


}

void Player::update(int deltaTime, float angle, bool &cambio, bool &acaba, bool &gameover)
{


	sprite->update(deltaTime, angle);

	if (posPlayer.x < -112.5f || posPlayer.x>112.5f ) {
		angle = M_PI - angle;
		cambio = true;
	}

	posPlayerF.x -= cos(angle) * 15;
	posPlayerF.y -= sin(angle) * 15;

	posPlayer.x=int(roundf(posPlayerF.x));
	posPlayer.y=int(roundf(posPlayerF.y));

	acaba = map->collision(posPlayer+tileMapDispl, color, gameover, deltaTime);

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







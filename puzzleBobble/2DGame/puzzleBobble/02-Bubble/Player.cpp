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
	BLUE, GREY, RED, YELLOW
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int colorbola)
{
	color=colorbola;
	cout << "color original" << colorbola << endl;
	bJumping = false;
	spritesheet.loadFromFile("images/Bolas.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(BLUE, 0);
	sprite->addKeyframe(BLUE, glm::vec2(0.f, colorbola/4.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	posPlayer = glm::vec2(0.0f);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

}

void Player::update(int deltaTime, float angle, bool &cambio, bool &acaba)
{


	sprite->update(deltaTime, angle);

	if (posPlayer.x < -112.5f || posPlayer.x>112.5f ) {
		angle = M_PI - angle;
		cambio = true;
	}

	posPlayer.x -= cos(angle) * 10;
	posPlayer.y -= sin(angle) * 10;
	acaba=map->collision(posPlayer+tileMapDispl, color);
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





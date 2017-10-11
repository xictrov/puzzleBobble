#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

bool Game::update(int deltaTime)
{
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
}

void Game::keyReleased(int key)
{
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}




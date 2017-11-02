#include <GL/glew.h>
#include <GLUT/glut.h>
#include "Game.h"
#include "Menu.h"
#include <irrKlang.h>
#include <iostream>
#if defined(WIN32)
#include <conio.h>
#else
#include "conio.h"
#endif


#pragma comment(lib, "irrKlang.lib")

using namespace std;

void Game::init()
{
	Sc=false;
	engine = irrklang::createIrrKlangDevice();
	if (!engine)
	{
		printf("Could not startup engine\n");
		//return 0; // error starting up the engine
	}

	engine->setSoundVolume(0.5f);
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	scene.init();
	scene.setSound(engine);

}

bool Game::update(int deltaTime)
{

		scene.update(deltaTime);
		return bPlay;

}

void Game::render()
{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene.render();

}

void Game::keyPressed(int key)
{

	if(key == 27) // Escape code
		bPlay = false;
	if(key==32)
		Sc=true;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
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

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}






#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <irrKlang.h>
#include <iostream>
#if defined(WIN32)
#include <conio.h>
#else
#include "conio.h"
#endif



void Game::init()
{
	bPlay = true;
	estado=3;
	engine = irrklang::createIrrKlangDevice();
	if (!engine)
	{
		printf("Could not startup engine\n");
		//return 0; // error starting up the engine
	}

	engine->setSoundVolume(0.5f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	menu.init();
	//scene.init();
	instructions.init();
	credits.init();
}

bool Game::update(int deltaTime)
{

	if(estado==3) menu.update(deltaTime);
	else if(estado==0) scene.update(deltaTime);
	else if (estado == 1) instructions.update(deltaTime);
	else credits.update(deltaTime);

	return bPlay;
}

void Game::render()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(estado==3)menu.render();
	else if(estado==0)scene.render();
	else if (estado == 1) instructions.render();
	else credits.render();


}

void Game::keyPressed(int key)
{
	if(key == 27) {// Escape code
		if (estado != 3) {
			estado = 3;
		}
		else {
			bPlay = false;
			cout << "entra" << endl;
		}
	}

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
bool Game::getSpecialKey(int key)
{
	return specialKeys[key];
}


void Game::initSc(){
	scene.setSound(engine);
	scene.init();
}

void Game::newaction(int act)
{

	estado=act;
	if(act==3){
		menu.init();
	}
	else if (act==0){
		initSc();
	}
	else if(act==1) {

	}
	else {
		//glClearColor(1.f, 1.f, 1.f, 1.0f);
	}
}
void Game::setSpecialKey(int key){
	specialKeyReleased(key);
}

void Game::Bomba(){
	scene.Boom();
}






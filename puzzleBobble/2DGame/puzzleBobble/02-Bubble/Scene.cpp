#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <time.h>
#include "Scene.h"
#include "Game.h"

using namespace std;

#define SCREEN_X 192
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

bool cambio = false;
bool acaba = false;
bool empieza = false;
bool gameover = false;
int tiempoDisparo = 0;
int tiempoTecho = 0;
int baja = 0;
float angleAux;

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	srand(time(NULL));
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 640.f, 480.f, simpleProgram);
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	// Load textures
	texs.loadFromFile("images/gameBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texturetecho.loadFromFile("images/techo.png", TEXTURE_PIXEL_FORMAT_RGBA);


	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	mapa = map->convertToSprites();

	player = new Player();
	player->init(glm::ivec2(305.f, 390.f), texProgram,rand()%4);
	//player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	playernext = new Player();
	playernext->init(glm::ivec2(250.f, 425.f), texProgram,rand()%4);
	//player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	playernext->setTileMap(map);

	arrow = new Arrow();
	arrow->init(glm::ivec2(299.f, 340.f), texProgram);
	arrow->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH -1), float(SCREEN_HEIGHT -1), 0.f);
	currentTime = 0.0f;
	angle = 90.0f;


	glm::vec2 geomTecho[2] = { glm::vec2(SCREEN_X, -480.f + SCREEN_Y + baja*32.f), glm::vec2(SCREEN_X + 250, SCREEN_Y + baja*32.f) };
	glm::vec2 texCoordsTecho[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	techo = Quad::createQuad(SCREEN_X, -480.f + SCREEN_Y + (baja + 1) * 32, SCREEN_X + 250, SCREEN_Y + (baja + 1)*32.f, simpleProgram);
	textecho = TexturedQuad::createTexturedQuad(geomTecho, texCoordsTecho, texProgram);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	tiempoDisparo+=1;
	tiempoTecho += 1;
	if(Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		gameover=false;
	}
	if(!gameover){
		if(tiempoTecho%300==0){
			//baja+=1;
			//map->bajaMapa(gameover);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			angle -= 2;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			angle += 2;
		}
		if (angle > 170) angle = 170;
		if (angle < 10) angle = 10;

		float numRadBola = angle * (M_PI / 180);
		float numRadArrow = (angle - 90.f) * (M_PI / 180);

		if(!empieza) arrow->update(deltaTime, numRadArrow);

		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			angleAux = angle;
			tiempoDisparo = 0;
			empieza = true;
		}
		if (tiempoDisparo % 600 == 0) {
			angleAux = angle;
			empieza = true;
		}
		if (empieza) {
			player->update(deltaTime, numRadBola, cambio, acaba, gameover);
			if (cambio) angle = 180 - angle;
			cambio = false;
		}
		if (acaba) {
			player->init(glm::ivec2(305.f, 390.f), texProgram, playernext->color);
			player->setTileMap(map);
			empieza = false;
			acaba = false;
			angle = angleAux;
			playernext->init(glm::ivec2(250.f, 425.f), texProgram, rand() % 4);
			playernext->setTileMap(map);
		}
		if(gameover) {
				baja=0;
				cambio = false;
				acaba = false;
				empieza = false;
				tiempoDisparo = 0;
				tiempoTecho = 0;
				map->free();
				map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				mapa = map->convertToSprites();
				playernext->init(glm::ivec2(250.f, 425.f), texProgram,rand()%4);
				playernext->setTileMap(map);
				player->init(glm::ivec2(305.f, 390.f), texProgram,rand()%4);
				player->setTileMap(map);
		}
	}
	updateSprites(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview(1.f);

	//simpleProgram.use();
	//simpleProgram.setUniformMatrix4f("projection", projection);
	//simpleProgram.setUniform4f("color", 0.2f, 0.2f, 0.8f, 1.0f);

	//simpleProgram.setUniformMatrix4f("modelview", modelview);
	//quad->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	texQuad->render(texs);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	techo->free();
	textecho->free();

	//glm::vec2 geomTecho[2] = { glm::vec2(SCREEN_X, -480.f+SCREEN_Y+baja*32.f), glm::vec2(SCREEN_X+256, SCREEN_Y+baja*32.f) };
	//glm::vec2 texCoordsTecho[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	//techo = Quad::createQuad(SCREEN_X, -480.f+SCREEN_Y+(baja+1)*32, SCREEN_X+250, SCREEN_Y+(baja+1)*32.f, simpleProgram);
	//textecho = TexturedQuad::createTexturedQuad(geomTecho, texCoordsTecho, texProgram);
	// Load textures
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	textecho->render(texturetecho);

	//map->render();

	renderSprites();

	arrow->render();

	player->render();

	playernext->render();


}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::renderSprites()
{
	glm::ivec2 mapSize = map->getMapSize();
	for (int j = 0; j < mapSize.y; ++j) {
		for (int i = 0; i < mapSize.x; ++i) {
			if ((*mapa)[j*mapSize.x + i] != NULL) {
				if((*mapa)[j*mapSize.x+i]->getAnimRepetitions()==1) {
					delete (*mapa)[j*mapSize.x+i];
					(*mapa)[j*mapSize.x + i] = NULL;
				}
				else {
					(*mapa)[j*mapSize.x + i]->render();
				}
			}
			//if ((*mapa)[j*mapSize.x + i]->getAnimRepetitions() == 1) delete (*mapa)[j*mapSize.x + i];
		}
	}
}

void Scene::updateSprites(int deltaTime)
{
	int contador=0;
	glm::ivec2 mapSize = map->getMapSize();
	for (int j = 0; j < mapSize.y; ++j) {
		for (int i = 0; i < mapSize.x; ++i) {
			if ((*mapa)[j*mapSize.x + i] != NULL) {
				if((*mapa)[j*mapSize.x+i]->getAnimRepetitions()==1) {
					delete (*mapa)[j*mapSize.x+i];
					(*mapa)[j*mapSize.x + i] = NULL;
				}
				else{
					(*mapa)[j*mapSize.x + i]->update(deltaTime,0.0f);
				}
			}
 			//if ((*mapa)[j*mapSize.x + i]->getAnimRepetitions() == 1) delete (*mapa)[j*mapSize.x + i];
		}
	}
}

void Scene::cleanSprites() 
{
		glm::ivec2 mapSize = map->getMapSize();
	for (int j = 0; j < mapSize.y; ++j) {
		for (int i = 0; i < mapSize.x; ++i) {
			if ((*mapa)[j*mapSize.x + i] != NULL) {
					delete (*mapa)[j*mapSize.x+i];
					(*mapa)[j*mapSize.x + i] = NULL;
			}
			//if ((*mapa)[j*mapSize.x + i]->getAnimRepetitions() == 1) delete (*mapa)[j*mapSize.x + i];
		}
	}
}




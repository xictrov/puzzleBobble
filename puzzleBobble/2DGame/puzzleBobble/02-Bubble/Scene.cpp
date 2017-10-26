#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include "Scene.h"
#include "Game.h"

#define SCREEN_X 192
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

bool cambio = false;
bool acaba = false;
bool empieza = false;
bool gameover=false;
int tiempo=0;
int baja=0;

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


	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(305.f, 390.f), texProgram,rand()%4);
	//player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	playernext = new Player();
	playernext->init(glm::ivec2(250.f, 390.f), texProgram,rand()%4);
	//player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	playernext->setTileMap(map);

	arrow = new Arrow();
	arrow->init(glm::ivec2(299.f, 340.f), texProgram);
	arrow->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH -1), float(SCREEN_HEIGHT -1), 0.f);
	currentTime = 0.0f;
	angle = 90.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	tiempo+=1;
	if(Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		gameover=false;
	}
	if(!gameover){

		
		if(tiempo%200==0){
			baja+=1;
			map->BajaMapa(gameover);
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			angle -= 1;
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			angle += 1;
		}
		if (angle > 170) angle = 170;
		if (angle < 10) angle = 10;

		float numRadBola = angle * (M_PI / 180);
		float numRadArrow = (angle - 90.f) * (M_PI / 180);
		if(!empieza)
		arrow->update(deltaTime, numRadArrow);


		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
			empieza = true;
		}
		if (empieza) {
			player->update(deltaTime, numRadBola, cambio, acaba,gameover);
			if (cambio) angle = 180 - angle;
			cambio = false;	
		}

		if (acaba) {
			player->init(glm::ivec2(305.f,390.f), texProgram,playernext->color);
			player->setTileMap(map);
			empieza = false;
			acaba = false;
			angle=90.0f;
			playernext->init(glm::ivec2(250.f, 390.f), texProgram,rand()%4);
			playernext->setTileMap(map);

		}
		if(tiempo%300==0){
			empieza=true;
			player->update(deltaTime, numRadBola, cambio,acaba,gameover);
			if (cambio) angle = 180 - angle;
			cambio = false;	

		}

		if(gameover){
				baja=0;		
				cambio = false;
				acaba = false;
				empieza = false;
				map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
				playernext->init(glm::ivec2(250.f, 390.f), texProgram,rand()%4);
				playernext->setTileMap(map);
				player->init(glm::ivec2(305.f, 390.f), texProgram,rand()%4);
				player->setTileMap(map);



		}
	}


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


	glm::vec2 geom[2] = { glm::vec2(SCREEN_X, -480.f+SCREEN_Y+baja*32.f), glm::vec2(SCREEN_X+250, SCREEN_Y+baja*32.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	techo = Quad::createQuad(SCREEN_X, -480.f+SCREEN_Y+(baja+1)*32, SCREEN_X+250, SCREEN_Y+(baja+1)*32.f, simpleProgram);
	textecho = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	// Load textures
	texturetecho.loadFromFile("images/Bolas.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	textecho->render(texturetecho);

	map->render();

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




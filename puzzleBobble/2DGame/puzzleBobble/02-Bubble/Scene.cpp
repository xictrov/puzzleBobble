#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <time.h>
#include "Scene.h"
#include "Game.h"
#include <string>
#include <irrKlang.h>

using namespace std;


#define SCREEN_X 192
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

bool cambio = false;
bool acaba = false;
bool empieza = false;
bool gameover = false;
int contadorNivel = 1;
int tiempoDisparo = 0;
int tiempoTecho = 0;
int baja = 0;
float angleAux;
int gameover_sonido=0;
bool first;

float numRadBola;
float numRadArrow;

enum State {
	THROWING_BALL, LVL_WON, LVL_LOST, WAITING_FOR_THROW, GAME_LOST, GAME_WIN
};

State state;

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


	//engine->play2D("./sounds/pbobble-025.wav", false);
	engine->play2D("./sounds/smash_mouth-all_star.wav", false);
	state = WAITING_FOR_THROW;

	srand(time(NULL));
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 640.f, 480.f, simpleProgram);
	texQuad = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	lvlNumber = "levels/level01.txt";
	background = "images/gameBackground1.png";
	superiorMap = "images/parteSuperiorMapa1.png";
	ceiling = "images/techo1.png";

	// Load textures

	texs.loadFromFile(background, TEXTURE_PIXEL_FORMAT_RGBA);
	texturesuperior.loadFromFile(superiorMap, TEXTURE_PIXEL_FORMAT_RGBA);
	texturetecho.loadFromFile(ceiling, TEXTURE_PIXEL_FORMAT_RGBA);

	texturelose.loadFromFile("images/youlose.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texturewin.loadFromFile("images/youwin.png", TEXTURE_PIXEL_FORMAT_RGBA);

	texturegameover.loadFromFile("images/gameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
	

	map = TileMap::createTileMap(lvlNumber, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	mapa = map->convertToSprites(gameover);

	checkColors();

	player = new Player();
	if (ballColors.size() > 0) {
		player->init(glm::ivec2(305.f, 390.f), texProgram, ballColors[rand() % ballColors.size()],gameover);
		player->setTileMap(map);
	}
	playernext = new Player();
	if (ballColors.size() > 0) {
		playernext->init(glm::ivec2(235.f, 426.f), texProgram, ballColors[rand() % ballColors.size()],gameover);
		playernext->setTileMap(map);
	}

	arrow = new ArrowMachine();
	arrow->init(glm::ivec2(299.f, 340.f), texProgram);
	arrow->setTileMap(map);

	dino = new Dino();
	dino->init(glm::ivec2(250.f, 400.f), texProgram);
	dino->setTileMap(map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH -1), float(SCREEN_HEIGHT -1), 0.f);
	currentTime = 0.0f;
	angle = 90.0f;

	winlvl = false;
	lives = 3;

	glm::vec2 geomTecho[2] = { glm::vec2(SCREEN_X, -480.f + SCREEN_Y + baja*32.f), glm::vec2(SCREEN_X + 250, SCREEN_Y + baja*32.f) };
	glm::vec2 texCoordsTecho[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	techo = Quad::createQuad(SCREEN_X, -480.f + SCREEN_Y + (baja + 1) * 32, SCREEN_X + 250, SCREEN_Y + (baja + 1)*32.f, simpleProgram);
	textecho = TexturedQuad::createTexturedQuad(geomTecho, texCoordsTecho, texProgram);

	glm::vec2 geomSuperior[2] = { glm::vec2(SCREEN_X, 0), glm::vec2(SCREEN_X + 256, SCREEN_Y) };
	glm::vec2 texCoordsSuperior[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	superior = Quad::createQuad(SCREEN_X, 0, SCREEN_X + 256, SCREEN_Y, simpleProgram);
	texsuperior = TexturedQuad::createTexturedQuad(geomSuperior, texCoordsSuperior, texProgram);

	if(!text.init("fonts/emulogic.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::update(int deltaTime) {

	map->setSound(engine);
	currentTime += deltaTime;
	tiempoDisparo += 1;
	tiempoTecho += 1;
	compruebaMapa();

	if (gameover) state = LVL_LOST;

	if (winlvl) state = LVL_WON;

	if (lives < 0) state = GAME_LOST;

	switch (state) {
		case (WAITING_FOR_THROW):

			if (tiempoTecho % 3000 == 0) {
				baja += 1;
				map->bajaMapa(gameover);
				cleanSprites();
				mapa = map->convertToSprites(gameover);
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

			numRadArrow = (angle - 90.f) * (M_PI / 180);

			arrow->update(deltaTime, numRadArrow);

			if (Game::instance().getSpecialKey(GLUT_KEY_UP) || tiempoDisparo % 5000 == 0) {
				engine->play2D("./sounds/pbobble-002.wav", false);
				tiempoDisparo = 0;
				angleAux = angle;
				state = THROWING_BALL;
				Game::instance().setSpecialKey(GLUT_KEY_UP);
			}

			dino->update(deltaTime, 0.0f);

			break;
		case (THROWING_BALL):

			numRadBola = angle * (M_PI / 180);

			player->update(deltaTime, numRadBola, cambio, acaba, gameover);
			if (cambio) angle = 180 - angle;
			cambio = false;

			if (acaba) {
				state = WAITING_FOR_THROW;
				player->init(glm::ivec2(305.f, 390.f), texProgram, playernext->color, gameover);
				player->setTileMap(map);
				acaba = false;
				angle = angleAux;
				checkColors();
				if (ballColors.size() > 0) {
					playernext->init(glm::ivec2(235.f, 426.f), texProgram, ballColors[rand() % ballColors.size()], gameover);
					playernext->setTileMap(map);
				}
				if (gameover) first = true;
			}

			break;
		case (LVL_WON):


			if (gameover_sonido == 0) {
				engine->stopAllSounds();
				engine->play2D("./sounds/allstar.wav", false);
			}
			++gameover_sonido;

			if(Game::instance().getKey(13)){
				++contadorNivel;
				setNewLvl(contadorNivel);

			}


			break;
		case (LVL_LOST):

			//player->init(glm::ivec2(305.f, 390.f), texProgram, player->color, gameover);
			//player->setTileMap(map);
			//playernext->init(glm::ivec2(250.f, 425.f), texProgram, ballColors[playernext->color], gameover);
			//playernext->setTileMap(map);

			if (first) {
				cleanSprites();
				mapa = map->convertToSprites(gameover);
				first = false;
			}
			if (gameover_sonido == 0) {
				engine->stopAllSounds();
				engine->play2D("./sounds/pbobble-041.wav", false);
			}
			++gameover_sonido;

			if (Game::instance().getKey(13))
			{
				setNewLvl(contadorNivel);
				first = true;
				--lives;
			}
			
			break;
		case (GAME_LOST) :

			engine->stopAllSounds();
			engine->play2D("./sounds/pbobble-041.wav", false);
			break;
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


	glm::vec2 geomTecho[2] = { glm::vec2(SCREEN_X, -480.f + SCREEN_Y + baja*32.f), glm::vec2(SCREEN_X + 256, SCREEN_Y + baja*32.f) };
	glm::vec2 texCoordsTecho[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	techo = Quad::createQuad(SCREEN_X, -480.f + SCREEN_Y + (baja + 1) * 32, SCREEN_X + 250, SCREEN_Y + (baja + 1)*32.f, simpleProgram);
	textecho = TexturedQuad::createTexturedQuad(geomTecho, texCoordsTecho, texProgram);
	// Load textures
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	textecho->render(texturetecho);

	techo->free();
	textecho->free();


	glm::vec2 geomSuperior[2] = { glm::vec2(SCREEN_X, 0), glm::vec2(SCREEN_X + 256, SCREEN_Y) };
	glm::vec2 texCoordsSuperior[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	superior = Quad::createQuad(SCREEN_X, 0, SCREEN_X + 256, SCREEN_Y, simpleProgram);
	texsuperior = TexturedQuad::createTexturedQuad(geomSuperior, texCoordsSuperior, texProgram);

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	texsuperior->render(texturesuperior);

	superior->free();
	texsuperior->free();

	//map->render();

	arrow->render();

	dino->render();

	player->render();

	playernext->render();

	renderSprites();

	if(state==LVL_LOST) {
		text.render("Score: ", glm::vec2(192, 300), 14, glm::vec4(1, 1, 1, 1));

		text.render(to_string(map->getPuntuacion()), glm::vec2(380, 300), 20, glm::vec4(1, 1, 1, 1));


		glm::vec2 geomlosewin[2] = { glm::vec2(192, 100), glm::vec2(192 + 250, 160) };
		glm::vec2 texCoordslosewin[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		losewin = Quad::createQuad(192, 50, 192 + 250, 110, simpleProgram);
		texlosewin = TexturedQuad::createTexturedQuad(geomlosewin, texCoordslosewin, texProgram);

		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		texlosewin->render(texturelose);

		losewin->free();
		texlosewin->free();

		if (int(tiempoTecho % 10)>3) { text.render("press enter to continue", glm::vec2(192, 240), 10, glm::vec4(1, 1, 1, 1)); }
	}


	else if(state==LVL_WON) {
		text.render("Score: ", glm::vec2(220, 300), 14, glm::vec4(1, 1, 1, 1));

		text.render(to_string(map->getPuntuacion()), glm::vec2(340, 300),20, glm::vec4(1, 1, 1, 1));

		glm::vec2 geomlosewin[2] = { glm::vec2(192, 100), glm::vec2(192+250, 160) };
		glm::vec2 texCoordslosewin[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		losewin = Quad::createQuad(192,50,192+250,110, simpleProgram);
		texlosewin = TexturedQuad::createTexturedQuad(geomlosewin, texCoordslosewin, texProgram);

		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		texlosewin->render(texturewin);

		losewin->free();
		texlosewin->free();

		if(int(tiempoTecho%10)>3) {
			text.render("press enter to next level", glm::vec2(176, 240),10, glm::vec4(1, 1, 1, 1));
		}
	}
	else if (state == GAME_LOST) {

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
		glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		texgameover = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

		texProgram.use();
		texProgram.setUniformMatrix4f("projection", projection);
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		texgameover->render(texturegameover);
	}
	else {
		text.render("Score: ", glm::vec2(40, 30), 14, glm::vec4(1, 1, 1, 1));

		text.render(to_string(map->getPuntuacion()), glm::vec2(200, 30), 16, glm::vec4(1, 1, 1, 1));

		text.render("Level: ", glm::vec2(SCREEN_X+280, SCREEN_Y+5), 14, glm::vec4(1, 1, 1, 1));

		text.render(to_string(contadorNivel), glm::vec2(SCREEN_X+380, SCREEN_Y+5), 14, glm::vec4(1, 1, 1, 1));

		text.render(to_string(int(tiempoTecho/60)), glm::vec2(500, 100),16, glm::vec4(1, 1, 1, 1));
	}

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

void Scene::compruebaMapa()
{
	winlvl = map->lvlClear();
}

void Scene::setNewLvl(int lvl)
{
	state=WAITING_FOR_THROW;
	char number = lvl + '0';
	lvlNumber[13] = number;
	gameover_sonido=0;
	baja = 0;
	cambio = false;
	acaba = false;
	empieza = false;
	gameover = false;
	tiempoDisparo = 0;
	tiempoTecho = 0;

	if (lvl == 3) {
		background[21] = number;
		superiorMap[24] = number;
		ceiling[12] = number;
		texs.loadFromFile(background, TEXTURE_PIXEL_FORMAT_RGBA);
		texturesuperior.loadFromFile(superiorMap, TEXTURE_PIXEL_FORMAT_RGBA);
		texturetecho.loadFromFile(ceiling, TEXTURE_PIXEL_FORMAT_RGBA);
	}

	delete map;
	map = TileMap::createTileMap(lvlNumber, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	mapa = map->convertToSprites(gameover);
	if(state==LVL_LOST)
	map->setPuntuacion(0);

	engine->stopAllSounds();
	engine->play2D("./sounds/smash_mouth-all_star.wav", false);
	checkColors();
	if (ballColors.size() > 0) {
		player->init(glm::ivec2(305.f, 390.f), texProgram, ballColors[rand() % ballColors.size()],gameover);
		player->setTileMap(map);
	}
	if (ballColors.size() > 0) {
		playernext->init(glm::ivec2(235.f, 426.f), texProgram, ballColors[rand() % ballColors.size()],gameover);
		playernext->setTileMap(map);
	}

}

void Scene::checkColors()
{
	set<int> colors = map->checkColors();
	if (colors.size() != ballColors.size()) {
		ballColors.resize(colors.size());
		ballColors.assign(colors.begin(), colors.end());
	}
}

void Scene::setSound(irrklang::ISoundEngine* eng) {
	engine = eng;
}

void Scene::Boom(){
	playernext->init(glm::ivec2(250.f, 425.f), texProgram, 20, gameover);
	//playernext->setColor(20);
}


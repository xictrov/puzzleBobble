#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Instructions.h"
#include "Game.h"
#include <GL/glut.h>


#define SCREEN_X 32
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

Instructions::Instructions()
{

}

Instructions::~Instructions()
{

}


void Instructions::init()
{
	initShaders();
	currentTime = 0.0f;
	accion = 0;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	fondo = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	instructions1.loadFromFile("images/instructions1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions2.loadFromFile("images/instructions2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions3.loadFromFile("images/instructions3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions4.loadFromFile("images/instructions4.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions5.loadFromFile("images/instructions5.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions6.loadFromFile("images/instructions6.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions7.loadFromFile("images/instructions7.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions8.loadFromFile("images/instructions8.png", TEXTURE_PIXEL_FORMAT_RGBA);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	instructions = instructions1;

	rPressed = false;
	rReleased = false;
	lPressed = false;
	lReleased = false;


	contador = 1;

	if (!text.init("fonts/emulogic.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

}

void Instructions::update(int deltaTime)
{
	currentTime += deltaTime;

	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) rPressed = true;
	else if (rPressed) rReleased = true;
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) lPressed = true;
	else if (lPressed) lReleased = true;

	if (rReleased) {
		rPressed = rReleased = false;
		++contador;
		if (contador > 8) contador = 8;
	}
	if (lReleased) {
		lPressed = lReleased = false;
		--contador;
		if (contador < 1) contador = 1;
	}

	switch (contador) {
	case (1) :
		instructions = instructions1;
		break;
	case (2) :
		instructions = instructions2;
		break;
	case (3) :
		instructions = instructions3;
		break;
	case (4) :
		instructions = instructions4;
		break;
	case (5) :
		instructions = instructions5;
		break;
	case (6) :
		instructions = instructions6;
		break;
	case (7) :
		instructions = instructions7;
		break;
	case (8) :
		instructions = instructions8;
		break;
	}
}

void Instructions::render()
{

	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	fondo->render(instructions);

	text.render("Press ESC to go to the MENU screen", glm::vec2(240, 450), 10, glm::vec4(1, 1, 1, 1));
}


void Instructions::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

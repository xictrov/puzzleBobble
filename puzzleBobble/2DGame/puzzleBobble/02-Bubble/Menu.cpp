#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>


#define SCREEN_X 32
#define SCREEN_Y 32

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

int currentTime_ant=0;
int contador=0;

Menu::Menu()
{

}

Menu::~Menu()
{

}


void Menu::init()
{
	initShaders();
	currentTime = 0.0f;
	accion=0;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	fondo = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	imgFondo.loadFromFile("images/logoscreen.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(32.f, 32.f) };
	glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);
	imgCursor.loadFromFile("images/Cursor.png", TEXTURE_PIXEL_FORMAT_RGBA);


	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	if(!texto.init("fonts/emulogic.ttf"))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;


}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	++contador;

	if(Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
			Game::instance().setSpecialKey(GLUT_KEY_DOWN);
			accion+=1;
			accion=accion%3;

	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_UP)){
			Game::instance().setSpecialKey(GLUT_KEY_UP);
			accion+=2;
			accion=accion%3;


	}
	if(accion==0){


		texto.render("Play ", glm::vec2(260, 380), 20, glm::vec4(1, 1, 1, 1));

		texto.render("How to play ", glm::vec2(200, 420), 14, glm::vec4(1, 1, 1, 1));

		texto.render("Credits ", glm::vec2(240, 460), 14, glm::vec4(1, 1, 1, 1));

		glm::vec2 geom2[2] = { glm::vec2(350.f, 370.f), glm::vec2(350+32.f, 370+32.f) };
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);

	}
	else if(accion==1){

		texto.render("Play ", glm::vec2(270, 380), 14, glm::vec4(1, 1, 1, 1));

		texto.render("How to play ", glm::vec2(160, 420), 20, glm::vec4(1, 1, 1, 1));

		texto.render("Credits ", glm::vec2(240, 460), 14, glm::vec4(1, 1, 1, 1));

		glm::vec2 geom2[2] = { glm::vec2(430.f, 410.f), glm::vec2(430+32.f, 410+32.f) };
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);

	}
	else if (accion==2){


		texto.render("Play ", glm::vec2(270, 380), 14, glm::vec4(1, 1, 1, 1));

		texto.render("How to play ", glm::vec2(200, 420), 14, glm::vec4(1, 1, 1, 1));

		texto.render("Credits ", glm::vec2(215, 460), 20, glm::vec4(1, 1, 1, 1));

		glm::vec2 geom2[2] = { glm::vec2(380.f, 450.f), glm::vec2(380+32.f, 450+32.f) };
		glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
		cursor = TexturedQuad::createTexturedQuad(geom2, texCoords2, texProgram);

	}
	if (Game::instance().getKey(13)) {
		Game::instance().newaction(accion);
	}


}

void Menu::render()
{

	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	fondo->render(imgFondo);

	if(accion==0){


		texto.render("Play ", glm::vec2(260, 380), 20, glm::vec4(1, 1, 1, 1));

		texto.render("How to play ", glm::vec2(220, 420), 14, glm::vec4(1, 1, 1, 1));

		texto.render("Credits ", glm::vec2(250, 460), 14, glm::vec4(1, 1, 1, 1));


	}
	else if(accion==1){

		texto.render("Play ", glm::vec2(270, 380), 14, glm::vec4(1, 1, 1, 1));

		texto.render("How to play ", glm::vec2(180, 420), 20, glm::vec4(1, 1, 1, 1));

		texto.render("Credits ", glm::vec2(250, 460), 14, glm::vec4(1, 1, 1, 1));


	}
	else if (accion==2){


		texto.render("Play ", glm::vec2(270, 380), 14, glm::vec4(1, 1, 1, 1));

		texto.render("How to play ", glm::vec2(220, 420), 14, glm::vec4(1, 1, 1, 1));

		texto.render("Credits ", glm::vec2(220, 460), 20, glm::vec4(1, 1, 1, 1));


	}



	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	cursor->render(imgCursor);


}


void Menu::initShaders()
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



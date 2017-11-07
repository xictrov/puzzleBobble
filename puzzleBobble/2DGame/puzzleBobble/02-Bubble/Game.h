#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "Instructions.h"
#include "Credits.h"
#include <irrKlang.h>
#if defined(WIN32)
#include <conio.h>
#else
#include "conio.h"
#endif

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}


	static Game &instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	bool getSpecialKey(int key);

	void setSpecialKey(int key);
	void initSc();
	void newaction(int act);
	void Bomba();

private:
	bool bPlay;                       // Continue to play game?
	Menu menu;
	Instructions instructions;
	Credits credits;
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that
	                                  // we can have access at any time
	int estado;
	bool bPulsed[256];
	irrklang::ISoundEngine* engine;

};


#endif // _GAME_INCLUDE



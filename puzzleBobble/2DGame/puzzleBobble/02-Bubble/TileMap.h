#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include <set>
#include "BolaMapa.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <irrKlang.h>
#if defined(WIN32)
#include <conio.h>
#else
#include "conio.h"
#endif


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSize() const { return mapSize; }
	bool collision(const glm::ivec2 &pos,int color, bool &gameover, int currentTime);
	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	void colocaBola(int i, int j, int color, float Bolax, float Bolay, bool &gameover);
	void colocaBolaTecho(int i, int j, int color, bool &gameover);
	void bajaMapa(bool &gameover);
	void searchBallsToDestroy(int j, int i);
	void searchBallsToDestroy2(int j, int i);

	void deleteBalls(std::vector<glm::ivec2> &positions, bool &gameover);
	void searchAloneBalls(int j, int i);
	void deleteAloneBalls(bool &gameover);
	vector<BolaMapa *> * convertToSprites(bool &gameover);
	bool lvlClear();
	set<int> checkColors();

	void setSound(irrklang::ISoundEngine* eng);
	int getPuntuacion();
	void setPuntuacion(int punt);

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program, bool &gameover);
	void clearVectors();
	void search(int j, int i, bool &gameover, int color);
	void addSprite(int j, int i, int color, bool &gameover);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	ShaderProgram program;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	glm::vec2 minCoords;
	string level;
	int *tileMap;
	bool *visitedMap;
	std::vector<glm::ivec2> ballsToDestroy;
	std::vector<BolaMapa *> spriteMap;
	int deltaTime;
	irrklang::ISoundEngine* engine;

};


#endif // _TILE_MAP_INCLUDE



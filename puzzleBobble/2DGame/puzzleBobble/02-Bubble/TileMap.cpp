#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Game.h"


using namespace std;
int bajada=0;
bool primero = true;
int puntuacion = 0;
int puntuacion2 = 1;
bool bomba=false;

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	this->minCoords=minCoords;
	this->program=program;
	this->level=levelFile;
	bool aux=false;
	prepareArrays(minCoords, program,aux);
}

TileMap::~TileMap()
{
	if(tileMap != NULL)
		delete tileMap;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	tileMap = new int[mapSize.x * mapSize.y];
	visitedMap = new bool[mapSize.x * mapSize.y];
	spriteMap.resize(mapSize.x * mapSize.y);

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			visitedMap[j*mapSize.x + i] = false;
		}
	}
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				tileMap[j*mapSize.x+i] = 0;
			else
				tileMap[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
	}
	fin.close();
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program, bool &gameover)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = tileMap[j * mapSize.x + i];
			if(tile != 0)
			{
				if(j+bajada >= 10) {
					gameover=true;
					bajada=0;
				}
				// Non-empty tile
				nTiles++;

				if(j%2==0)
					posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				else
					posTile = glm::vec2(minCoords.x + i * tileSize+tileSize/2, minCoords.y + j * tileSize);


				texCoordTile[0] = glm::vec2(0, float((tile-1)) / 8.f);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(tileMap[y*mapSize.x+x] != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(tileMap[y*mapSize.x+x] != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(tileMap[y*mapSize.x+x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collision(const glm::ivec2 &pos,int color, bool &gameover, int currentTime)
{

	deltaTime = currentTime;
	for (int j=0;j<mapSize.y;++j){
		for (int i=0;i<mapSize.x;++i){

			int tile= tileMap[j*mapSize.x + i];
			if(tile!=0){

				glm::vec2 posTile;
				if(j%2==0){
					posTile=glm::vec2(minCoords.x+i*tileSize,minCoords.y+j*tileSize);
				}
				else{
					posTile=glm::vec2(minCoords.x+i*tileSize+tileSize/2,minCoords.y+j*tileSize);
				}
				float BolaJugadax=pos.x+tileSize/2.f;
				float BolaJugaday=pos.y+tileSize/2.f;
				float BolaMapax=posTile.x+tileSize/2.f;
				float BolaMapay=posTile.y+tileSize/2.f;

				double dist=(sqrt(pow(abs(BolaMapax-BolaJugadax),2)+pow(abs(BolaMapay-BolaJugaday),2)));
				if(dist<=32){
					colocaBola(j,i,color,BolaJugadax,BolaJugaday,gameover);
					return true;

				}
				if(BolaJugaday<60+bajada*32){
					if(color!=20){
						colocaBolaTecho(0,(BolaJugadax-193.f)/32.f,color,gameover);
					}
					return true;
				}
			}
		}
	}
	return false;
}

void TileMap::colocaBola(int j, int i, int color, float Bolax, float Bolay, bool &gameover)
{
	double dist_anterior = 100.0f;
	int posfx = 0;
	int posfy = 0;
	int par=0;
	if(color!=20){
		if(j%2!=0){
			par=-1;
		}
		else{
			par=1;
		}
		for (int jj = -1; jj < 2; ++jj) {
			for (int ii = -1; ii < 2; ++ii) {
				if (!(jj == 0 && ii == 0) && !(jj == -1 && ii == par) && !(jj == 1 && ii == par) && (((j+jj)%2==0) || ((j+jj%2!=0 && (i+ii)<7)))) {

							int tile = tileMap[(j + jj)*mapSize.x + (i + ii)];
							if (tile == 0) {
								glm::vec2 posTile;
								if ((jj+j) % 2 == 0) {
									posTile = glm::vec2(minCoords.x + (i + ii)*tileSize, minCoords.y + (j + jj)*tileSize);
								}
								else {
									posTile = glm::vec2(minCoords.x + (i + ii)*tileSize + tileSize / 2, minCoords.y + (j + jj)*tileSize);
								}

								int BolaMapax = posTile.x + tileSize / 2;
								int BolaMapay = posTile.y + tileSize / 2;

								double dist = (sqrt(pow(abs(BolaMapax - Bolax), 2) + pow(abs(BolaMapay - Bolay), 2)));
								if (dist_anterior > dist) {
										dist_anterior = dist;
										posfy = j + jj;
										posfx = i + ii;
								}
							}
						}
					}
				}
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);

		tileMap[posfy*mapSize.x + posfx] = color+1;

		addSprite(posfy, posfx, color,gameover);

		if (!lvlClear()) prepareArrays(minCoords, program,gameover);

		search(posfy,posfx,gameover,color);
	}
	else{

		if(j%2!=0){
			par=-1;
		}
		else{
			par=1;
		}
		for (int jj = -1; jj < 2; ++jj) {
			for (int ii = -1; ii < 2; ++ii) {
				if (!(jj == 0 && ii == 0) && !(jj == -1 && ii == par) && !(jj == 1 && ii == par) && (((j+jj)%2==0) || ((j+jj%2!=0 && (i+ii)<7)))) {

							int tile = tileMap[(j + jj)*mapSize.x + (i + ii)];
							if (tile == 0) {
								glm::vec2 posTile;
								if ((jj+j) % 2 == 0) {
									posTile = glm::vec2(minCoords.x + (i + ii)*tileSize, minCoords.y + (j + jj)*tileSize);
								}
								else {
									posTile = glm::vec2(minCoords.x + (i + ii)*tileSize + tileSize / 2, minCoords.y + (j + jj)*tileSize);
								}

								float BolaMapax = posTile.x + tileSize / 2.f;
								float BolaMapay = posTile.y + tileSize / 2.f;

								double dist = (sqrt(pow(abs(BolaMapax - Bolax), 2) + pow(abs(BolaMapay - Bolay), 2)));
								if (dist_anterior > dist) {
										dist_anterior = dist;
										posfy = j + jj;
										posfx = i + ii;
								}
							}
						}
					}
				}

		if(posfy%2!=0){
			par=-1;
		}
		else{
			par=1;
		}
		for (int jj = -1; jj < 2; ++jj) {
			for (int ii = -1; ii < 2; ++ii) {
				if (!(jj == 0 && ii == 0) && !(jj == -1 && ii == par) && !(jj == 1 && ii == par) && (((j+jj)%2==0) || ((j+jj%2!=0 && (i+ii)<7)))) {

					int tile = tileMap[(posfy + jj)*mapSize.x + (posfx + ii)];
					if (tile != 0) {
						search((posfy+jj),(posfx+ii),gameover,color);
						//tileMap[(posfy + jj)*mapSize.x + (posfx + ii)] = 0;
						//spriteMap[(posfy + jj)*mapSize.x + (posfx + ii)]->explode();
					}
				}
			}
		}

		engine->play2D("./sounds/boom.wav", false);

		clearVectors();

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		if (!lvlClear()) prepareArrays(minCoords, program, gameover);


	}
}


void TileMap::colocaBolaTecho(int j, int i, int color, bool &gameover)
{

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	tileMap[j*mapSize.x + i] = color+1;

	addSprite(j, i, color,gameover);

	bool aux=false;

	if (!lvlClear()) prepareArrays(minCoords, program, aux);

	search(j, i, gameover,color);
}


void TileMap::bajaMapa(bool &gameover){
	bajada+=1;
	minCoords.y+=32;
	if (gameover) {
		bajada = 0;
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	if (!lvlClear()) prepareArrays(minCoords, program, gameover);
}

void TileMap::searchBallsToDestroy(int j, int i)
{
	static vector<pair<int, int> > positions = {
		{ -1, -1 },
		{ -1,  0 },
		{ 0, -1 },
		{ 0,  1 },
		{ 1,  0 },
		{ 1, -1 },
	};
	int ni, nj;
	glm::ivec2 vecino;

	int color = tileMap[j*mapSize.x + i];
	visitedMap[j*mapSize.x + i] = true;

	for (int k = 0; k < 6; ++k) {
		nj = positions[k].first;
		ni = positions[k].second;
		if (j % 2 != 0 && (nj == -1 || nj == 1) && ni == -1) ni = 1;
		vecino.x = i + ni;
		vecino.y = j + nj;

		if (vecino.x >= 0 && vecino.x <= mapSize.x &&
			vecino.y >= 0 && vecino.y <= mapSize.y &&
			!visitedMap[vecino.y*mapSize.x + vecino.x] &&
			(tileMap[vecino.y*mapSize.x + vecino.x] == color)) {

			ballsToDestroy.push_back(vecino);

			searchBallsToDestroy(vecino.y, vecino.x);
		}
	}
}












void TileMap::searchBallsToDestroy2(int j, int i)
{
	static vector<pair<int, int> > positions = {
		{ -1, -1 },
		{ -1, 0 },
		{ 0, -1 },
		{ 0, 1 },
		{ 1, 0 },
		{ 1, -1 },
	};
	int ni, nj;
	glm::ivec2 vecino;

	int color = tileMap[j*mapSize.x + i];

	visitedMap[j*mapSize.x + i] = true;

	for (int k = 0; k < 6; ++k) {
		nj = positions[k].first;
		ni = positions[k].second;
		if (j % 2 != 0 && (nj == -1 || nj == 1) && ni == -1) ni = 1;
		vecino.x = i + ni;
		vecino.y = j + nj;

		if (vecino.x >= 0 && vecino.x <= mapSize.x &&
			vecino.y >= 0 && vecino.y <= mapSize.y &&
			!visitedMap[vecino.y*mapSize.x + vecino.x] &&
			(tileMap[vecino.y*mapSize.x + vecino.x] == color)) {

			ballsToDestroy.push_back(vecino);

			searchBallsToDestroy(vecino.y, vecino.x);
		}
	}
}





















void TileMap::searchAloneBalls(int j, int i)
{
	static vector<pair<int, int> > positions = {
		{ -1, -1 },
		{ -1,  0 },
		{ 0, -1 },
		{ 0,  1 },
		{ 1,  0 },
		{ 1, -1 },
	};
	int ni, nj;
	glm::ivec2 vecino;

	visitedMap[j*mapSize.x + i] = true;

	for (int k = 0; k < 6; ++k) {
		nj = positions[k].first;
		ni = positions[k].second;
		if (j % 2 != 0 && (nj == -1 || nj == 1) && ni == -1) ni = 1;
		vecino.x = i + ni;
		vecino.y = j + nj;

		if (vecino.x >= 0 && vecino.x < mapSize.x &&
			vecino.y >= 0 && vecino.y < mapSize.y &&
			!visitedMap[vecino.y*mapSize.x + vecino.x] &&
			tileMap[vecino.y*mapSize.x + vecino.x] != 0) {

			searchAloneBalls(vecino.y, vecino.x);
		}
	}
}

void TileMap::deleteBalls(vector<glm::ivec2> &ballsToDelete, bool &gameover)
{
	int i, j;
	for (unsigned int k = 0; k < ballsToDelete.size(); ++k) {
		i = ballsToDelete[k].x;
		j = ballsToDelete[k].y;
		tileMap[j*mapSize.x + i] = 0;
		spriteMap[j*mapSize.x + i]->explode();
		if(j==10) gameover=false;
		if (spriteMap[j*mapSize.x + i]->getAnimRepetitions() == 1) {
			delete spriteMap[j*mapSize.x + i];
			spriteMap[j*mapSize.x + i] = NULL;
		}

	}

	clearVectors();

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	if (!lvlClear()) prepareArrays(minCoords, program, gameover);
}

void TileMap::deleteAloneBalls(bool &gameover)
{
	int contador=0;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (visitedMap[j*mapSize.x + i] == false && tileMap[j*mapSize.x + i] != 0) {
				++contador;
				puntuacion+=20;
				tileMap[j*mapSize.x + i] = 0;
				spriteMap[j*mapSize.x + i]->explode();
				if (spriteMap[j*mapSize.x + i]->getAnimRepetitions() == 1) {
					delete spriteMap[j*mapSize.x + i];
				    spriteMap[j*mapSize.x + i] = NULL;
				}
			}
		}
	}
	if(contador>0){

		engine->play2D("./sounds/pbobble-032.wav", false);
	}

	clearVectors();

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	if (!lvlClear()) prepareArrays(minCoords, program, gameover);
}

void TileMap::clearVectors()
{
	ballsToDestroy.clear();

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			visitedMap[j*mapSize.x + i] = false;
		}
	}
}

vector<BolaMapa *> * TileMap::convertToSprites(bool &gameover)
{
	for (int j = 0; j < mapSize.y; ++j) {
		for (int i = 0; i < mapSize.x; ++i) {
			int tile = tileMap[j*mapSize.x + i];
			if (tile != 0) {
				addSprite(j, i, tile-1,gameover);
			}
		}
	}
	return &spriteMap;
}

void TileMap::search(int j, int i, bool &gameover,int color)
{
	if (color != 20){
		searchBallsToDestroy(j, i);
		if (ballsToDestroy.size() != 0) { ballsToDestroy.push_back(glm::ivec2(i, j)); }


		if (puntuacion > puntuacion2 * 200){
			++puntuacion2;
			Game::instance().Bomba();
		}
		if (ballsToDestroy.size() >= 3 || tileMap[j*mapSize.x + i] == 20) {
			engine->play2D("./sounds/pbobble-010.wav", false);
			puntuacion += ballsToDestroy.size() * 10;
			deleteBalls(ballsToDestroy, gameover);

		}
		else {
			engine->play2D("./sounds/pbobble-005.wav", false);
			clearVectors();
		}



		for (int k = 0; k < mapSize.x; ++k) {
			if (tileMap[0 * mapSize.x + k] != 0) searchAloneBalls(0, k);
		}

		deleteAloneBalls(gameover);
	}
	else{
		searchBallsToDestroy2(j, i);
		ballsToDestroy.push_back(glm::ivec2(i, j)); 
		cout << ballsToDestroy.size() << "size " << endl;

		if (puntuacion > puntuacion2 * 200){
			++puntuacion2;
			Game::instance().Bomba();
		}
			engine->play2D("./sounds/pbobble-010.wav", false);
			puntuacion += ballsToDestroy.size() * 10;
			deleteBalls(ballsToDestroy, gameover);


		for (int k = 0; k < mapSize.x; ++k) {
			if (tileMap[0 * mapSize.x + k] != 0) searchAloneBalls(0, k);
		}

		deleteAloneBalls(gameover);
	}

	/*for (int jj = 0; jj < mapSize.y; ++jj) {
		for (int ii = 0; ii < mapSize.x; ++ii) {
			int tile = tileMap[jj*mapSize.x + ii];
			cout << tile;
		}
		cout << endl;
	}*/
}

void TileMap::addSprite(int j, int i, int color,bool &gameover)
{
	glm::vec2 posTile;

	if (j % 2 == 0) posTile = glm::vec2(minCoords.x + i*tileSize, minCoords.y + j*tileSize);
	else posTile = glm::vec2(minCoords.x + i*tileSize + tileSize / 2, minCoords.y + j*tileSize);

	if (spriteMap[j*mapSize.x + i] != NULL) {
		delete spriteMap[j*mapSize.x + i];
		spriteMap[j*mapSize.x + i] = NULL;
	}
	else {
		spriteMap[j*mapSize.x + i] = new BolaMapa();
		spriteMap[j*mapSize.x + i]->init(posTile, program, color + 1,gameover);
	}
}

bool TileMap::lvlClear()
{
	bool lvlclear = true;
	for (int i = 0; i < mapSize.x; ++i) {
		if (tileMap[0 * mapSize.x + i] != 0) lvlclear = false;
	}
	return lvlclear;
}

set<int> TileMap::checkColors()
{
	set<int> vColors;
	for (int j = 0; j < mapSize.y; ++j) {
		for (int i = 0; i < mapSize.x; ++i) {
			int tile = tileMap[j*mapSize.x + i];
			if (tile != 0) {
				vColors.insert(tile-1);
			}
		}
	}
	return vColors;
}

void TileMap::setSound(irrklang::ISoundEngine* eng) {
	engine = eng;
}
int TileMap::getPuntuacion(){
	return puntuacion;
}
void TileMap::setPuntuacion(int punt){
	puntuacion=punt;
}



























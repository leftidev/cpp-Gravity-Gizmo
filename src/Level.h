#pragma once

#include <string>
#include <vector>

#include <GEngine/SpriteBatch.h>

#include "Tile.h"


const int TILE_WIDTH = 42;

class Level {
public:
    // Load the level
    Level(const std::string& fileName);
    ~Level();

    void draw();

    // Getters
    int getWidth() const { return levelData[0].size(); }
    int getHeight() const { return levelData.size(); }

	int levelWidth;
	int levelHeight;

	std::vector<std::string> levelData;
	std::vector<Tile*> tiles;
	glm::vec2 startPlayerPos;
	std::vector<int> enemyTextureIDs;
	std::vector<glm::vec2> enemyStartPositions;
	std::vector<glm::fvec2> enemyVelocities;
};


#include <fstream>
#include <iostream>
#include <algorithm>

#include <GEngine/Errors.h>
#include <GEngine/ResourceManager.h>

#include "Level.h"


Level::Level(const std::string& fileName) {

    std::ifstream file;
    file.open(fileName);

    // Error checking
    if (file.fail()) {
        GEngine::fatalError("Failed to open " + fileName);
    }

    // Throw away the first string in tmp
    std::string tmp;

    file >> tmp;

    std::getline(file, tmp); // Throw away the rest of the first line

    // Read the level data
    while (std::getline(file, tmp)) {
        _levelData.emplace_back(tmp);
    }    
    
    // The level is flipped, reverse the vector
    std::reverse(_levelData.begin(), _levelData.end());

    // Render all the tiles
    for (unsigned int y = 0; y < _levelData.size(); y++) {
        for (unsigned int x = 0; x < _levelData[y].size(); x++) {
            // Grab the tile
            char tile = _levelData[y][x];

            // Process the tile
            switch (tile) {
                case 'R': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/rock_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
				case '#': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/grass_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case 'I': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/ice_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case 'S': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_bot_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case 's': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_top_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case 'L': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_top_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case 'l': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_bot_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case 'X': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/disappearing_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
                case '*': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/exit_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH));
                    _tiles.push_back(temp);
                } break;
				case '@': {
					_startPlayerPos.x = (float)x * (float)TILE_WIDTH;
					_startPlayerPos.y = (float)y * (float)TILE_WIDTH;
				} break;
				case 'E': {
					_enemyTextureIDs.emplace_back(GEngine::ResourceManager::getTexture("../assets/Textures/enemy_black_78x78.png").id);
					_enemyStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					_enemyVelocities.emplace_back(10.0f, 0.0f);
				} break;
                case '.':
                    break;
				default: {
					std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
				} break;
            }
        }
    }
}

Level::~Level()
{
    // Delete the tiles
    for (unsigned int i = 0; i < _tiles.size(); i++) {
        delete _tiles[i];
    }
}
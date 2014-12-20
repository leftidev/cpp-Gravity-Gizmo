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
        levelData.emplace_back(tmp);
    }    

	// Level height in pixels
	levelHeight = levelData.size() * TILE_WIDTH;
	// Level width in pixels
	levelWidth = levelData[0].size() * TILE_WIDTH;

	std::cout << "Level height: " << levelHeight << ", width: " << levelWidth << std::endl;

    // The level is flipped, reverse the vector
    std::reverse(levelData.begin(), levelData.end());

    // Render all the tiles
    for (unsigned int y = 0; y < levelData.size(); y++) {
        for (unsigned int x = 0; x < levelData[y].size(); x++) {
            // Grab the tile
            char tile = levelData[y][x];

            // Process the tile
            switch (tile) {
                case 'R': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/rock_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::SOLID);
                    tiles.push_back(temp);
                } break;
				case '#': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/grass_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::SOLID);
                    tiles.push_back(temp);
                } break;
                case 'I': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/ice_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::SOLID);
                    tiles.push_back(temp);
                } break;
                case 'S': {
                    Tile* temp = new Tile();
                    temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_bot_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::KILL);
                    tiles.push_back(temp);
                } break;
                case 's': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_top_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::KILLREVERSE);
                    tiles.push_back(temp);
                } break;
                case 'L': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_top_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::KILLREVERSE);
                    tiles.push_back(temp);
                } break;
                case 'l': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/spike_bot_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::KILL);
                    tiles.push_back(temp);
                } break;
                case 'X': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/disappearing_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::DISAPPEARING);
					disappearBlockStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					tiles.push_back(temp);
                } break;
                case '*': {
                    Tile* temp = new Tile();
					temp->init(GEngine::ResourceManager::getTexture("../assets/Textures/exit_block_52x52.png").id, glm::fvec2(x * TILE_WIDTH, y * TILE_WIDTH), TileType::EXIT);
                    tiles.push_back(temp);
                } break;
				case '@': {
					startPlayerPos.x = (float)x * (float)TILE_WIDTH;
					startPlayerPos.y = (float)y * (float)TILE_WIDTH;
				} break;
				case 'E': {
					enemyTextureIDs.emplace_back(GEngine::ResourceManager::getTexture("../assets/Textures/enemy_black_78x78.png").id);
					enemyStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					enemyVelocities.emplace_back(7.0f, 0.0f);
					enemyTypes.emplace_back(EnemyType::X_MOVING);
				} break;
				case 'M': {
					enemyTextureIDs.emplace_back(GEngine::ResourceManager::getTexture("../assets/Textures/enemy_black_78x78.png").id);
					enemyStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					enemyVelocities.emplace_back(5.0f, 0.0f);
					enemyTypes.emplace_back(EnemyType::X_MOVINGJUMPING);
				} break;
				case 'J': {
					enemyTextureIDs.emplace_back(GEngine::ResourceManager::getTexture("../assets/Textures/enemy_black_78x78.png").id);
					enemyStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					enemyVelocities.emplace_back(0.0f, 0.0f);
					enemyTypes.emplace_back(EnemyType::JUMPING);
				} break;
				case 'j': {
					enemyTextureIDs.emplace_back(GEngine::ResourceManager::getTexture("../assets/Textures/enemy_black_rev_78x78.png").id);
					enemyStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					enemyVelocities.emplace_back(0.0f, 0.0f);
					enemyTypes.emplace_back(EnemyType::REVERSEJUMPING);
				} break;
				case 'f': {
					enemyTextureIDs.emplace_back(GEngine::ResourceManager::getTexture("../assets/Textures/enemy_black_78x78.png").id);
					enemyStartPositions.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
					enemyVelocities.emplace_back(0.0f, 0.0f);
					enemyTypes.emplace_back(EnemyType::STILL);
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
    for (unsigned int i = 0; i < tiles.size(); i++) {
        delete tiles[i];
    }
}
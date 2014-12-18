#pragma once

#include "Entity.h"

enum TileType {
	SOLID,
	KILL,
	KILLREVERSE,
	DISAPPEARING,
	EXIT
};

class Tile {
public:
    Tile();
    ~Tile();

    void init(int textureID, glm::vec2 pos, TileType Type);
	void draw(GEngine::SpriteBatch& spriteBatch);
	glm::fvec2 getPosition() const { return m_position; }

	float width;
	float height;

	TileType type;
	bool playerDiedOnThisTile = false;

private:
	int m_textureID;
	int m_textureID2;
	glm::fvec2 m_position;
	GEngine::ColorRGBA8 m_color;
};


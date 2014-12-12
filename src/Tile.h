#pragma once

#include "Entity.h"

enum TileType {
	SOLID,
	KILL
};

class Tile {
public:
    Tile();
    ~Tile();

    void init(int TextureID, glm::vec2 pos, TileType type);
	void draw(GEngine::SpriteBatch& _spriteBatch);
	glm::fvec2 getPosition() const { return _position; }

	float width;
	float height;
	int textureID;
	glm::fvec2 _position;
	GEngine::ColorRGBA8 _color;
	TileType _type;
};


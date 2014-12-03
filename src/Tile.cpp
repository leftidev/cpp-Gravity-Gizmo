#include "Tile.h"


Tile::Tile() {
    // Empty
}

Tile::~Tile() {
    // Empty
}

void Tile::init(int TextureID, glm::fvec2 pos) {
    textureID = TextureID;

	width = 42.0f;
	height = 42.0f;

    _position = pos;

    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;
}

void Tile::draw(GEngine::SpriteBatch& _spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = width;
	destRect.w = height;

	_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}
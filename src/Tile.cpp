#include "Tile.h"


Tile::Tile() { }

Tile::~Tile() { }

void Tile::init(int textureID, glm::fvec2 pos, TileType Type) {
    m_textureID = textureID;

	width = 42.0f;
	height = 42.0f;

    m_position = pos;
	type = Type;

    m_color.r = 255;
    m_color.g = 255;
    m_color.b = 255;
    m_color.a = 255;
}

void Tile::draw(GEngine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = width;
	destRect.w = height;

	spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color);
}
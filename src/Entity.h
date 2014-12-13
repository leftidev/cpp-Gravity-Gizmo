#pragma once

#include <glm/glm.hpp>

#include <GEngine/SpriteBatch.h>
#include <GEngine/ResourceManager.h>


class Tile;

class Entity {
public:
    Entity();
    virtual ~Entity();

    void draw(GEngine::SpriteBatch& spriteBatch);
    glm::fvec2 getPosition() const { return m_position; }
	void setPosition(glm::fvec2 position) { m_position = position; }

    float width;
    float height;
	GEngine::ColorRGBA8 m_color;
protected:
    bool collideWithTile(int width, int height, Tile* tile);
	bool collideWithEntity(int width, int height, Entity* entity);
    int m_textureID;

    glm::fvec2 m_speed;
	glm::fvec2 m_position;
};
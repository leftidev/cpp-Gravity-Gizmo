#pragma once

#include <glm/glm.hpp>

#include <GEngine/SpriteBatch.h>
#include <GEngine/ResourceManager.h>


class Tile;

class Entity {
public:
    Entity();
    virtual ~Entity();

    void draw(GEngine::SpriteBatch& _spriteBatch);
    glm::fvec2 getPosition() const { return _position; }
	void setPosition(glm::fvec2 position) { _position = position; }

    float width;
    float height;
	glm::fvec2 _position;
protected:
    bool collideWithTile(int width, int height, Tile* tile);
	bool collideWithEntity(int width, int height, Entity* entity);

    int textureID;


    GEngine::ColorRGBA8 _color;
    glm::fvec2 _speed;
};
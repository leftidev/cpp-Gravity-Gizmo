#pragma once

#include <glm/glm.hpp>

#include <GEngine/SpriteBatch.h>
#include <GEngine/ResourceManager.h>

class Tile;

class Entity
{
public:
    Entity();
    virtual ~Entity();

    void draw(GEngine::SpriteBatch& _spriteBatch);

    glm::fvec2 getPosition() const { return _position; }

    float width;
    float height;

protected:
    bool collideWithTile(int width, int height, Tile* tile);

    int textureID;

    glm::fvec2 _position;
    GEngine::ColorRGBA8 _color;
    glm::fvec2 _speed;
};
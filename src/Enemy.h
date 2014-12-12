#pragma once

#include "Entity.h"
#include "Tile.h"


class Enemy : public Entity {
public:
	Enemy();
	~Enemy();

	void init(int textureID, glm::fvec2 speed, glm::vec2 pos);
};
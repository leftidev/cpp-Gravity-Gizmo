#pragma once

#include "Entity.h"
#include "Enemy.h"


class Projectile : public Entity {
public:
	Projectile(glm::fvec2 speed, glm::vec2 pos);
	~Projectile();

	void update(std::vector<Tile*> tiles, std::vector<Enemy*> enemies, float deltaTime);
	void applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles, std::vector<Enemy*> enemies);

	bool destroyed = false;
};


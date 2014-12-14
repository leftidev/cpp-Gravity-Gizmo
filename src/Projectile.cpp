#include "Projectile.h"
#include "Enemy.h"


Projectile::Projectile(glm::fvec2 speed, glm::vec2 pos) {
	m_textureID = GEngine::ResourceManager::getTexture("../assets/textures/bubble_78x78.png").id;
	m_speed = speed;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	width = 78.0f;
	height = 78.0f;
}

Projectile::~Projectile() { }

void Projectile::update(std::vector<Tile*> tiles, std::vector<Enemy*> enemies, float deltaTime) {
	// Move on X-axis
	m_position.x += m_speed.x * deltaTime;

	// Check collisions on X-axis
	applyCollisions(tiles, enemies);
}

void Projectile::applyCollisions(std::vector<Tile*> tiles, std::vector<Enemy*> enemies) {
	// Collide with level tiles
	for (unsigned int i = 0; i < tiles.size(); i++) {
		if (collideWithTile((int)width, (int)height, tiles[i])) {
			if (tiles[i]->type == SOLID) {
				destroyed = true;
			}
		}
	}
	// Collide with enemies
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (collideWithEntity((int)width, (int)height, enemies[i])) {
			destroyed = true;
			enemies[i]->bubbled = true;
		}
	}
}
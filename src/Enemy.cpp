#include "Enemy.h"

#include <iostream>


Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::init(int TextureID, glm::fvec2 speed, glm::vec2 pos, EnemyType enemyType) {
	m_textureID = TextureID;
	m_speed = speed;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	type = enemyType;

	width = 78.0f;
	height = 78.0f;
}

void Enemy::update(std::vector<Tile*> tiles, float deltaTime) {
	if (type == JUMPING || type == REVERSEJUMPING || type == X_MOVINGJUMPING) {
		if (!jumped && !inAir) {
			// Normal jumping
			applyJump();
		}

		// Player is in air, apply gravity
		if (inAir) {
			jumped = true;
			if (type == JUMPING || type == X_MOVINGJUMPING) {
				m_speed.y -= gravityAcceleration * deltaTime;
			}
			if (type == REVERSEJUMPING) {
				m_speed.y -= -gravityAcceleration * deltaTime;
			}
		}

		// Move on Y-axis
		m_position.y += m_speed.y * deltaTime;

		// Assume player is in air, this makes player fall off platform ledges
		inAir = true;

		// Check collisions on Y-axis
		applyCollisions(glm::fvec2(0.0f, m_speed.y), tiles);


	}

	if (type == X_MOVING || type == X_MOVINGJUMPING) {
		// Move on X-axis
		m_position.x += m_speed.x * deltaTime;

		// Check collisions on X-axis
		applyCollisions(glm::fvec2(m_speed.x, 0.0f), tiles);
	}
}

void Enemy::applyJump() {
	inAir = true;
	jumped = true;

	if (type == JUMPING || X_MOVINGJUMPING) {
		m_speed.y = JUMP_SPEED;
	}
	if (type == REVERSEJUMPING) {
		m_speed.y = -JUMP_SPEED;
	}
}

void Enemy::applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles) {
	// Collide with level tiles
	for (unsigned int i = 0; i < tiles.size(); i++) {
		if (collideWithTile((int)width, (int)height, tiles[i])) {
			if (tiles[i]->type == SOLID) {
				// Collide from left
				if (speed.x > 0) {
					m_position.x = tiles[i]->getPosition().x - width;
					m_speed.x *= -1;
				}
				// Collide from right
				else if (speed.x < 0) {
					m_position.x = tiles[i]->getPosition().x + tiles[i]->width;
					m_speed.x *= -1;
				}

				if (type == JUMPING || type == X_MOVINGJUMPING) {
					// Collide from below
					if (speed.y > 0) {
						m_speed.y = 0;
						m_position.y = tiles[i]->getPosition().y - height;
						inAir = true;
					}
					// Collide from above
					else if (speed.y < 0) {
						m_speed.y = 0;
						m_position.y = tiles[i]->getPosition().y + tiles[i]->height;
						inAir = false;
						jumped = false;
					}
				}
				else if (type == REVERSEJUMPING) {
					// Collide from below
					if (speed.y > 0) {
						m_speed.y = 0;
						m_position.y = tiles[i]->getPosition().y - height;
						inAir = false;
						jumped = false;
					}
					// Collide from above
					else if (speed.y < 0) {
						m_speed.y = 0;
						m_position.y = tiles[i]->getPosition().y + tiles[i]->height;
						inAir = true;
					}
				}
			}
		}
	}
}
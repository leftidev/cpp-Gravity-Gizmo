#include <SDL/SDL.h>

#include "Player.h"

#include <iostream>


Player::Player() { }

Player::~Player() { }

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
	m_textureID = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right.png").id;
	textureID2 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right.png").id;
	textureID3 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_left.png").id;
	textureID4 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right_reverse.png").id;
	textureID5 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_left_reverse.png").id;

    width = 40.0f;
    height = 40.0f;

	m_speed = glm::fvec2(0.0f, 0.0f);
    m_position = pos;

	playerStartPos = pos;

    m_inputManager = inputManager;
    m_camera = camera;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);
}

void Player::draw(GEngine::SpriteBatch& _spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = m_position.x;
	destRect.y = m_position.y;
	destRect.z = width;
	destRect.w = height;

	if (upsideDown && direction == "right") {
		m_textureID = textureID4;
	}
	else if (!upsideDown && direction == "right"){
		m_textureID = textureID2;
	}
	else if (upsideDown && direction == "left") {
		m_textureID = textureID5;
	}
	else if (!upsideDown && direction == "left"){
		m_textureID = textureID3;
	}

	_spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color);
}

void Player::update(std::vector<Tile*> tiles, std::vector<Enemy*> enemies, float deltaTime) {
	// Apply bend gravity, if player presses W or UP arrow and the gravity is normal
	if ((m_inputManager->isKeyDown(SDLK_w) || m_inputManager->isKeyDown(SDLK_UP)) && normalGravity) {
		applyGravityBend();
	}
	// Apply bend gravity, if player presses S or DOWN arrow and the gravity is reversed
	if ((m_inputManager->isKeyDown(SDLK_s) || m_inputManager->isKeyDown(SDLK_DOWN)) && !normalGravity) {
		applyGravityBend();
	}
    // Space jumps
    if(m_inputManager->isKeyPressed(SDLK_SPACE)) {
        if(jumped) {
            // Double jumping
            applyDoubleJump();
        }
    }
	// Space jumps
    if(m_inputManager->isKeyDown(SDLK_SPACE)) {
		if (!jumped) {
			// Normal jumping
			applyJump();
		}
    }

    // Player is in air, apply gravity
    if (inAir) {
        jumped = true;
		m_speed.y -= gravityAcceleration * deltaTime;
    }

	// Player movement left or right
	updateHorizontalMovement();

    // Move on Y-axis
    m_position.y += m_speed.y * deltaTime;

    // Assume player is in air, this makes player fall off platform ledges
    inAir = true;

    // Check collisions on Y-axis
    applyCollisions(glm::fvec2(0.0f, m_speed.y), tiles, enemies);

    // Move on X-axis
    m_position.x += m_speed.x * deltaTime;

    // Check collisions on X-axis
	applyCollisions(glm::fvec2(m_speed.x, 0.0f), tiles, enemies);
}

// TODO: Add freeze for some time, so player doesn't accidentally move fatally when respawning
void Player::respawnAt(glm::vec2 respawnPos) {
	m_speed.x = 0;
	m_speed.y = 0;
	setPosition(respawnPos);
	if (!normalGravity) {
		upsideDown = false;
		gravityAcceleration *= -1;
		normalGravity = true;
	}
}

void Player::updateHorizontalMovement() {
	// Move left
	if (m_inputManager->isKeyDown(SDLK_a) || m_inputManager->isKeyDown(SDLK_LEFT)) {
		direction = "left";
		// Apply acceleration
		m_speed.x -= ACCELERATION;
		if (m_speed.x < -MAX_MOVE_SPEED) {
			m_speed.x = -MAX_MOVE_SPEED;
		}
	}
	// Move right
	else if (m_inputManager->isKeyDown(SDLK_d) || m_inputManager->isKeyDown(SDLK_RIGHT)) {
		direction = "right";
		// Apply acceleration
		m_speed.x += ACCELERATION;
		if (m_speed.x > MAX_MOVE_SPEED) {
			m_speed.x = MAX_MOVE_SPEED;
		}
	}
	else {
		// Apply de-acceleration
		if (m_speed.x < 0) {
			m_speed.x += ACCELERATION * 2;
			if (m_speed.x >= 0) {
				m_speed.x = 0.0f;
			}
		}
		// Apply de-acceleration
		else {
			m_speed.x -= ACCELERATION * 2;
			if (m_speed.x <= 0) {
				m_speed.x = 0.0f;
			}
		}
	}
}

void Player::applyJump() {
	inAir = true;
	jumped = true;
	canDoubleJump = true;

	if (normalGravity) {
		m_speed.y = JUMP_SPEED;
	}
	else {
		m_speed.y = -JUMP_SPEED;
	}
}

void Player::applyDoubleJump() {
    if(canDoubleJump) {
        canDoubleJump = false;
        if(normalGravity) {
            m_speed.y = JUMP_SPEED + 1;
        }
        else {
            m_speed.y = -JUMP_SPEED - 1;
        }
    }

}

void Player::applyGravityBend() {
	if (normalGravity) {
		if (!inAir && gravityAcceleration > 0) {
			upsideDown = true;
			gravityAcceleration *= -1;
			normalGravity = false;
		}
	}
	else {
		if (!inAir && gravityAcceleration < 0) {
			upsideDown = false;
			gravityAcceleration *= -1;
			normalGravity = true;
		}
	}
}

// Collisions
void Player::applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles, std::vector<Enemy*> enemies) {

	// Collide with level tiles
	for (unsigned int i = 0; i < tiles.size(); i++) {
		if (collideWithTile((int)width, (int)height, tiles[i])) {
			if (tiles[i]->type == SOLID) {
				// Collide from left
				if (speed.x > 0) {
					m_position.x = tiles[i]->getPosition().x - width;
				}
				// Collide from right
				else if (speed.x < 0) {
					m_position.x = tiles[i]->getPosition().x + tiles[i]->width;
				}

				if (normalGravity) {
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
						canDoubleJump = false;
					}
				}
				else {
					// Collide from below
					if (speed.y > 0) {
						m_speed.y = 0;
						m_position.y = tiles[i]->getPosition().y - height;
						inAir = false;
						jumped = false;
						canDoubleJump = false;
					}
					// Collide from above
					else if (speed.y < 0) {
						m_speed.y = 0;
						m_position.y = tiles[i]->getPosition().y + tiles[i]->height;
						inAir = true;
					}
				}
			}
			if (tiles[i]->type == KILL) {
				dead = true;
			}
		}
	}
	// Collide with enemies
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (collideWithEntity((int)width, (int)height, enemies[i])) {
			dead = true;
		}
	}
}
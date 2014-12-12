#include <SDL/SDL.h>

#include "Player.h"

#include <iostream>


Player::Player() { }

Player::~Player() { }

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
	textureID = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right.png").id;
	textureID2 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right.png").id;
	textureID3 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_left.png").id;
	textureID4 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right_reverse.png").id;
	textureID5 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_left_reverse.png").id;

    width = 40.0f;
    height = 40.0f;

	_speed = glm::fvec2(0.0f, 0.0f);
    _position = pos;

	_playerStartPos = pos;

    _inputManager = inputManager;
    _camera = camera;
	_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	inAir = true;
	jumped = false;
	canDoubleJump = false;
	normalGravity = true;
	upsideDown = false;
	direction == "right";
}

void Player::draw(GEngine::SpriteBatch& _spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = width;
	destRect.w = height;

	if (upsideDown && direction == "right") {
		textureID = textureID4;
	}
	else if (!upsideDown && direction == "right"){
		textureID = textureID2;
	}
	else if (upsideDown && direction == "left") {
		textureID = textureID5;
	}
	else if (!upsideDown && direction == "left"){
		textureID = textureID3;
	}

	_spriteBatch.draw(destRect, uvRect, textureID, 0.0f, _color);
}

void Player::update(std::vector<Tile*> tiles, std::vector<Enemy*> enemies, float deltaTime) {
	// Apply bend gravity, if player presses W or UP arrow and the gravity is normal
	if ((_inputManager->isKeyDown(SDLK_w) || _inputManager->isKeyDown(SDLK_UP)) && normalGravity) {
		applyGravityBend();
	}
	// Apply bend gravity, if player presses S or DOWN arrow and the gravity is reversed
	if ((_inputManager->isKeyDown(SDLK_s) || _inputManager->isKeyDown(SDLK_DOWN)) && !normalGravity) {
		applyGravityBend();
	}
    // Space jumps
    if(_inputManager->isKeyPressed(SDLK_SPACE)) {
        if(jumped) {
            // Double jumping
            applyDoubleJump();
        }
    }
	// Space jumps
    if(_inputManager->isKeyDown(SDLK_SPACE)) {
		if (!jumped) {
			// Normal jumping
			applyJump();
		}
    }

    // Player is in air, apply gravity
    if (inAir) {
        jumped = true;
		_speed.y -= gravityAcceleration * deltaTime;
    }

	// Player movement left or right
	applyHorizontalMovement();

    // Move on Y-axis
    _position.y += _speed.y * deltaTime;

    // Assume player is in air, this makes player fall off platform ledges
    inAir = true;

    // Check collisions on Y-axis
    applyCollisions(glm::fvec2(0.0f, _speed.y), tiles, enemies);

    // Move on X-axis
    _position.x += _speed.x * deltaTime;

    // Check collisions on X-axis
	applyCollisions(glm::fvec2(_speed.x, 0.0f), tiles, enemies);
}

// TODO: Add freeze for some time, so player doesn't accidentally move fatally when respawning
void Player::respawnAt(glm::vec2 respawnPos) {
	_speed.x = 0;
	_speed.y = 0;
	setPosition(respawnPos);
	if (!normalGravity) {
		upsideDown = false;
		gravityAcceleration *= -1;
		normalGravity = true;
	}
}

void Player::applyHorizontalMovement() {
	// Move left
	if (_inputManager->isKeyDown(SDLK_a) || _inputManager->isKeyDown(SDLK_LEFT)) {
		direction = "left";
		// Apply acceleration
		_speed.x -= ACCELERATION;
		if (_speed.x < -MAX_MOVE_SPEED) {
			_speed.x = -MAX_MOVE_SPEED;
		}
	}
	// Move right
	else if (_inputManager->isKeyDown(SDLK_d) || _inputManager->isKeyDown(SDLK_RIGHT)) {
		direction = "right";
		// Apply acceleration
		_speed.x += ACCELERATION;
		if (_speed.x > MAX_MOVE_SPEED) {
			_speed.x = MAX_MOVE_SPEED;
		}
	}
	else {
		// Apply de-acceleration
		if (_speed.x < 0) {
			_speed.x += ACCELERATION * 2;
			if (_speed.x >= 0) {
				_speed.x = 0.0f;
			}
		}
		// Apply de-acceleration
		else {
			_speed.x -= ACCELERATION * 2;
			if (_speed.x <= 0) {
				_speed.x = 0.0f;
			}
		}
	}
}

void Player::applyJump() {
	inAir = true;
	jumped = true;
	canDoubleJump = true;

	if (normalGravity) {
		_speed.y = JUMP_SPEED;
	}
	else {
		_speed.y = -JUMP_SPEED;
	}
}

void Player::applyDoubleJump() {
    if(canDoubleJump) {
        canDoubleJump = false;
        if(normalGravity) {
            _speed.y = JUMP_SPEED;
        }
        else {
            _speed.y = -JUMP_SPEED;
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
			if (tiles[i]->_type == SOLID) {
				// Collide from left
				if (speed.x > 0) {
					_position.x = tiles[i]->getPosition().x - width;
				}
				// Collide from right
				else if (speed.x < 0) {
					_position.x = tiles[i]->getPosition().x + tiles[i]->width;
				}

				if (normalGravity) {
					// Collide from below
					if (speed.y > 0) {
						_speed.y = 0;
						_position.y = tiles[i]->getPosition().y - height;
						inAir = true;
					}
					// Collide from above
					else if (speed.y < 0) {
						_speed.y = 0;
						_position.y = tiles[i]->getPosition().y + tiles[i]->height;
						inAir = false;
						jumped = false;
						canDoubleJump = false;
					}
				}
				else {
					// Collide from below
					if (speed.y > 0) {
						_speed.y = 0;
						_position.y = tiles[i]->getPosition().y - height;
						inAir = false;
						jumped = false;
						canDoubleJump = false;
					}
					// Collide from above
					else if (speed.y < 0) {
						_speed.y = 0;
						_position.y = tiles[i]->getPosition().y + tiles[i]->height;
						inAir = true;
					}
				}
			}
			if (tiles[i]->_type == KILL) {
				respawnAt(_playerStartPos);
			}
		}
	}
	// Collide with enemies
	for (unsigned int i = 0; i < enemies.size(); i++) {
		if (collideWithEntity((int)width, (int)height, enemies[i])) {
			respawnAt(_playerStartPos);
		}
	}
}
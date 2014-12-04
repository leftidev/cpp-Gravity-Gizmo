#include <SDL/SDL.h>

#include "Player.h"

#include <iostream>

Player::Player() {
    // Empty
}

Player::~Player() {
    // Empty
}

void Player::init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera) {
	textureID = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right.png").id;
	textureID2 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right.png").id;
	textureID3 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_left.png").id;
	textureID4 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_right_reverse.png").id;
	textureID5 = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo_left_reverse.png").id;

    width = 40.0f;
    height = 40.0f;

    runningSpeed = 20.0f;

	_speed = glm::fvec2(0.0f, 0.0f);

    _position = pos;
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

void Player::update(std::vector<Tile*> tiles, float deltaTime) {
	// Apply bend gravity, if player presses W or UP arrow and normalGravity
	if ((_inputManager->isKeyDown(SDLK_w) == true || _inputManager->isKeyDown(SDLK_UP) == true) && normalGravity) {
		applyGravityBend();
	}
	// Apply bend gravity, if player presses S or DOWN arrow and !normalGravity
	if ((_inputManager->isKeyDown(SDLK_s) == true || _inputManager->isKeyDown(SDLK_DOWN) == true) && !normalGravity) {
		applyGravityBend();
	}

    // SPACEBAR
	if (_inputManager->isKeyPressed(SDLK_SPACE) == true) {
		if (!jumped) {
			// Normal jumping
			applyJump();
		}
		else {
			// Double jumping
			applyDoubleJump();
		}

    }

    // Player is in air, apply gravity
    if (inAir) {
        jumped = true;
		_speed.y -= gravity_acceleration * deltaTime;
    }

	// Move left
	if (_inputManager->isKeyDown(SDLK_a) || _inputManager->isKeyDown(SDLK_LEFT)) {
		direction = "left";
		// Apply acceleration
		_speed.x -= ACCELERATION;
		if (_speed.x < -MAX_SPEED) {
			_speed.x = -MAX_SPEED;
		}
	}
	// Move right
	else if (_inputManager->isKeyDown(SDLK_d) || _inputManager->isKeyDown(SDLK_RIGHT)) {
		direction = "right";
		// Apply acceleration
		_speed.x += ACCELERATION;
		if (_speed.x > MAX_SPEED) {
			_speed.x = MAX_SPEED;
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

    // Move on Y-axis
    _position.y += _speed.y * deltaTime;

    // Assume player is in air, this makes player fall off platform ledges
    inAir = true;

    // Check collisions on Y-axis
    applyCollisions(glm::fvec2(0.0f, _speed.y), tiles);

    // Move on X-axis
    _position.x += _speed.x * deltaTime;

    // Check collisions on X-axis
    applyCollisions(glm::fvec2(_speed.x, 0.0f), tiles);
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
	if (canDoubleJump) {
		canDoubleJump = false;
		if (normalGravity) {
			_speed.y = JUMP_SPEED;
		}
		else {
			_speed.y = -JUMP_SPEED;
		}
	}
}

void Player::applyGravityBend() {
	if (normalGravity) {
		if (!inAir && gravity_acceleration > 0) {
			upsideDown = true;
			gravity_acceleration *= -1;
			normalGravity = false;
		}
	}
	else {
		if (!inAir && gravity_acceleration < 0) {
			upsideDown = false;
			gravity_acceleration *= -1;
			normalGravity = true;
		}
	}
}

// Collisions
void Player::applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles) {

    // Collide with level tiles
	for (unsigned int i = 0; i < tiles.size(); i++) {
        if (collideWithTile((int)width, (int)height, tiles[i])) {
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
				}
			}
			else {
				// Collide from below
				if (speed.y > 0) {
					_speed.y = 0;
					_position.y = tiles[i]->getPosition().y - height;
					inAir = false;
					jumped = false;
				}
				// Collide from above
				else if (speed.y < 0) {
					_speed.y = 0;
					_position.y = tiles[i]->getPosition().y + tiles[i]->height;
					inAir = true;
				}
			}
        }
    }
}
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
    textureID = GEngine::ResourceManager::getTexture("../assets/Textures/gizmo.png").id;

    width = 40.0f;
    height = 40.0f;

    runningSpeed = 20.0f;

    _speed.x = 0.0f;
    _speed.y = 0.0f;

    _position = pos;
    _inputManager = inputManager;
    _camera = camera;

    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;

	inAir = true;
	jumped = false;
	normalGravity = true;
}

void Player::update(std::vector<Tile*> tiles, float deltaTime) {
	// Hacky way of quitting
	if (_inputManager->isKeyDown(SDLK_ESCAPE)) {
		SDL_Quit();
		exit(69);
	}

	// Apply bend gravity, if player presses W or UP arrow and normalGravity
	if ((_inputManager->isKeyDown(SDLK_w) == true || _inputManager->isKeyDown(SDLK_UP) == true) && normalGravity) {
		applyGravityBend();
	}
	// Apply bend gravity, if player presses S or DOWN arrow and !normalGravity
	if ((_inputManager->isKeyDown(SDLK_s) == true || _inputManager->isKeyDown(SDLK_DOWN) == true) && !normalGravity) {
		applyGravityBend();
	}

    // Apply jump, if player has not jumped and presses SPACEBAR
	if (jumped == false && _inputManager->isKeyDown(SDLK_SPACE) == true) {
        applyJump();
    }

    // Player is in air, apply gravity
    if (inAir) {
        jumped = true;
		_speed.y -= gravity_acceleration * deltaTime;
    }

	// Move left
	if (_inputManager->isKeyDown(SDLK_a) || _inputManager->isKeyDown(SDLK_LEFT)) {
		// Apply acceleration
		_speed.x -= ACCELERATION;
		if (_speed.x < -MAX_SPEED) {
			_speed.x = -MAX_SPEED;
		}
	}
	// Move right
	else if (_inputManager->isKeyDown(SDLK_d) || _inputManager->isKeyDown(SDLK_RIGHT)) {
		// Apply acceleration
		_speed.x += ACCELERATION;
		if (_speed.x > MAX_SPEED) {
			_speed.x = MAX_SPEED;
		}
	}
	else {
		// Apply de-acceleration
		if (_speed.x < 0) {
			_speed.x += ACCELERATION + ACCELERATION;
			if (_speed.x >= 0) {
				_speed.x = 0.0f;
			}
		}
		// Apply de-acceleration
		else {
			_speed.x -= ACCELERATION + ACCELERATION;
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
	if (normalGravity) {
		_speed.y = 20.0f;
		inAir = true;
		jumped = true;
	}
	else {
		_speed.y = -20.0f;
		inAir = true;
		jumped = true;
	}
}

void Player::applyGravityBend() {
	if (normalGravity) {
		if (!inAir && gravity_acceleration > 0) {
			gravity_acceleration *= -1;
			normalGravity = false;
		}
	}
	else {
		if (!inAir && gravity_acceleration < 0) {
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
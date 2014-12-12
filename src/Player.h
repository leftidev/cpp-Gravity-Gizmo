#pragma once

#include <GEngine/InputManager.h>
#include <GEngine/Camera2D.h>

#include "Entity.h"
#include "Tile.h"


class Level;

const float ACCELERATION = 0.70f;
const float MAX_MOVE_SPEED = 10.0f;
const float JUMP_SPEED = 15.0f;

class Player : public Entity {
public:
    Player();
    ~Player();

    void init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera);
	void draw(GEngine::SpriteBatch& _spriteBatch);
    void update(std::vector<Tile*> tiles, float deltaTime);
	void death();
	void applyHorizontalMovement();
    void applyJump();
	void applyDoubleJump();
	void applyGravityBend();
    void applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles);

	std::string direction;

	bool upsideDown; // Sprite flipping variable
	bool inAir;	// Player falling
    bool jumped; // Player has jumped
	bool canDoubleJump; // Player can double jump
	bool normalGravity; // Gravity direction

	float gravityAcceleration = 0.80f; // Gravity velocity

	// For flipping the player sprite
	// NOTE: This is a hacky way of doing it (be sure to refactor)
	int textureID2;
	int textureID3;
	int textureID4;
	int textureID5;

private:
    GEngine::InputManager* _inputManager; // Handle for the input manager
    GEngine::Camera2D* _camera; // Handle for the camera

	glm::fvec2 _playerStartPos;
};


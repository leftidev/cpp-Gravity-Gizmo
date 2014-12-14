#pragma once

#include <GEngine/InputManager.h>
#include <GEngine/Camera2D.h>

#include "Entity.h"
#include "Tile.h"
#include "Enemy.h"
#include "Projectile.h"


class Player : public Entity {
public:
    Player();
    ~Player();

	void init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera);
	void draw(GEngine::SpriteBatch& spriteBatch);
	void update(std::vector<Tile*> tiles, std::vector<Enemy*> enemies, float deltaTime);
	void shootProjectile();
	void applyDeathFlicker();
	void respawnAt(glm::vec2 respawnPos);
	void updateHorizontalMovement();
    void applyJump();
	void applyDoubleJump();
	void applyGravityBend();
	void applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles, std::vector<Enemy*> enemies);

	std::string direction = "right";

	bool dead = false;
	bool upsideDown = false;	// Sprite flipping variable
	bool inAir = true;			// Player falling
    bool jumped = false;		// Player has jumped
	bool canDoubleJump = false; // Player can double jump
	bool normalGravity = true;	// Gravity direction
	float gravityAcceleration = 0.80f; // Gravity velocity
	// For flipping the player sprite
	// NOTE: This is a hacky way of doing it (be sure to refactor)
	int textureID2;
	int textureID3;
	int textureID4;
	int textureID5;
	int deathFlickerFrames = 60;

	const float ACCELERATION = 0.70f;
	const float MAX_MOVE_SPEED = 10.0f;
	const float JUMP_SPEED = 14.0f;

	glm::fvec2 playerStartPos;

	std::vector<Projectile*> projectiles;

private:
    GEngine::InputManager* m_inputManager; // Handle for the input manager
    GEngine::Camera2D* m_camera; // Handle for the camera
};


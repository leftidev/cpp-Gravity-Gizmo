#pragma once

#include <GEngine/InputManager.h>
#include <GEngine/Camera2D.h>
#include <GEngine/Timing.h>
#include <GEngine/SoundManager.h>
#include <GEngine/ParticleEngine2D.h>
#include <GEngine/ParticleBatch2D.h>

#include "Entity.h"
#include "Tile.h"
#include "Enemy.h"
#include "Projectile.h"


class Player : public Entity {
public:
    Player();
    ~Player();

	void init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera, GEngine::SoundManager* soundManager);
	void draw(GEngine::SpriteBatch& spriteBatch);
	void update(GEngine::ParticleBatch2D* smokeParticleBatch, std::vector<Tile*> tiles, std::vector<Enemy*> enemies, float deltaTime);
	void addSmoke(GEngine::ParticleBatch2D* spriteBatch, const glm::vec2& position, int numParticles);
	void shootProjectile();
	void applyDeathFlicker();
	void respawnAt(glm::vec2 respawnPos);
	void updateHorizontalMovement();
	void applyJump(GEngine::ParticleBatch2D* smokeParticleBatch);
	void applyDoubleJump(GEngine::ParticleBatch2D* smokeParticleBatch);
	void applyGravityBend();
	void applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles, std::vector<Enemy*> enemies);

	std::string direction = "right";

	bool finishedLevel = false;
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
	
	const int PROJECTILE_REACH = 400;
	const float ACCELERATION = 0.40f;
	const float MAX_MOVE_SPEED = 10.0f;
	const float JUMP_SPEED = 14.0f;

	glm::fvec2 playerStartPos;

	std::vector<Projectile*> projectiles;

	GEngine::Timer flickerTimer;

	float timeSinceFlickerStarted = 0.0f;

private:
    GEngine::InputManager* m_inputManager; // Handle for the input manager
    GEngine::Camera2D* m_camera; // Handle for the camera

	GEngine::SoundEffect m_jumpSound;
	GEngine::SoundEffect m_doubleJumpSound;
	GEngine::SoundEffect m_normalGravitySound;
	GEngine::SoundEffect m_reverseGravitySound;
	GEngine::SoundEffect m_shootBubbleSound;
	GEngine::SoundEffect m_bubbleHitSound;
};


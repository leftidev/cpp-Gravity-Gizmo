#pragma once

#include <GEngine/InputManager.h>
#include <GEngine/Camera2D.h>

#include "Entity.h"
#include "Tile.h"


const float ACCELERATION = 0.05f;
const float MAX_SPEED = 10.0f;

class Player : public Entity {
public:
    Player();
    ~Player();

    void init(glm::fvec2 pos, GEngine::InputManager* inputManager, GEngine::Camera2D* camera);

	void draw(GEngine::SpriteBatch& _spriteBatch);

    void update(std::vector<Tile*> tiles, float deltaTime);

    void applyJump();

	void applyGravityBend();

    void applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles);

	std::string direction;
	bool upsideDown;

	bool inAir;
    bool jumped;
	bool normalGravity;

	float runningSpeed;
	float gravity_acceleration = 0.80f;

	int textureID2;
	int textureID3;
	int textureID4;

private:
    GEngine::InputManager* _inputManager;

    GEngine::Camera2D* _camera;
};


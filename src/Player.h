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

    void update(std::vector<Tile*> tiles, float deltaTime);

    void applyJump();

	void applyGravityBend();

    void applyCollisions(glm::fvec2(speed), std::vector<Tile*> tiles);

	bool inAir;
    bool jumped;
	bool normalGravity;

	float runningSpeed;
	float gravity_acceleration = 0.80f;

private:
    GEngine::InputManager* _inputManager;

    GEngine::Camera2D* _camera;
};


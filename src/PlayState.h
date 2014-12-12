#pragma once

#include <GEngine/GameState.h>
#include <GEngine/InputManager.h>
#include <GEngine/SpriteBatch.h>
#include <GEngine/GLSLProgram.h>

#include "Player.h"
#include "Tile.h"
#include "Level.h"
#include "Enemy.h"


class StateManager;

class PlayState : public GEngine::GameState {
public:
	PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager);
	~PlayState();
	void init() override;
	void initShaders();
	void initLevel();
	void processEvents() override;
	void update(float deltaTime) override;
	void updateCamera() override;
	void draw() override;

private:
	GEngine::SpriteBatch _spriteBatch; // Draws all sprites
	GEngine::GLSLProgram _textureProgram; // The shader program
	std::vector<Level*> _levels; // vector of all levels
	std::vector<Enemy*> _enemies; // vector of all enemies

	Player* _player;
	int _currentLevel;
};


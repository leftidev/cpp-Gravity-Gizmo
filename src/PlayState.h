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
	PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel);
	~PlayState();
	void init() override;
	void initShaders();
	void initLevel();
	void processEvents() override;
	void update(float deltaTime) override;
	void updateCamera() override;
	void draw() override;

private:
	GEngine::SpriteBatch m_spriteBatch; // Draws all sprites
	GEngine::GLSLProgram m_textureProgram; // The shader program
	Level* m_level; // The level
	std::vector<Enemy*> m_enemies; // All enemies

	Player* m_player = nullptr;
};


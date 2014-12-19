#pragma once

#include <GEngine/GameState.h>
#include <GEngine/InputManager.h>
#include <GEngine/SpriteBatch.h>
#include <GEngine/Shader.h>
#include <GEngine/ShaderProgram.h>

#include "Player.h"
#include "Tile.h"
#include "Level.h"
#include "Enemy.h"


class StateManager;

class PlayState : public GEngine::GameState {
public:
	PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel);
	~PlayState();
	void restart();
	void init() override;
	void loadShaders();
	void initLevel();
	void processEvents() override;
	void update(float deltaTime) override;
	void updateCamera() override;
	void draw() override;
	bool addDisappearingBlock = false;
private:
	GEngine::SpriteBatch m_spriteBatch; // Draws all sprites
	GEngine::ShaderProgram m_shaderProgram; // The shader program
	std::vector<GEngine::Shader> m_shaders;

	std::vector<Enemy*> m_enemies; // All enemies
	Player* m_player = nullptr;
	Level* m_level = nullptr;
};


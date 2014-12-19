#pragma once

#include <GEngine/GameState.h>
#include <GEngine/InputManager.h>
#include <GEngine/SpriteBatch.h>
#include <GEngine/Shader.h>
#include <GEngine/ShaderProgram.h>
#include <GEngine/SpriteFont.h>
#include <GEngine/Timing.h>

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
	void drawHud();
	void draw() override;
	bool addDisappearingBlock = false;

private:
	GEngine::SpriteBatch m_spriteBatch;
	GEngine::SpriteBatch m_hudSpriteBatch;
	GEngine::ShaderProgram m_shaderProgram;
	std::vector<GEngine::Shader> m_shaders;
	GEngine::Camera2D m_hudCamera;

	GEngine::SpriteFont* m_spriteFont;

	std::vector<Enemy*> m_enemies; // All enemies
	Player* m_player = nullptr;
	Level* m_level = nullptr;

	float timeSinceLevelStart = 0;
	float newTime = 0;
	GEngine::Timer m_levelTimer;
};


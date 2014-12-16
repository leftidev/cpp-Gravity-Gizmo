#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <GEngine/StateManager.h>
#include <GEngine/Window.h>

#include "PlayState.h"


PlayState::PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager, int currentLevel) :
	GameState(stateMachine, window, inputManager, currentLevel) {
	std::cout << "GameState::PlayState initialized" << std::endl;
}

PlayState::~PlayState() {
	// Delete the enemies
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		delete m_enemies[i];
	}
	// Delete the player
	delete m_player;

	// Delete the level
	delete m_level;
}

void PlayState::init() {
	// Set up the shaders
	initShaders();

	// Initialize the sprite batch
	m_spriteBatch.init();

	// Set up the camera
	m_camera.init(1024, 768);

	const float CAMERA_SCALE = 1.0f;
	m_camera.setScale(CAMERA_SCALE);

	initLevel();

	// Set black background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void PlayState::initShaders() {
	// Compile the color shader
	m_textureProgram.compileShaders("../assets/shaders/textureShading.vert", "../assets/shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void PlayState::initLevel() {
	// Initialize level 1
	if (m_currentLevel == 1) {
		m_level = new Level("../assets/levels/level01.txt");
	}
	else if (m_currentLevel == 2) {
		m_level = new Level("../assets/levels/level02.txt");
	}
	else if (m_currentLevel == 3) {
		m_level = new Level("../assets/levels/level03.txt");
	}
	else if (m_currentLevel == 4) {
		m_level = new Level("../assets/levels/level04.txt");
	}
	else if (m_currentLevel == 5) {
		m_level = new Level("../assets/levels/level05.txt");
	}
	else if (m_currentLevel == 6) {
		m_level = new Level("../assets/levels/level06.txt");
	}
	else if (m_currentLevel == 7) {
		m_level = new Level("../assets/levels/level07.txt");
	}
	else if (m_currentLevel == 8) {
		m_level = new Level("../assets/levels/level08.txt");
	}
	else if (m_currentLevel == 9) {
		m_level = new Level("../assets/levels/level09.txt");
	}
	else if (m_currentLevel == 10) {
		m_level = new Level("../assets/levels/level10.txt");
	}
	else if (m_currentLevel == 11) {
		m_level = new Level("../assets/levels/level11.txt");
	}
	else if (m_currentLevel == 12) {
		m_level = new Level("../assets/levels/level12.txt");
	}
	else if (m_currentLevel == 13) {
		m_level = new Level("../assets/levels/level13.txt");
	}
	else if (m_currentLevel == 14) {
		m_level = new Level("../assets/levels/level14.txt");
	}
	else if (m_currentLevel == 15) {
		m_level = new Level("../assets/levels/level15.txt");
	}

	// Initialize the player
	m_player = new Player();
	m_player->init(m_level->startPlayerPos, &m_inputManager, &m_camera);

	// Add the enemies
	const std::vector<glm::vec2>& enemyPositions = m_level->enemyStartPositions;
	const std::vector<int>& enemyTextureIDs = m_level->enemyTextureIDs;
	const std::vector<glm::fvec2>& enemyVelocities = m_level->enemyVelocities;
	const std::vector<EnemyType>& enemyTypes = m_level->enemyTypes;
	for (unsigned int i = 0; i < enemyPositions.size(); i++) {
		m_enemies.push_back(new Enemy);
		m_enemies.back()->init(enemyTextureIDs[i], enemyVelocities[i], enemyPositions[i], enemyTypes[i]);
	}
}

void PlayState::processEvents() {
	SDL_Event evnt;
	// Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_stateMachine.quit();
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(float(evnt.motion.x), float(evnt.motion.y));
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void PlayState::update(float deltaTime) {
	if (m_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		m_stateMachine.quit();
	}
	if (m_inputManager.isKeyPressed(SDLK_RETURN)) {
		// Restart the level
		m_stateMachine.changeState(new PlayState(m_stateMachine, m_window, m_inputManager, m_currentLevel));
	}
	if (m_inputManager.isKeyPressed(SDLK_1)) {
		glClearColor(1.f, 0.5f, 0.5f, 1.f);
	}
	if (m_inputManager.isKeyPressed(SDLK_2)) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	}
	if (m_inputManager.isKeyPressed(SDLK_F1)) {
		m_player->respawnAt(m_player->playerStartPos);
	}

	m_player->update(m_level->tiles, m_enemies, deltaTime);

	if (m_player->dead) {
		m_stateMachine.changeState(new PlayState(m_stateMachine, m_window, m_inputManager, m_currentLevel));
	}
	// Player dies when going out of level bounds
	if (m_player->getPosition().y < -400 || m_player->getPosition().y > m_level->levelHeight + 400) {
		m_stateMachine.changeState(new PlayState(m_stateMachine, m_window, m_inputManager, m_currentLevel));
	}
	if (m_player->finishedLevel) {
		m_stateMachine.changeState(new PlayState(m_stateMachine, m_window, m_inputManager, m_currentLevel + 1));
	}

	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->update(m_level->tiles, m_player->projectiles, deltaTime);

		if (m_enemies[i]->destroyed) {
			m_enemies.erase(m_enemies.begin() + i);
		}
	}
	if (m_enemies.empty()) {
		for (unsigned int i = 0; i < m_level->tiles.size(); i++) {
			if (m_level->tiles[i]->type == DISAPPEARING) {
				m_level->tiles.erase(m_level->tiles.begin() + i);
			}
		}
	}
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	m_camera.setPosition(m_player->getPosition());
	m_camera.update();
}

void PlayState::draw() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable shaders
	m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	const glm::vec2 tileDimensions(42.0f);
	const glm::vec2 enemyDimensions(78.0f);

	// Begin drawing
	m_spriteBatch.begin();

	// Draw tiles
	for (unsigned int i = 0; i < m_level->tiles.size(); i++) {
		if (m_camera.isBoxInView(m_level->tiles[i]->getPosition(), tileDimensions)) {
			m_level->tiles[i]->draw(m_spriteBatch);
		}
	}

	// Draw the enemies
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		if (m_camera.isBoxInView(m_enemies[i]->getPosition(), enemyDimensions)) {
			m_enemies[i]->draw(m_spriteBatch);
		}
	}

	// Draw the player
	m_player->draw(m_spriteBatch);

	for (unsigned int i = 0; i < m_player->projectiles.size(); i++) {
		m_player->projectiles[i]->draw(m_spriteBatch);
	}


	// End sprite batch creation
	m_spriteBatch.end();

	// Render to the screen
	m_spriteBatch.renderBatch();

	// Disable shaders
	m_textureProgram.unuse();

	// Swap the backbuffer and draw everything to the screen
	m_window.swapBuffer();
}
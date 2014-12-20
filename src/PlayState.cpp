#include <iostream>
#include <random>
#include <ctime>

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/gtx/rotate_vector.hpp>

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

void PlayState::restart() {
	m_player->respawnAt(m_player->playerStartPos);

	// Clear enemies from the vector
	if (!m_enemies.empty()) {
		m_enemies.clear();
	}
	// If DISAPPEARING blocks have disappeared..
	for (unsigned int i = 0; i < m_level->tiles.size(); i++) {
		if (m_level->tiles[i]->type != TileType::DISAPPEARING) {
			addDisappearingBlock = true;
		}
	}
	// .. add the blocks again
	if (addDisappearingBlock) {
		const std::vector<glm::vec2>& disappearBlockPositions = m_level->disappearBlockStartPos;
		for (unsigned int i = 0; i < disappearBlockPositions.size(); i++) {
			m_level->tiles.push_back(new Tile);
			m_level->tiles.back()->init(GEngine::ResourceManager::getTexture("../assets/Textures/disappearing_block_52x52.png").id, disappearBlockPositions[i], TileType::DISAPPEARING);
		}
	}

	// Add the enemies again to the vector
	const std::vector<glm::vec2>& enemyPositions = m_level->enemyStartPositions;
	const std::vector<int>& enemyTextureIDs = m_level->enemyTextureIDs;
	const std::vector<glm::fvec2>& enemyVelocities = m_level->enemyVelocities;
	const std::vector<EnemyType>& enemyTypes = m_level->enemyTypes;
	for (unsigned int i = 0; i < enemyPositions.size(); i++) {
		m_enemies.push_back(new Enemy);
		m_enemies.back()->init(enemyTextureIDs[i], enemyVelocities[i], enemyPositions[i], enemyTypes[i]);
	}

	// Restart the timer
	m_levelTimer.start();
}

void PlayState::init() {
	// Set up the shaders
	loadShaders();

	m_soundManager.init();

	// Initialize the sprite batch
	m_spriteBatch.init();
	m_hudSpriteBatch.init();

	// Initialize sprite font
	m_spriteFont = new GEngine::SpriteFont("../assets/fonts/centurygothic.ttf", 64);

	// Set up the camera
	m_camera.init(1024, 768);

	m_hudCamera.init(1024, 768);
	m_hudCamera.setPosition(glm::vec2(1024 / 2, 768 / 2));

	const float CAMERA_SCALE = 1.0f;
	m_camera.setScale(CAMERA_SCALE);

	initLevel();

	// Set black background color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Initialize particles
	m_smokeParticleBatch = new GEngine::ParticleBatch2D;
	m_bubbleParticleBatch = new GEngine::ParticleBatch2D;
	m_smokeParticleBatch->init(1000, 0.025f, GEngine::ResourceManager::getTexture("../assets/textures/smoke_particle.png"));
	m_bubbleParticleBatch->init(1000, 0.1f, GEngine::ResourceManager::getTexture("../assets/textures/bubble_particle.png"));
	m_particleEngine.addParticleBatch(m_smokeParticleBatch);
	m_particleEngine.addParticleBatch(m_bubbleParticleBatch);

	// Start the timer
	m_levelTimer.start();
}

void PlayState::loadShaders() {
	// Create and compile the shaders
	m_shaders.push_back(GEngine::Shader("../assets/shaders/textureShading.vert", GL_VERTEX_SHADER));
	m_shaders.push_back(GEngine::Shader("../assets/shaders/textureShading.frag", GL_FRAGMENT_SHADER));
	// Add attributes to shaders
	m_shaderProgram.addAttribute("vertexPosition");
	m_shaderProgram.addAttribute("vertexColor");
	m_shaderProgram.addAttribute("vertexUV");
	// Link the shaders
	m_shaderProgram.linkShaders(m_shaders);
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
	m_player->init(m_level->startPlayerPos, &m_inputManager, &m_camera, &m_soundManager);

	// Add the enemies
	const std::vector<glm::vec2>& enemyPositions = m_level->enemyStartPositions;
	const std::vector<int>& enemyTextureIDs = m_level->enemyTextureIDs;
	const std::vector<glm::fvec2>& enemyVelocities = m_level->enemyVelocities;
	const std::vector<EnemyType>& enemyTypes = m_level->enemyTypes;
	for (unsigned int i = 0; i < enemyPositions.size(); i++) {
		m_enemies.push_back(new Enemy);
		m_enemies.back()->init(enemyTextureIDs[i], enemyVelocities[i], enemyPositions[i], enemyTypes[i]);
	}

	// Load and play music
	m_music = m_soundManager.loadMusic("../assets/musics/music_1.ogg");
	m_music.play(-1);
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
		timeSinceLevelStart = 0.0f;
	}

	m_particleEngine.update(deltaTime);

	m_player->update(m_smokeParticleBatch, m_bubbleParticleBatch, m_level->tiles, m_enemies, deltaTime);

	if (m_player->dead) {
		m_player->dead = false;
		restart();
	}
	// Player dies when going out of level bounds
	if (m_player->getPosition().y < -400 || m_player->getPosition().y > m_level->levelHeight + 400) {
		m_player->dead = true;
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
	timeSinceLevelStart = m_levelTimer.getTicks() / 1000.f;
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	m_camera.setPosition(m_player->getPosition());
	m_camera.update();
	m_hudCamera.update();
}

void PlayState::drawHud() {
	char buffer[256];

	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_shaderProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	// Humans left
	sprintf_s(buffer, "Enemies: %d", m_enemies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(4, 0), glm::vec2(0.5), 0.0f, GEngine::ColorRGBA8(255, 255, 255, 255), GEngine::Justification::LEFT);

	// Zombies left
	sprintf_s(buffer, "Time: %.2f s.", timeSinceLevelStart);
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(4, 768 - 36),
		glm::vec2(0.5), 0.0f, GEngine::ColorRGBA8(255, 255, 255, 255), GEngine::Justification::LEFT);

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}

void PlayState::draw() {
	if (m_player->upsideDown) {
		glClearColor(0.10f, 0.0f, 0.0f, 1.0f);
	} else {
		glClearColor(0.0f, 0.0f, 0.10f, 1.0f);
	}

	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable shaders
	m_shaderProgram.enable();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = m_shaderProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_shaderProgram.getUniformLocation("P");
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

	// Render the particles
	m_particleEngine.draw(&m_spriteBatch);

	// Draw the heads up display
	drawHud();

	// Disable shaders
	m_shaderProgram.disable();

	// Swap the backbuffer and draw everything to the screen
	m_window.swapBuffer();
}
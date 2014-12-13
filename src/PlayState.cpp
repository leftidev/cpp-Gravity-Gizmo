#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <GEngine/StateManager.h>
#include <GEngine/Window.h>

#include "PlayState.h"


PlayState::PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager) :
	GameState(stateMachine, window, inputManager) {
	std::cout << "GameState::PlayState initialized" << std::endl;
}

PlayState::~PlayState() {
	// Delete the levels
	for (unsigned int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
	// Delete the enemies
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		delete m_enemies[i];
	}
}

void PlayState::init() {
	// Set up the shaders
	initShaders();

	// Initialize the sprite batch
	m_spriteBatch.init();

	// Set up the camera
	_camera.init(1024, 768);

	const float CAMERA_SCALE = 1.0f;
	_camera.setScale(CAMERA_SCALE);

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
	m_levels.push_back(new Level("../assets/levels/level05.txt"));
	m_currentLevel = 0;

	// Initialize the player
	m_player = new Player();
	m_player->init(m_levels[m_currentLevel]->startPlayerPos, &_inputManager, &_camera);

	// Add the enemies
	const std::vector<glm::vec2>& enemyPositions = m_levels[m_currentLevel]->enemyStartPositions;
	const std::vector<int>& enemyTextureIDs = m_levels[m_currentLevel]->enemyTextureIDs;
	const std::vector<glm::fvec2>& enemyVelocities = m_levels[m_currentLevel]->enemyVelocities;
	const std::vector<EnemyType>& enemyTypes = m_levels[m_currentLevel]->enemyTypes;
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
			_stateMachine.quit();
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(float(evnt.motion.x), float(evnt.motion.y));
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void PlayState::update(float deltaTime) {
	if (_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		_stateMachine.quit();
	}
	if (_inputManager.isKeyPressed(SDLK_RETURN)) {
		// Restart the level
		_stateMachine.changeState(new PlayState(_stateMachine, _window, _inputManager));
	}
	if (_inputManager.isKeyPressed(SDLK_1)) {
		glClearColor(1.f, 0.5f, 0.5f, 1.f);
	}
	if (_inputManager.isKeyPressed(SDLK_2)) {
		glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	}
	if (_inputManager.isKeyPressed(SDLK_F1)) {
		m_player->respawnAt(m_player->playerStartPos);
	}

	m_player->update(m_levels[m_currentLevel]->tiles, m_enemies, deltaTime);

	if (m_player->dead) {
		m_player->respawnAt(m_player->playerStartPos);
	}
	// Player dies when going out of level bounds
	if (m_player->getPosition().y < -400 || m_player->getPosition().y > m_levels[m_currentLevel]->levelHeight + 400) {
		m_player->respawnAt(m_player->playerStartPos);
	}

	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		m_enemies[i]->update(m_levels[m_currentLevel]->tiles, deltaTime);
	}
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	_camera.setPosition(m_player->getPosition());
	_camera.update();
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
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	const glm::vec2 tileDimensions(42.0f);
	const glm::vec2 enemyDimensions(78.0f);

	// Begin drawing
	m_spriteBatch.begin();

	// Draw tiles
	for (unsigned int i = 0; i < m_levels[m_currentLevel]->tiles.size(); i++) {
		if (_camera.isBoxInView(m_levels[m_currentLevel]->tiles[i]->getPosition(), tileDimensions)) {
			m_levels[m_currentLevel]->tiles[i]->draw(m_spriteBatch);
		}
	}

	// Draw the enemies
	for (unsigned int i = 0; i < m_enemies.size(); i++) {
		if (_camera.isBoxInView(m_enemies[i]->getPosition(), enemyDimensions)) {
			m_enemies[i]->draw(m_spriteBatch);
		}
	}

	// Draw the player
	m_player->draw(m_spriteBatch);

	// End sprite batch creation
	m_spriteBatch.end();

	// Render to the screen
	m_spriteBatch.renderBatch();

	// Disable shaders
	m_textureProgram.unuse();

	// Swap the backbuffer and draw everything to the screen
	_window.swapBuffer();
}
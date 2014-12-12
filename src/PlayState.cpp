#include <iostream>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <GEngine/StateManager.h>
#include <GEngine/Window.h>

#include "PlayState.h"


PlayState::PlayState(GEngine::StateManager& stateMachine, GEngine::Window& window, GEngine::InputManager& inputManager) :
	GameState(stateMachine, window, inputManager),
	_player(nullptr) {
	std::cout << "GameState::PlayState initialized" << std::endl;
}

PlayState::~PlayState() {
	// Delete the levels
	for (unsigned int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
	// Delete the enemies
	for (unsigned int i = 0; i < _enemies.size(); i++) {
		delete _enemies[i];
	}
}

void PlayState::init() {
	// Set up the shaders
	initShaders();

	// Initialize the sprite batch
	_spriteBatch.init();

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
	_textureProgram.compileShaders("../assets/shaders/textureShading.vert", "../assets/shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void PlayState::initLevel() {
	// Initialize level 1
	_levels.push_back(new Level("../assets/levels/level12.txt"));
	_currentLevel = 0;

	// Initialize the player
	_player = new Player();
	_player->init(_levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera);

	// Add the enemies
	const std::vector<glm::vec2>& enemyPositions = _levels[_currentLevel]->getEnemyStartPositions();
	const std::vector<int>& enemyTextureIDs = _levels[_currentLevel]->getEnemyTextureIDs();
	const std::vector<glm::fvec2>& enemyVelocities = _levels[_currentLevel]->getEnemyVelocities();
	for (unsigned int i = 0; i < enemyPositions.size(); i++) {
		_enemies.push_back(new Enemy);
		_enemies.back()->init(enemyTextureIDs[i], enemyVelocities[i], enemyPositions[i]);
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

	_player->update(_levels[_currentLevel]->_tiles, deltaTime);
}

void PlayState::updateCamera() {
	// Make sure the camera is bound to the player position
	_camera.setPosition(_player->getPosition());
	_camera.update();
}

void PlayState::draw() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable shaders
	_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	const glm::vec2 tileDimensions(42.0f);
	const glm::vec2 enemyDimensions(78.0f);

	// Begin drawing
	_spriteBatch.begin();

	// Draw tiles
	for (unsigned int i = 0; i < _levels[_currentLevel]->_tiles.size(); i++) {
		if (_camera.isBoxInView(_levels[_currentLevel]->_tiles[i]->getPosition(), tileDimensions)) {
			_levels[_currentLevel]->_tiles[i]->draw(_spriteBatch);
		}
	}

	// Draw the enemies
	for (unsigned int i = 0; i < _enemies.size(); i++) {
		if (_camera.isBoxInView(_enemies[i]->getPosition(), enemyDimensions)) {
			_enemies[i]->draw(_spriteBatch);
		}
	}

	// Draw the player
	_player->draw(_spriteBatch);

	// End sprite batch creation
	_spriteBatch.end();

	// Render to the screen
	_spriteBatch.renderBatch();

	// Disable shaders
	_textureProgram.unuse();

	// Swap the backbuffer and draw everything to the screen
	_window.swapBuffer();
}
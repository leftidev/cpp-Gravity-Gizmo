/*
    This is Gravity Gizmo. Under development.
*/

#include <algorithm>
#include <iostream>

#include <SDL/SDL.h>

#include <GEngine/GEngine.h>
#include <GEngine/Timing.h>
#include <GEngine/Errors.h>

#include "MainGame.h"


MainGame::MainGame() :
    _screenWidth(1024),
    _screenHeight(768),
    _gameState(GameState::PLAY),
    _fps(0),
    _player(nullptr) {
    // Empty
}

MainGame::~MainGame() {
    // Delete the levels
    for (unsigned int i = 0; i < _levels.size(); i++) {
        delete _levels[i];
    }
	// Delete the enemies
	for (unsigned int i = 0; i < _enemies.size(); i++) {
		delete _enemies[i];
	}
}

void MainGame::run() {
    initSystems();

    initLevel();

    gameLoop();
}

void MainGame::initSystems() {
    // Initialize the game engine
    GEngine::init();

    // Create our window
    _window.create("Gravity Gizmo", _screenWidth, _screenHeight, 0);

    // Black background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set up the shaders
    initShaders();

    // Initialize our sprite batch
    _mainSpriteBatch.init();

    // Set up the camera
    _camera.init(_screenWidth, _screenHeight);
}

void MainGame::initLevel() {
    // Initialize level 1
    _levels.push_back(new Level("../assets/Levels/level1.txt"));
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

void MainGame::initShaders() {
    // Compile the color shader
    _textureProgram.compileShaders("../assets/Shaders/textureShading.vert", "../assets/Shaders/textureShading.frag");
    _textureProgram.addAttribute("vertexPosition");
    _textureProgram.addAttribute("vertexColor");
    _textureProgram.addAttribute("vertexUV");
    _textureProgram.linkShaders();
}

void MainGame::gameLoop() {
    // Some helpful constants
    const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
    const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
    const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
    const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
    const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

    // Used to cap the FPS
    GEngine::FpsLimiter fpsLimiter;
    fpsLimiter.setMaxFPS(3000000.0f);

    // Zoom out the camera by 2x
    const float CAMERA_SCALE = 1.0f;
    _camera.setScale(CAMERA_SCALE);

    // Start our previousTicks variable
    float previousTicks = (float)SDL_GetTicks();

    // Main loop
    while (_gameState == GameState::PLAY) {
        fpsLimiter.beginFrame();

        // Calculate the frameTime in milliseconds
        float newTicks = (float)SDL_GetTicks();
        float frameTime = newTicks - previousTicks;
        previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
        // Get the total delta time
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        _inputManager.update();

		// Quit the game when ESCAPE is pressed
		if (_inputManager.isKeyDown(SDLK_ESCAPE)) {
			SDL_Quit();
			exit(69);
		}

        processInput();

        int i = 0; // This counter makes sure we don't spiral to death!
        // Loop while we still have steps to process
        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
            // The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
            // Update all physics here and pass in deltaTime
            _player->update(_levels[_currentLevel]->_tiles, deltaTime);
            // Since we just took a step that is length deltaTime, subtract from totalDeltaTime
            totalDeltaTime -= deltaTime;
            // Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
            i++;
        }

        // Make sure the camera is bound to the player position
        _camera.setPosition(_player->getPosition());
        _camera.update();

        drawGame();

        // End the frame, limit the FPS, and get the current FPS
        _fps = fpsLimiter.endFrame();
        std::cout << _fps << std::endl;
    }
}

void MainGame::processInput() {
    SDL_Event evnt;
    // Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
        case SDL_QUIT:
            _gameState = GameState::EXIT;
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

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _textureProgram.use();

    // Draw code goes here
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
    _mainSpriteBatch.begin();

    // Draw tiles
    for (unsigned int i = 0; i < _levels[_currentLevel]->_tiles.size(); i++) {
        if (_camera.isBoxInView(_levels[_currentLevel]->_tiles[i]->getPosition(), tileDimensions)) {
            _levels[_currentLevel]->_tiles[i]->draw(_mainSpriteBatch);
        }
    }

	// Draw the enemies
	for (unsigned int i = 0; i < _enemies.size(); i++) {
		if (_camera.isBoxInView(_enemies[i]->getPosition(), enemyDimensions)) {
			_enemies[i]->draw(_mainSpriteBatch);
		}
	}

    // Draw the player
    _player->draw(_mainSpriteBatch);

    // End sprite batch creation
    _mainSpriteBatch.end();

    // Render to the screen
    _mainSpriteBatch.renderBatch();

    // Unbind the program
    _textureProgram.unuse();

    // Swap the buffer and draw everything to the screen
    _window.swapBuffer();
}
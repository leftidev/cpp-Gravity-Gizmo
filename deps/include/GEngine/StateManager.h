#pragma once

#include <vector>

#include "GameState.h"


namespace GEngine {

class StateManager {
public:
	StateManager();
	~StateManager();
	// Changes the state
	void changeState(GameState* gameState);
	void updateInputManager();
	void update(float deltaTime);
	void processEvents();
	void updateCamera();
	void draw();

	bool running() { return _running; }
	void quit() { _running = false; }

private:
	// The stack of states
	std::vector<GameState*> _gameStates;
	bool _running;
};

}
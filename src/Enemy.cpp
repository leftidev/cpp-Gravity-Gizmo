#include "Enemy.h"


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}

void Enemy::init(int TextureID, glm::fvec2 speed, glm::vec2 pos) {
	textureID = TextureID;
	_speed = speed;
	_position = pos;
	_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	width = 78.0f;
	height = 78.0f;
}
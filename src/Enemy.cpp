#include "Enemy.h"


Enemy::Enemy() { }

Enemy::~Enemy() { }

void Enemy::init(int TextureID, glm::fvec2 speed, glm::vec2 pos) {
	m_textureID = TextureID;
	m_speed = speed;
	m_position = pos;
	m_color = GEngine::ColorRGBA8(255, 255, 255, 255);

	width = 78.0f;
	height = 78.0f;
}
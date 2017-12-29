#include "Enemy.h"
#include "BaseApplication.h"
#include <cmath>

Enemy::Enemy() : CCSprite(),
				 SPEED((300+(rand()%150))/BaseApplication::visibleSize.height)
{
}

Enemy::~Enemy()
{
}

void Enemy::update(float deltaTime)
{
	float positionY = getPositionY();
	setPositionY( positionY - SPEED );
}

void Enemy::restart()
{
	_dead = false;
	setVisible(true);
}

bool Enemy::isDead()
{
	return _dead;
}

void Enemy::destroy()
{
	_dead = true;
	setVisible(false);
}
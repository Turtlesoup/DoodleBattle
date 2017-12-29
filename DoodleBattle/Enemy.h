#ifndef ENEMY_H
#define ENEMY_H

#include "cocos2d.h"

USING_NS_CC;

class Enemy : public CCSprite
{
	public:
		Enemy();
		virtual ~Enemy();
		virtual void update(float deltaTime);
		void restart();
		bool isDead();
		void destroy();
	private:
		const float SPEED;
		bool _dead;
};

#endif
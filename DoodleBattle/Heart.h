#ifndef HEART_H
#define HEART_H

#include "cocos2d.h"

USING_NS_CC;

class Heart : public CCSprite
{
	public:
		Heart();
		~Heart();
		void on(bool value);
	private:
		bool _on;
};

#endif
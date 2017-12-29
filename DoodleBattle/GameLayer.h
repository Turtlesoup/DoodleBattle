#ifndef GAME_LAYER_H
#define GAME_LAYER_H

#include "cocos2d.h"
#include <vector>
#include "PatternObject.h"

USING_NS_CC;

class GameLayer
{
	public:
		GameLayer();
		~GameLayer();
		void update(float deltaTime);
		void draw();
		void addPatternObject(PatternObject* patternObject);
		std::vector<PatternObject*>& getPatternObjects();
	private:
		std::vector<PatternObject*> _patternObjects;

};

#endif
#ifndef SPRITE_BATCH_MANAGER_H
#define SPRITE_BATCH_MANAGER_H

#include <map>
#include "cocos2d.h"

USING_NS_CC;

class SpriteBatchManager
{
	public:
		SpriteBatchManager();
		~SpriteBatchManager();
		CCSpriteBatchNode* createSpriteBatch(const char* id, const char* plistURL, const char* pngURL, unsigned int capacity);
		void removeSpriteBatch(const char* id);
		CCSpriteBatchNode* getSpriteBatch(const char* id);
	private:
		std::map<const char*, CCSpriteBatchNode*> _idToSpriteBatchNode;
};

#endif
#include "SpriteBatchManager.h"

SpriteBatchManager::SpriteBatchManager()
{
}

SpriteBatchManager::~SpriteBatchManager()
{
}

CCSpriteBatchNode* SpriteBatchManager::createSpriteBatch(const char* id, const char* plistURL, const char* pngURL, unsigned int capacity)
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistURL);
    CCSpriteBatchNode *spriteBatch = CCSpriteBatchNode::create(pngURL, capacity);

	_idToSpriteBatchNode[id] = spriteBatch;

	return spriteBatch;
}

void SpriteBatchManager::removeSpriteBatch(const char* id)
{
	_idToSpriteBatchNode.erase(id);
}

CCSpriteBatchNode* SpriteBatchManager::getSpriteBatch(const char* id)
{
	return _idToSpriteBatchNode[id];
}
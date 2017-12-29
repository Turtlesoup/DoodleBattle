#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "cocos2d.h"
#include "Screen.h"
#include "DrawableLayer.h"
#include "EventData.h"
#include "GameLayer.h"
#include "Enemy.h"
#include "Heart.h"

USING_NS_CC;

class MainScreen : public Screen
{
	public:
		MainScreen(ScreenData* screenData = NULL);
		~MainScreen();
		virtual bool initialise();
		virtual void draw();
		virtual void onUpdate(float deltaTime);
		void menuCloseCallback(CCObject* pSender);
		virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
		virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
		virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
		virtual void ccTouchesCancelled(CCSet* touches, CCEvent* event);
	private:
		CCLabelBMFont* _scoreTextField;
		CCSpriteBatchNode* _spriteBatch;
		std::vector<Enemy*> _enemies;
		std::vector<Heart*> _hearts;

		unsigned int _score;
		std::stringstream _scoreText;
		float _spawnTimeDecreaseDelay;
		unsigned int _currentLifeCount;
		const unsigned int MAX_LIFE_COUNT;
		const float PATTERN_DISTANCE_THRESHOLD;
		float _currentEnemySpawnTime;
		float _currentEnemySpawnTimeDelay;
		const float ENEMY_SPAWN_DELAY;
		const float ENEMY_SPAWN_DELAY_DECRETION_DELAY;
		const float ENEMY_SPAWN_DELAY_DECRETION_DELTA;
		const float LOWEST_ENEMY_SPAWN_DELAY;
		unsigned int _currentEnemyCount;
		void spawnEnemy();
		DrawableLayer* _drawableLayer;
		GameLayer* _gameLayer;
		void onPatternDetected(EventData* eventData);
		void onButterflyReachedGoal(EventData* eventData);

		void increaseScore();
		void decreaseLives();
};

#endif
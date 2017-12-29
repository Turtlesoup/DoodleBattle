#include <cmath>
#include "MainScreen.h"
#include "EventManager.h"
#include "ScreenManager.h"
#include "SpriteBatchManager.h"
#include "ButterflyObject.h"
#include "ArrowObject.h"
#include "PatternDetectedEventData.h"
#include "BaseApplication.h"
#include "SimpleAudioEngine.h"

MainScreen::MainScreen(ScreenData* screenData) :   Screen(screenData),
							 PATTERN_DISTANCE_THRESHOLD(10.0),
							 ENEMY_SPAWN_DELAY(5.0),
							 ENEMY_SPAWN_DELAY_DECRETION_DELTA(0.5),
							 LOWEST_ENEMY_SPAWN_DELAY(1.0),
							 ENEMY_SPAWN_DELAY_DECRETION_DELAY(20.0),
							 MAX_LIFE_COUNT(3)
{
}

MainScreen::~MainScreen()
{
	delete _drawableLayer;
	delete _gameLayer;

	ScreenManager::getInstance()->getSpriteBatchManager()->removeSpriteBatch("gameSprites");

	//remove event listeners
	EventManager::Get()->removeEventListener(ON_PATTERN_DETECTION, std::bind(&MainScreen::onPatternDetected, this, std::placeholders::_1));
	EventManager::Get()->removeEventListener(ON_BUTTERFLY_REACHED_GOAL, std::bind(&MainScreen::onButterflyReachedGoal, this, std::placeholders::_1));
}

bool MainScreen::initialise()
{
	//super init
	if(!Screen::initialise())
	{
		return false;
	}

	_score = 0;

	_spawnTimeDecreaseDelay = 0.0;
	_currentLifeCount = MAX_LIFE_COUNT;
	_currentEnemySpawnTimeDelay = ENEMY_SPAWN_DELAY;

	setTouchEnabled(true);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float screenHalfWidth = visibleSize.width*0.5;
	float screenHalfHeight = visibleSize.height*0.5;
	CCPoint origin = BaseApplication::origin;

	//create and add a sprite batch to the sprite batch manager
	_spriteBatch = ScreenManager::getInstance()->getSpriteBatchManager()->createSpriteBatch("gameSprites", "assetsList.plist", "assetsList.png", 20);
	addChild(_spriteBatch, -1);
    
	//create the background
	CCSprite* background = CCSprite::createWithSpriteFrameName("paper.png");
	CCSize backgroundSpriteSize = background->boundingBox().size;
	background->setScaleX(visibleSize.width/backgroundSpriteSize.width);
	background->setScaleY(visibleSize.height/backgroundSpriteSize.height);
	background->setPosition(ccp(screenHalfWidth + origin.x, screenHalfHeight + origin.y));
	_spriteBatch->addChild(background, 0);

	//create the instrctions layer
	CCSprite* instructions = CCSprite::createWithSpriteFrameName("instructions.png");
	CCSize instructionsSpriteSize = instructions->boundingBox().size;
	instructions->setScaleX(visibleSize.width/instructionsSpriteSize.width);
	instructions->setScaleY(visibleSize.height/instructionsSpriteSize.height);
	instructions->setPosition(ccp(screenHalfWidth + origin.x, screenHalfHeight + origin.y));
	_spriteBatch->addChild(instructions, 0);

	//create the score text
	_scoreText << "Score: " << _score;

	//create the score textfield
	_scoreTextField = CCLabelBMFont::create(_scoreText.str().c_str(), "font1.fnt");
	_scoreTextField->setPosition(ccp(origin.x + BaseApplication::visibleSize.width*0.15, origin.y + BaseApplication::visibleSize.height*0.95));
	addChild(_scoreTextField);

	//inistialise enemy vars
	_currentEnemyCount = 0;
	_currentEnemySpawnTime = ENEMY_SPAWN_DELAY;

	//create a drawable layer for the patterns to be drawn onto
	_drawableLayer = new DrawableLayer();

	//create a game layer to contain pattern objects and other game entities
	_gameLayer = new GameLayer();

	//create the hearts
	Heart* heart;
	float heartWidth;
	float heartHeight;
	float xPos = BaseApplication::visibleSize.width + BaseApplication::origin.x;
	float yPos = BaseApplication::visibleSize.height + BaseApplication::origin.y;

	for(unsigned int heartIndex = 0; heartIndex < MAX_LIFE_COUNT; heartIndex++)
	{
		heart = new Heart();
		heart->initWithSpriteFrameName("heart.png");
		heart->setScale(BaseApplication::screenScaleFactorFrom768x1024*0.5);
		heartWidth = heart->boundingBox().size.width;
		heartHeight = heart->boundingBox().size.height;
		heart->setPosition( ccp(xPos + (heartWidth*heartIndex*1.5) - (heartWidth*(MAX_LIFE_COUNT+1)), yPos - heartHeight*0.75) );
		addChild(heart, 2);
		_hearts.push_back(heart);

		//set the heart on
		heart->on(true);
	}

	//attach an event listener to listen for detected patterns from the drawable layer
	EventManager::Get()->addEventListener(ON_PATTERN_DETECTION, std::bind(&MainScreen::onPatternDetected, this, std::placeholders::_1));

	//attach and event listener to listen for a butterfly reaching the goal
	EventManager::Get()->addEventListener(ON_BUTTERFLY_REACHED_GOAL, std::bind(&MainScreen::onButterflyReachedGoal, this, std::placeholders::_1));

    return true;
}


void MainScreen::onPatternDetected(EventData* eventData)
{
	PatternDetectedEventData* patternDectecionData = (PatternDetectedEventData*)eventData;
	if(patternDectecionData->distance() < PATTERN_DISTANCE_THRESHOLD)
	{
		//create a copy of the vertices
		std::vector<CCPoint>& patternVerts = _drawableLayer->getPatternVertices();
		unsigned int vertsLength = patternVerts.size();
		unsigned int vertsIndex;

		switch(patternDectecionData->id())
		{	
			case 0: //arrow
			{
				ArrowObject* arrow = new ArrowObject(patternVerts);
				_gameLayer->addPatternObject(arrow);
				break;
			}
			case 1: //butterfly
			{
				ButterflyObject* butterfly = new ButterflyObject(patternVerts);
				_gameLayer->addPatternObject(butterfly);
				break;
			}
		}
	}

	_drawableLayer->clear();
}


void MainScreen::onButterflyReachedGoal(EventData* eventData)
{
	increaseScore();
}

void MainScreen::draw()
{
	_drawableLayer->draw();
	_gameLayer->draw();
}

void MainScreen::increaseScore()
{
	//increase score
	_score++;

	//update score text
	_scoreText.str("");
	_scoreText << "Score: " << _score;
	_scoreTextField->setString(_scoreText.str().c_str());

	//play the collected sound
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/success.wav");
}

void MainScreen::decreaseLives()
{
	//take off a life
	_currentLifeCount--;

	//update the hearts
	Heart* heart;
	bool heartOn;
	for(unsigned int heartIndex = 0; heartIndex < MAX_LIFE_COUNT; heartIndex++)
	{
		heart = _hearts[heartIndex];
		heartOn = heartIndex < _currentLifeCount;
		heart->on(heartOn);
	}

	//check if weve lost
	if(_currentLifeCount == 0)
	{
		//create a screen data instace to house the score
		ScreenData* screenData = new ScreenData();
		screenData->intParameters["score"] = _score;

		//change to the win screen
		ScreenManager::getInstance()->changeScene(this, ScreenManager::WIN_SCREEN, screenData);
	}

	//play the life lost sound
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/lifeLost.wav");
}

void MainScreen::onUpdate(float deltaTime)
{
	Screen::onUpdate(deltaTime);

	_drawableLayer->update(deltaTime);
	_gameLayer->update(deltaTime);

	if(_currentEnemySpawnTimeDelay > LOWEST_ENEMY_SPAWN_DELAY)
	{
		_spawnTimeDecreaseDelay += deltaTime;
		if(_spawnTimeDecreaseDelay >= ENEMY_SPAWN_DELAY_DECRETION_DELAY)
		{
			_spawnTimeDecreaseDelay = 0.0;
			_currentEnemySpawnTimeDelay -= ENEMY_SPAWN_DELAY_DECRETION_DELTA;
			if(_currentEnemySpawnTimeDelay <= LOWEST_ENEMY_SPAWN_DELAY)
			{
				_currentEnemySpawnTimeDelay = LOWEST_ENEMY_SPAWN_DELAY;
			}
		}
	}

	unsigned int enemyIndex;

	//check for collision between the enemies and arrow patters
	std::vector<PatternObject*>& patternObjects = _gameLayer->getPatternObjects();
	unsigned int patternObjectsLength = patternObjects.size();
	PatternObject* patternObject;
	Enemy* enemy;
	CCSize enemyBounds;
	CCPoint enemyOrigin;
	bool hit;
	for(unsigned int patternObjectIndex = 0; patternObjectIndex < patternObjectsLength; patternObjectIndex++)
	{
		patternObject = patternObjects[patternObjectIndex];
		if(!patternObject->getCollision() || patternObject->isDestroyed())
		{
			continue;
		}

		for(enemyIndex = 0; enemyIndex < _currentEnemyCount; enemyIndex++)
		{
			enemy = _enemies[enemyIndex];
			if(enemy->isDead())
			{
				continue;
			}

			enemyBounds = enemy->boundingBox().size;
			enemyOrigin = enemy->boundingBox().origin;
			hit = patternObject->checkCollision(enemy->getPositionX() + enemyOrigin.x, enemy->getPositionY(), enemyBounds.width, enemyBounds.height);
			if(hit)
			{
				//destroy the enemy
				enemy->destroy();

				//destroy the arrow
				patternObject->destroy();

				//play the enemy die sound
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/cat.wav");

				break;
			}
		}
	}

	_currentEnemySpawnTime -= deltaTime;
	if(_currentEnemySpawnTime <= 0)
	{
		_currentEnemySpawnTime = _currentEnemySpawnTimeDelay;
		spawnEnemy();
	}

	for(enemyIndex = 0; enemyIndex < _currentEnemyCount; enemyIndex++)
	{
		enemy = _enemies[enemyIndex];
		if(enemy->isDead())
		{
			continue;
		}

		enemy->update(deltaTime);

		if(enemy->getPositionY() < 0)
		{
			enemy->destroy();
			decreaseLives();
			break;
		}
	}
}

void MainScreen::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void MainScreen::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	CCPoint touchPos = this->getParent()->convertTouchToNodeSpace(dynamic_cast<CCTouch*>(touches->anyObject()));
	_drawableLayer->onTouchBegin(touchPos);
}
void MainScreen::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	CCPoint touchPos = this->getParent()->convertTouchToNodeSpace(dynamic_cast<CCTouch*>(touches->anyObject()));
	_drawableLayer->onTouchMoved(touchPos);
}
void MainScreen::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCPoint touchPos = this->getParent()->convertTouchToNodeSpace(dynamic_cast<CCTouch*>(touches->anyObject()));
	_drawableLayer->onTouchEnd(touchPos);
}

void MainScreen::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
	_drawableLayer->onTouchCancel();
}



void MainScreen::spawnEnemy()
{
	//check if any enemies are dead and re-use if so
	Enemy* enemy = NULL;
	for(unsigned int enemyIndex = 0; enemyIndex < _currentEnemyCount; enemyIndex++)
	{
		if(_enemies[enemyIndex]->isDead())
		{
			enemy = _enemies[enemyIndex];
			break;
		}
	}

	//else create a new enemy instance
	if(enemy == NULL)
	{
		enemy = new Enemy();
		enemy->initWithSpriteFrameName("enemy.png");
		enemy->setScale(BaseApplication::screenScaleFactorFrom768x1024*0.5);

		_spriteBatch->addChild(enemy, 1);
		_enemies.push_back(enemy);
		
		_currentEnemyCount++;
	}

	CCSize bounds = enemy->boundingBox().size;

	float xPos = BaseApplication::origin.x + (rand() % (int)(BaseApplication::visibleSize.width - bounds.width)) + bounds.width*0.5;
	float yPos = BaseApplication::origin.y + BaseApplication::visibleSize.height;

	enemy->setPosition(ccp(xPos, yPos));
	enemy->restart();
}
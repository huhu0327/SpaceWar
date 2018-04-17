#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

#pragma execution_character_set("utf-8")

#define TAG_SPRITE_PLAYER               1000
#define TAG_LABEL_SCORE                 1001
#define TAG_LABEL_HIGHSCORE             1002
#define PADDING_SCREEN					10

class GameScene : public Layer
{
public:

	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(GameScene);

	Size winSize;
	Point posStartTouch, posStartPlayer;

	Vector<Sprite*> items, enemies, missiles;

	bool isGetItem;
	int nScore, nScoreHigh;

	void update(float delta);

	void resetBoom(Ref *sender);

	void resetGetItem(float delta);

	void initData();

	void initBG();
	void initPlayer();

	void initScore();
	void addScore(int add);

	void setMissile(float delta);
	void resetMissile(Ref *sender);

	void setItem(float delta);
	void resetItem(Ref *sender);

	void setEnemy(float delta);
	void resetEnemy(Ref *sender);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
};

#endif
#pragma once

#include "cocos2d.h"

USING_NS_CC;

class MenuScene : public Layer
{
private:
	Point winSize;

public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(MenuScene);

private:
	void InitBg();
	void InitMenu();
	void InitShip();
	void menuCallback(Ref *sender);
};
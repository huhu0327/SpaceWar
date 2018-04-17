#pragma once

#include "cocos2d.h"

USING_NS_CC;

class OptionScene : public Layer
{
public:
	static bool isMute;

public:
	static Scene* createScene();

	virtual bool init();
	CREATE_FUNC(OptionScene);

private:
	void InitBg();
	void MenuCallback(Ref * sender);
};


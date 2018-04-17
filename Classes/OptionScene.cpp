#include "OptionScene.h"
#include "AudioManager.h"

bool OptionScene::isMute = false;

Scene * OptionScene::createScene()
{
	auto scene = Scene::create();

	auto layer = OptionScene::create();
	scene->addChild(layer);

	return scene;
}

bool OptionScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	InitBg();

	return true;
}

void OptionScene::InitBg()
{
	auto bg = Sprite::create("Option_Bg.png");
	bg->setAnchorPoint(Point::ZERO);
	this->addChild(bg);

	auto muteLb = Label::createWithTTF("OPTION", "D2Coding.ttf", 50);
	muteLb->setPosition({ Director::getInstance()->getWinSize().width / 2, 900 });
	//muteLb->setColor(Color3B::BLACK);
	muteLb->enableOutline(Color4B::BLACK, 3);
	this->addChild(muteLb);

	auto closeBtn = MenuItemImage::create("close.png", "close1.png", CC_CALLBACK_1(OptionScene::MenuCallback, this));
	closeBtn->setTag(0);
	closeBtn->setPosition({ closeBtn->getPositionX(), -400 });

	auto muteBtn = MenuItemImage::create("mute.png", "mute1.png", CC_CALLBACK_1(OptionScene::MenuCallback, this));
	muteBtn->setTag(1);
	//muteBtn->setPosition({ muteBtn->getPositionX(), -400 });

	auto button = Menu::create(closeBtn, muteBtn, NULL);
	this->addChild(button);
}

void OptionScene::MenuCallback(Ref * sender)
{
	auto menu = (MenuItem*)sender;

	AudioManager::GetInstance().PlayAudio(AudioManager::Btn);

	switch (menu->getTag())
	{
	case 0:
		Director::getInstance()->popScene();
		break;
	case 1:
		isMute = !isMute;
		AudioManager::GetInstance().MuteAudio(isMute);
		break;
	}
}

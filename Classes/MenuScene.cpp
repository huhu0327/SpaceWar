#include "MenuScene.h"
#include "GameScene.h"
#include "OptionScene.h"
#include "AudioManager.h"

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MenuScene::create();
	scene->addChild(layer);

	return scene;
}

bool MenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	winSize = Director::getInstance()->getWinSize();

	AudioManager::GetInstance().PlayAudio(AudioManager::Bg);
	InitBg();
	InitMenu();

	return true;
}

void MenuScene::InitBg()
{
	auto bg = Sprite::create("Menu_Bg.png");
	bg->setAnchorPoint(Point::ZERO);
	this->addChild(bg);

	InitShip();

	auto logo = Sprite::create("Space War.png");
	logo->setPosition({ winSize.x / 2,  winSize.y - 50 });
	logo->setAnchorPoint({ 0.5, 1 });
	this->addChild(logo);

	auto action_0 = Sequence::create(
		ScaleTo::create(2.0f, 1.15f),
		ScaleTo::create(2.0f, 1.0f),
		NULL
	);

	auto action_1 = RepeatForever::create(action_0);
	logo->runAction(action_1);
}

void MenuScene::InitMenu()
{
	auto playBtn = MenuItemImage::create("Play.png", "Play1.png", CC_CALLBACK_1(MenuScene::menuCallback, this));
	//playBtn->setPosition({ winSize.x / 2 , 350 });
	playBtn->setTag(0);

	auto optionBtn = MenuItemImage::create("Option.png", "Option1.png", CC_CALLBACK_1(MenuScene::menuCallback, this));
	//optionBtn->setPosition({ winSize.x / 2 , 250 });
	optionBtn->setTag(1);

	auto quitBtn = MenuItemImage::create("Quit.png", "Quit1.png", CC_CALLBACK_1(MenuScene::menuCallback, this));
	//quitBtn->setPosition({ winSize.x / 2 , 150 });
	quitBtn->setTag(2);

	auto menu = Menu::create(playBtn, optionBtn, quitBtn, NULL);
	menu->alignItemsVerticallyWithPadding(30);
	menu->setPosition({ menu->getPositionX(), menu->getPositionY() - 300 });
	this->addChild(menu);
}

void MenuScene::InitShip()
{
	auto shipSheet = Sprite::create("ship.png");
	auto shipTexture = shipSheet->getTexture();

	auto animation = Animation::create();
	animation->setDelayPerUnit(0.3f);

	for (int i = 0; i < 10; i++) {
		int column = i / 5;
		int row = i % 5;

		animation->addSpriteFrameWithTexture(shipTexture,
			Rect(row * 16, column * 25, 16, 25));
	}

	auto ship = Sprite::create("ship.png", Rect(0, 0, 16, 25));

	ship->setPosition({ winSize.x / 2, 500 });
	ship->setScale(2.5);

	this->addChild(ship);
	auto animate = Animate::create(animation);
	ship->runAction(RepeatForever::create(animate));

	srand(static_cast<unsigned int>(time(NULL)));

	float delayTime = rand() % 10;

	auto moveShip = Sequence::create(
		MoveTo::create(10.0f, { winSize.x / 2, winSize.y, 0.0f }),
		MoveTo::create(0, { winSize.x / 2, -28, 0.0 }),
		DelayTime::create(delayTime),
		NULL
	);

	ship->runAction(RepeatForever::create(moveShip));
}

void MenuScene::menuCallback(Ref * sender)
{
	auto menu = (MenuItem*)sender;

	AudioManager::GetInstance().PlayAudio(AudioManager::Btn);

	switch (menu->getTag())
	{
	case 0: {
		Scene * pScene = TransitionCrossFade::create(5.0, GameScene::createScene());
		Director::getInstance()->pushScene(pScene);
		break;
	}
	case 1: {
		Director::getInstance()->pushScene(OptionScene::createScene());
		break;
	}
	case 2:
		Director::getInstance()->end();
		break;
	}
}

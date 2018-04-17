#include "GameScene.h"

Scene* GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	initData();

	initBG();
	initPlayer();
	initScore();

	this->scheduleUpdate();
	this->schedule(schedule_selector(GameScene::setMissile), 0.1);
	this->schedule(schedule_selector(GameScene::setItem), 5.0 + rand() % 4);
	this->schedule(schedule_selector(GameScene::setEnemy), 3.0 + rand() % 4);

	return true;
}

void GameScene::update(float delta)
{
	auto sprPlayer = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);
	Rect rectPlayer = sprPlayer->getBoundingBox();

	auto removeSpr = Sprite::create();

	for (Sprite* sprItem : items) {

		Rect rectItem = sprItem->getBoundingBox();

		if (rectPlayer.intersectsRect(rectItem)) {
			removeSpr = sprItem;
		}
	}

	if (items.contains(removeSpr)) {
		resetItem(removeSpr);

		isGetItem = true;
		this->scheduleOnce(schedule_selector(GameScene::resetGetItem), 5.0);
	}

	auto removeMissile = Sprite::create();
	auto removeEnemy = Sprite::create();

	for (Sprite* sprMissile : missiles) {

		Rect rectMissile = sprMissile->getBoundingBox();

		for (Sprite* sprEnemy : enemies) {

			Rect rectEnemy = Rect(sprEnemy->getPositionX()-10, sprEnemy->getPositionY()-10, 20, 20);

			if (rectMissile.intersectsRect(rectEnemy)) {
				int attack = sprMissile->getTag();
				int hp = sprEnemy->getTag();

				removeMissile = sprMissile;

				if (hp-attack>0) {
					sprEnemy->setTag(hp - attack);
					addScore(1);
				}
				else {
					removeEnemy = sprEnemy;
					addScore(100);
				}
			}
		}
	}

	if (missiles.contains(removeMissile)) {
		resetMissile(removeMissile);
	}

	if (enemies.contains(removeEnemy)) {
		auto particle = ParticleSystemQuad::create("game/explosion.plist");
		particle->setPosition(removeEnemy->getPosition());
		particle->setScale(0.5);
		this->addChild(particle);

		auto action = Sequence::create(
										DelayTime::create(1.0),
										CallFuncN::create(CC_CALLBACK_1(GameScene::resetBoom, this)),
										NULL);
		particle->runAction(action);

		resetEnemy(removeEnemy);
	}
}

void GameScene::resetBoom(Ref *sender)
{
	auto particle = (ParticleSystemQuad*)sender;

	this->removeChild(particle);
}


void GameScene::resetGetItem(float delta)
{
	isGetItem = false;
}

void GameScene::initData()
{
	winSize = Director::getInstance()->getWinSize();

	items.clear();
	enemies.clear();
	missiles.clear();

	isGetItem = false;
	nScore = 0;
	nScoreHigh = UserDefault::getInstance()->getIntegerForKey("최고 점수", 0);
}

void GameScene::initBG()		//배경
{
	auto bgLayer = Layer::create();
	this->addChild(bgLayer);

	for (int i = 0; i < 2; i++) {
		auto spr = Sprite::create("Menu_Bg.png");
		spr->setAnchorPoint(Point::ZERO);
		spr->setPosition(Point(0, 1024 * i));
		bgLayer->addChild(spr);
	}

	auto action_0 = Sequence::create(
									MoveBy::create(10, Point(0, -1024)),
									Place::create(Point::ZERO),
									NULL);

	auto action_1 = RepeatForever::create(action_0);
	bgLayer->runAction(action_1);
}

void GameScene::initPlayer()
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

	ship->setScale(2.5);

	auto animate = Animate::create(animation);
	ship->runAction(RepeatForever::create(animate));
	ship->setPosition(Point(winSize.width / 2, 100));
	ship->setTag(TAG_SPRITE_PLAYER);
	this->addChild(ship, 1);
}

void GameScene::initScore()
{
	auto labelScore = Label::createWithTTF("", "D2Coding.ttf", 24);
	labelScore->enableOutline(Color4B::BLACK, 3);
	labelScore->setAnchorPoint(Point(0, 1));
	labelScore->setPosition(Point(10, winSize.height - 10));
	labelScore->setTag(TAG_LABEL_SCORE);
	this->addChild(labelScore, 100);

	auto labelHigh = Label::createWithTTF("", "D2Coding.ttf", 24);
	labelHigh->enableOutline(Color4B::BLACK, 3);
	labelHigh->setAnchorPoint(Point(1, 1));
	labelHigh->setPosition(Point(winSize.width - 10, winSize.height - 10));
	labelHigh->setTag(TAG_LABEL_HIGHSCORE);
	this->addChild(labelHigh, 100);

	addScore(0);
}

void GameScene::addScore(int add)
{
	nScore += add;

	if (nScore>nScoreHigh) {
		nScoreHigh = nScore;
		UserDefault::getInstance()->setIntegerForKey("최고 점수", nScoreHigh);
		UserDefault::getInstance()->flush();
	}

	auto labelScore = (Label*)this->getChildByTag(TAG_LABEL_SCORE);
	labelScore->setString(StringUtils::format("점수 : %d", nScore));

	auto labelHigh = (Label*)this->getChildByTag(TAG_LABEL_HIGHSCORE);
	labelHigh->setString(StringUtils::format("최고 점수 : %d", nScoreHigh));
}

void GameScene::setMissile(float delta)
{
	auto sprPlayer = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);

	Sprite *sprMissile;

	if (isGetItem) {
		sprMissile = Sprite::create("game/missile_pow.png");
		sprMissile->setTag(5);
	}
	else {
		sprMissile = Sprite::create("game/missile.png");
		sprMissile->setTag(1);
	}

	sprMissile->setPosition(sprPlayer->getPosition() + Point(-1, 20));
	this->addChild(sprMissile);

	missiles.pushBack(sprMissile);

	auto action = Sequence::create(
									MoveBy::create(1.0, Point(0, winSize.height)),
									CallFuncN::create(CC_CALLBACK_1(GameScene::resetMissile, this)),
									NULL);
	sprMissile->runAction(action);
}

void GameScene::resetMissile(Ref *sender)
{
	auto sprMissile = (Sprite*)sender;

	missiles.eraseObject(sprMissile);

	this->removeChild(sprMissile);
}

void GameScene::setItem(float delta)
{
	int x = PADDING_SCREEN + rand() % ((int)winSize.width - PADDING_SCREEN * 2);

	auto sprItem = Sprite::create("power-up.png");

	auto animation = Animation::create();

	animation->setDelayPerUnit(0.5f);

	for (int i = 0; i < 4; i++) {
		int column = i / 2;
		int row = i % 2;

		animation->addSpriteFrameWithTexture(sprItem->getTexture(),
			Rect(row * 16, column * 16, 16, 16));
	}

	auto item = Sprite::create("power-up.png", Rect(0, 0, 16, 16));

	auto animate = Animate::create(animation);
	
	item->runAction(RepeatForever::create(animate));

	item->setPosition(Point(x, winSize.height));
	item->setScale(1.5);
	this->addChild(item);

	items.pushBack(item);

	auto action = Sequence::create(
		MoveBy::create(3.0, Point(0, -winSize.height)),
		CallFuncN::create(CC_CALLBACK_1(GameScene::resetItem, this)),
		NULL);
	item->runAction(action);
}

void GameScene::resetItem(Ref *sender)
{
	auto sprItem = (Sprite*)sender;

	items.eraseObject(sprItem);

	this->removeChild(sprItem);
}

void GameScene::setEnemy(float detla)
{
	int x = PADDING_SCREEN + rand() % ((int)winSize.width - PADDING_SCREEN * 2);

	Sprite *sprEnemy = nullptr;

	if (rand() % 100<20) {
		auto tex = Sprite::create("enemy-big.png");

		auto animation = Animation::create();

		animation->setDelayPerUnit(0.5f);

		for (int i = 0; i < 2; i++) {
			int row = i % 2;

			animation->addSpriteFrameWithTexture(tex->getTexture(),
				Rect(row * 32, 0, 32, 32));
		}

		sprEnemy = Sprite::create("enemy-big.png", Rect(0, 0, 32, 32));

		auto animate = Animate::create(animation);

		sprEnemy->runAction(RepeatForever::create(animate));
		sprEnemy->setTag(100);
	}
	else {
		auto tex = Sprite::create("enemy-small.png");

		auto animation = Animation::create();

		animation->setDelayPerUnit(0.3f);

		for (int i = 0; i < 2; i++) {
			int row = i % 2;

			animation->addSpriteFrameWithTexture(tex->getTexture(),
				Rect(row * 16, 0, 16, 16));
		}

		sprEnemy = Sprite::create("enemy-small.png", Rect(0, 0, 16, 16));

		auto animate = Animate::create(animation);

		sprEnemy->runAction(RepeatForever::create(animate));
		sprEnemy->setTag(10);
	}
	sprEnemy->setPosition(Point(x, winSize.height));
	sprEnemy->setScale(3.0);
	this->addChild(sprEnemy);

	enemies.pushBack(sprEnemy);

	auto action = Sequence::create(
									MoveBy::create(12.5, Point(0, -winSize.height)),
									CallFuncN::create(CC_CALLBACK_1(GameScene::resetEnemy, this)),
									NULL);
	sprEnemy->runAction(action);
}

void GameScene::resetEnemy(Ref *sender)
{
	auto sprEnemy = (Sprite*)sender;

	enemies.eraseObject(sprEnemy);

	this->removeChild(sprEnemy);
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)		//처음 터치
{
	posStartTouch = touch->getLocation();

	auto sprPlayer = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);
	posStartPlayer = sprPlayer->getPosition();

	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)		//터치 드래그
{
	Point location = touch->getLocation();

	Point posChange = location - posStartTouch;

	float playerPos = posStartPlayer.x + posChange.x;

	auto sprPlayer = (Sprite*)this->getChildByTag(TAG_SPRITE_PLAYER);
	if (0 >= playerPos - sprPlayer->getTexture()->getContentSize().width / 2) {
		sprPlayer->setPositionX(0.1f + sprPlayer->getTexture()->getContentSize().width / 2);
	}
	if (playerPos + sprPlayer->getTexture()->getContentSize().width / 2 >= winSize.width) {
		sprPlayer->setPositionX(winSize.width - 0.1f - sprPlayer->getTexture()->getContentSize().width / 2);
	}
	if(0 < playerPos - sprPlayer->getTexture()->getContentSize().width / 2 &&
		playerPos + sprPlayer->getTexture()->getContentSize().width / 2 < winSize.width ) {
		sprPlayer->setPositionX(playerPos);
	}
}

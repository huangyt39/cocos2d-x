#include "GameScene.h"

USING_NS_CC;

Scene* GameSence::createScene()
{
	return Scene::create();
}

// on "init" you need to initialize your instance
bool GameSence::init()
{
	//////////////////////////////
	// 1. super init first

	if (!Scene::init())
	{
		return false;
	}

	//add Sprite Sheet
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile("level-sheet.plist");

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameSence::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	//background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bg = Sprite::create("level-background-0.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height /2 + origin.y));
	this->addChild(bg, 0);

	//stoneLayer
	stoneLayer = Layer::create();

	//shoot
	auto label = Label::createWithTTF("Shoot", "fonts/Marker Felt.ttf", 50);
	auto shootBtn = MenuItemLabel::create(label, CC_CALLBACK_1(GameSence::shootCallBack, this));
	Menu* shoot = Menu::create(shootBtn, NULL);
	shoot->setPosition(Vec2(700, 500));
	stoneLayer->addChild(shoot);

	//mouseLayer
	mouseLayer = Layer::create();

	//mouse
	this->mouse = Sprite::createWithSpriteFrameName("mouse-0.png");
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	this->mouse->runAction(RepeatForever::create(mouseAnimate));
	this->mouse->setPosition(Vec2(visibleSize.width / 2 +origin.x, visibleSize.height / 2 + origin.y));
	this->mouseLayer->addChild(this->mouse);

	//cloud
	auto cloud = Sprite::createWithSpriteFrameName("clouds-0.png");
	Animate* cloudAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("cloudAnimation"));
	cloud->runAction(RepeatForever::create(cloudAnimate));
	cloud->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - 40));
	this->mouseLayer->addChild(cloud);

	//stone
	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	stoneLayer->addChild(stone);

	this->addChild(stoneLayer, 1);
	this->addChild(mouseLayer, 1);

	return true;
}

bool GameSence::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	Sprite* cheese;

	auto mousePosition = mouse->getPosition();
	if (true) {
		cheese = Sprite::create("cheese.png");
		cheese->setPosition(location.x,location.y);
		mouseLayer->addChild(cheese);
	}

	auto moveto = MoveTo::create(3.0f, location);
	mouse->runAction(moveto);
	cheese->runAction(Sequence::create(ScaleTo::create(2.0f, 1.0f), FadeOut::create(2.0f), NULL));

	return true;
}

void GameSence::shootCallBack(Ref * pSender){
		Vec2 mousePosition = mouse->getPosition();
		Sprite* shootstone = Sprite::create("stone.png");
		shootstone->setPosition(stone->getPosition());
		stoneLayer->addChild(shootstone);

		/*auto stonemoveto = MoveTo::create(3.0, mousePosition);
		shootstone->runAction(stonemoveto);*/

		shootstone->runAction(Sequence::create(MoveTo::create(3.0, mousePosition), FadeOut::create(1.0f), NULL));

		auto diamond = Sprite::createWithSpriteFrameName("diamond-0.png");
		Animate* diamondAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("diamondAnimation"));
		diamond->runAction(RepeatForever::create(diamondAnimate));
		diamond->setPosition(mousePosition);
		mouseLayer->addChild(diamond);

		float random_x = (int)(CCRANDOM_0_1() * 900 + 60);
		float random_y = (int)(CCRANDOM_0_1() * 260 + 20);
		mouse->runAction(MoveTo::create(3.0f, Vec2(random_x, random_y)));
}

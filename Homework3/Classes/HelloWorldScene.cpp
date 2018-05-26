#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <string>
#pragma execution_character_set("utf-8")

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	dtime = 30;
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

	//background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto bg = Sprite::create("background.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(bg, 0);

	//a
	auto aLabel = Label::createWithTTF("A", "fonts/arial.ttf", 36);
	auto aBtn = MenuItemLabel::create(aLabel, CC_CALLBACK_1(HelloWorld::aCallBack, this));
	Menu* a = Menu::create(aBtn, NULL);
	a->setPosition(Vec2(40, 40));
	this->addChild(a);

	//s
	auto sLabel = Label::createWithTTF("S", "fonts/arial.ttf", 36);
	auto sBtn = MenuItemLabel::create(sLabel, CC_CALLBACK_1(HelloWorld::sCallBack, this));
	Menu* s = Menu::create(sBtn, NULL);
	s->setPosition(Vec2(80, 40));
	this->addChild(s);

	//d
	auto dLabel = Label::createWithTTF("D", "fonts/arial.ttf", 36);
	auto dBtn = MenuItemLabel::create(dLabel, CC_CALLBACK_1(HelloWorld::dCallBack, this));
	Menu* d = Menu::create(dBtn, NULL);
	d->setPosition(Vec2(120, 40));
	this->addChild(d);

	//w
	auto wLabel = Label::createWithTTF("W", "fonts/arial.ttf", 36);
	auto wBtn = MenuItemLabel::create(wLabel, CC_CALLBACK_1(HelloWorld::wCallBack, this));
	Menu* w = Menu::create(wBtn, NULL);
	w->setPosition(Vec2(80, 80));
	this->addChild(w);

	//x
	auto xLabel = Label::createWithTTF("X", "fonts/arial.ttf", 36);
	auto xBtn = MenuItemLabel::create(xLabel, CC_CALLBACK_1(HelloWorld::xCallBack, this));
	Menu* x = Menu::create(xBtn, NULL);
	x->setPosition(Vec2(visibleSize.width + origin.x - 80, 40));
	this->addChild(x);

	//y
	auto yLabel = Label::createWithTTF("Y", "fonts/arial.ttf", 36);
	auto yBtn = MenuItemLabel::create(yLabel, CC_CALLBACK_1(HelloWorld::yCallBack, this));
	Menu* y = Menu::create(yBtn, NULL);
	y->setPosition(Vec2(visibleSize.width + origin.x - 40, 80));
	this->addChild(y);

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(-20, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height / 2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	addChild(pT, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 0);

	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113 * i - 10, 0, 113, 113)));
		attack.pushBack(frame);
	}
	attack.pushBack(frame0);

	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame2 = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 79, 90)));
	auto frameDead = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 79 * 21 - 10, 90)));
	//使用第一帧创建精灵
	dead.reserve(22);
	for (int i = 0; i < 22; i++) {
		auto frame2 = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i - 10, 0, 79, 90)));
		dead.pushBack(frame2);
	}
	dead.pushBack(frame0);

	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
	auto frame3 = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 68, 101)));
	//使用第一帧创建精灵

	//运动动画
	run.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame3 = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame3);
	}
	run.pushBack(frame0);

	runback.reserve(8);
	for (int i = 7; i >= 0; i--) {
		auto frame3 = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		runback.pushBack(frame3);
	}
	runback.pushBack(frame0);

	//time
	time = Label::createWithTTF("0:30", "fonts/arial.ttf", 36);
	time->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 30));
	this->addChild(time);

	//keyboard
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event * event) {
		switch ((int)keyCode) {
		case 146: case 28:			//W:146 ↑:28
			this->wCallBack(this);
			break;
		case 124: case 26:			//A:124 ←:26
			this->aCallBack(this);
			break;
		case 142: case 29:			//S:142 ↓:29
			this->sCallBack(this);
			break;
		case 127: case 27:			//D:127 →:27
			this->dCallBack(this);
			break;
		case 147:					//X:147
			this->xCallBack(this);
			break;
		case 148:					//Y:148
			this->yCallBack(this);
			break;
		}
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

	//调度器
	schedule(schedule_selector(HelloWorld::updateCustom), 1.0f, kRepeatForever, 0);
    return true;
}

void HelloWorld::updateCustom(float dt) {
	//log("update");
	if(dtime > 0) dtime--;
	else {
		pT->runAction(CCProgressTo::create(1.0f, 0));
	}

	if (pT->getPercentage() <= 0) {
		auto over = Label::createWithTTF("GAME OVER", "fonts/arial.ttf", 72);
		over->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(over, 2);
		return;
	}

	//dtime
	std::string timestr = "";
	char minute[5];
	char second[5];
	_itoa(dtime / 60, minute, 10);
	_itoa(dtime % 60, second, 10);
	timestr += minute;
	timestr += ":";
	timestr += ((dtime % 60 )< 10 ? "0" : "");
	timestr += second;

	time->setString(timestr);
}

void HelloWorld::aCallBack(Ref * pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage() > 0) {
		Vec2 beforePos = player->getPosition();

		if (beforePos.x - 40 > 0) {
			Vec2 afterPos = Vec2(beforePos.x - 40, beforePos.y);

			auto runAnimation = Animation::createWithSpriteFrames(runback, 0.1f, 1);
			auto runAnimate = Animate::create(runAnimation);

			//auto MoveLeft = Sequence::create(MoveTo::create(0.8f, afterPos), runAnimate, NULL);
			//player->runAction(MoveLeft);

			player->runAction(MoveTo::create(0.8f, afterPos));
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::sCallBack(Ref * pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage() > 0) {
		Vec2 beforePos = player->getPosition();

		if (beforePos.y - 40 > 0) {
			Vec2 afterPos = Vec2(beforePos.x, beforePos.y - 40);

			auto runAnimation = Animation::createWithSpriteFrames(run, 0.1f, 1);
			auto runAnimate = Animate::create(runAnimation);

			//auto MoveLeft = Sequence::create(MoveTo::create(0.8f, afterPos), runAnimate, NULL);
			//player->runAction(MoveLeft);

			player->runAction(MoveTo::create(0.8f, afterPos));
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::dCallBack(Ref * pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage() > 0) {
		Vec2 beforePos = player->getPosition();

		if (beforePos.x + 40 < visibleSize.width) {
			Vec2 afterPos = Vec2(beforePos.x + 40, beforePos.y);

			auto runAnimation = Animation::createWithSpriteFrames(run, 0.1f, 1);
			auto runAnimate = Animate::create(runAnimation);

			//auto MoveLeft = Sequence::create(MoveTo::create(0.8f, afterPos), runAnimate, NULL);
			//player->runAction(MoveLeft);

			player->runAction(MoveTo::create(0.8f, afterPos));
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::wCallBack(Ref * pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage() > 0) {
		Vec2 beforePos = player->getPosition();

		if (beforePos.y + 40 < visibleSize.height) {
			Vec2 afterPos = Vec2(beforePos.x, beforePos.y + 40);

			auto runAnimation = Animation::createWithSpriteFrames(run, 0.1f, 1);
			auto runAnimate = Animate::create(runAnimation);

			//auto MoveLeft = Sequence::create(MoveTo::create(0.8f, afterPos), runAnimate, NULL);
			//player->runAction(MoveLeft);

			player->runAction(MoveTo::create(0.8f, afterPos));
			player->runAction(runAnimate);
		}
	}
}

void HelloWorld::xCallBack(Ref * pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage() > 0) {
		auto attackAnimation = Animation::createWithSpriteFrames(attack, 0.1f, 1);
		auto attackAnimate = Animate::create(attackAnimation);

		player->runAction(attackAnimate);

		if (pT->getPercentage() <= 80)
			pT->runAction(CCProgressTo::create(1.7f, pT->getPercentage() + 20));
	}
}

void HelloWorld::yCallBack(Ref * pSender) {
	if (player->getSpriteFrame() == frame0 && pT->getPercentage() > 0) {
		auto deadAnimation = Animation::createWithSpriteFrames(dead, 0.1f, 1);
		auto deadAnimate = Animate::create(deadAnimation);

		player->runAction(deadAnimate);

		if (pT->getPercentage() >= 20)
			pT->runAction(CCProgressTo::create(2.2f, pT->getPercentage() - 20));
	}
}



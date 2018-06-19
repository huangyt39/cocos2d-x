#pragma execution_character_set("utf-8")
#include "HitBrick.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#define database UserDefault::getInstance()

USING_NS_CC;
using namespace CocosDenshion;

void HitBrick::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* HitBrick::createScene() {
  srand((unsigned)time(NULL));
  auto scene = Scene::createWithPhysics();

  scene->getPhysicsWorld()->setAutoStep(true);

  // Debug ģʽ
  scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));
  auto layer = HitBrick::create();
  layer->setPhysicsWorld(scene->getPhysicsWorld());
  layer->setJoint();
  scene->addChild(layer);
  return scene;
}

// on "init" you need to initialize your instance
bool HitBrick::init() {
  //////////////////////////////
  // 1. super init first
  if (!Layer::init()) {
    return false;
  }
  visibleSize = Director::getInstance()->getVisibleSize();


  auto edgeSp = Sprite::create();  //����һ������
  auto boundBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.0f, 1.0f, 0.0f), 3);  //edgebox�ǲ��ܸ�����ײӰ���һ�ָ��壬����������������������ı߽�
  edgeSp->setPosition(visibleSize.width / 2, visibleSize.height / 2);  //λ����������Ļ����
  edgeSp->setPhysicsBody(boundBody);
  addChild(edgeSp);


  preloadMusic(); // Ԥ������Ч

  addSprite();    // ��ӱ����͸��־���
  addListener();  // ��Ӽ����� 
  addPlayer();    // ��������
  BrickGeneraetd();  // ����ש��


  schedule(schedule_selector(HitBrick::update), 0.01f, kRepeatForever, 0.1f);

  isPreparing = false;
  onBall = true;
  speed = 10;
  shooted = false;
  spFactor = 0;
  return true;
}

// �ؽ����ӣ��̶��������
// Todo
void HitBrick::setJoint() {
	springJoint = PhysicsJointDistance::construct(
		ball->getPhysicsBody(), player->getPhysicsBody(),
		ball->getAnchorPoint(), player->getAnchorPoint());
	m_world->addJoint(springJoint);
}



// Ԥ������Ч
void HitBrick::preloadMusic() {
  auto sae = SimpleAudioEngine::getInstance();
  sae->preloadEffect("gameover.mp3");
  sae->preloadBackgroundMusic("bgm.mp3");
  sae->playBackgroundMusic("bgm.mp3", true);
}

// ��ӱ����͸��־���
void HitBrick::addSprite() {
  // add background
  auto bgSprite = Sprite::create("bg.png");
  bgSprite->setPosition(visibleSize / 2);
  bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
  this->addChild(bgSprite, 0);


  // add ship
  ship = Sprite::create("ship.png");
  ship->setScale(visibleSize.width / ship->getContentSize().width * 0.97, 1.2f);
  ship->setPosition(visibleSize.width / 2, 0);
  auto shipbody = PhysicsBody::createBox(ship->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
  shipbody->setCategoryBitmask(0xFFFFFFFF);
  shipbody->setCollisionBitmask(0xFFFFFFFF);
  shipbody->setContactTestBitmask(0xFFFFFFFF);
  shipbody->setTag(1);
  shipbody->setDynamic(false);  // ??????�I?????????, ????????????��??
  ship->setPhysicsBody(shipbody);
  this->addChild(ship, 1);

  // add sun and cloud
  auto sunSprite = Sprite::create("sun.png");
  sunSprite->setPosition(rand() % (int)(visibleSize.width - 200) + 100, 550);
  this->addChild(sunSprite);
  auto cloudSprite1 = Sprite::create("cloud.png");
  cloudSprite1->setPosition(rand() % (int)(visibleSize.width - 200) + 100, rand() % 100 + 450);
  this->addChild(cloudSprite1);
  auto cloudSprite2 = Sprite::create("cloud.png");
  cloudSprite2->setPosition(rand() % (int)(visibleSize.width - 200) + 100, rand() % 100 + 450);
  this->addChild(cloudSprite2);
}

// ��Ӽ�����
void HitBrick::addListener() {
  auto keyboardListener = EventListenerKeyboard::create();
  keyboardListener->onKeyPressed = CC_CALLBACK_2(HitBrick::onKeyPressed, this);
  keyboardListener->onKeyReleased = CC_CALLBACK_2(HitBrick::onKeyReleased, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

  auto contactListener = EventListenerPhysicsContact::create();
  contactListener->onContactBegin = CC_CALLBACK_1(HitBrick::onConcactBegin, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

// ������ɫ
void HitBrick::addPlayer() {



  player = Sprite::create("bar.png");
  int xpos = visibleSize.width / 2;

  player->setScale(0.1f, 0.1f);
  player->setPosition(Vec2(xpos, ship->getContentSize().height - player->getContentSize().height*0.1f));
  // ���ð�ĸ�������
  // Todo
  auto playerPhysicsBody = PhysicsBody::createBox(Size(player->getContentSize().width, player->getContentSize().height), PhysicsMaterial(20.0f, 1.0f, 20.0f));
  playerPhysicsBody->setDynamic(true);
  player->setPhysicsBody(playerPhysicsBody);

  this->addChild(player, 2);
  
  ball = Sprite::create("ball.png");
  auto ballPhysicsBody = PhysicsBody::createCircle(ball->getContentSize().width / 2, PhysicsMaterial(0.1f, 1.0f, 1.0f), Vec2(0, 0));
  ballPhysicsBody->setDynamic(true);
  ball->setPhysicsBody(ballPhysicsBody);
  ball->setTag(2);
  //ball->setPosition(Vec2(xpos, player->getPosition().y + ball->getContentSize().height*0.1f));
  ball->setPosition(Vec2(xpos, player->getPosition().y + 36.7f));
  ball->setScale(0.1f, 0.1f);
  // ������ĸ�������
  // Todo
  ball->getPhysicsBody()->setCategoryBitmask(0x03);
  ball->getPhysicsBody()->setCollisionBitmask(0x03);
  ball->getPhysicsBody()->setContactTestBitmask(0x03);


  addChild(ball, 3);
  
}

// ʵ�ּ򵥵�����Ч��
// Todo
void HitBrick::update(float dt) {
	if (isMove)
		this->movePlayer(movekey);
	if (isPreparing && speed <= 20 && !shooted) {
		speed++;
	}
}

void HitBrick::movePlayer(char movekey) {
	auto nowPoc = player->getPosition();
	switch (movekey) {
	case 'A':
		if (player->getPosition().x - 10.0f > 100.0f) {
			player->runAction(MoveBy::create(0.2f, Vec2(-10.0f, 0)));
			if (!shooted) ball->runAction(MoveBy::create(0.2f, Vec2(-10.0f, 0)));
		}
		break;
	case 'D':
		if (player->getPosition().x + 10.0f < visibleSize.width - 100.0f) {
			player->runAction(MoveBy::create(0.2f, Vec2(10.0f, 0)));
			if (!shooted) ball->runAction(MoveBy::create(0.2f, Vec2(10.0f, 0)));
		}
		break;
	}
}


// Todo
void HitBrick::BrickGeneraetd() {

for (int i = 0; i < 3; i++) {
	int cw = 0;
	while (cw <= visibleSize.width) {
		auto box = Sprite::create("box.png");
		// Ϊש�����ø�������
		// Todo
		auto boxPhysicsBody = PhysicsBody::createBox(Size(box->getContentSize().width, box->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 1.0f));
		boxPhysicsBody->setDynamic(false);
		box->setPhysicsBody(boxPhysicsBody);
		box->setTag(3);
		box->setPosition(cw + box->getContentSize().width/2, visibleSize.height - box->getContentSize().height/2 - i * box->getContentSize().height);
		
		box->getPhysicsBody()->setCategoryBitmask(0x03);
		box->getPhysicsBody()->setCollisionBitmask(0x03);
		box->getPhysicsBody()->setContactTestBitmask(0x03);

		this->addChild(box, 3);
		cw += box->getContentSize().width;
	}
 }
}


// ����
void HitBrick::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {

  switch (code) {
  case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case cocos2d::EventKeyboard::KeyCode::KEY_A:
	  movekey = 'A';
	  isMove = true;
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case cocos2d::EventKeyboard::KeyCode::KEY_D:
	  movekey = 'D';
	  isMove = true;
    // �����ƶ�
    // Todo
    break;

  case cocos2d::EventKeyboard::KeyCode::KEY_SPACE: // ��ʼ����
	  //Todo
	  isPreparing = true;
      break;
  default:
    break;
  }
}

// �ͷŰ���
void HitBrick::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
  switch (code) {
  case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
  case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
  case cocos2d::EventKeyboard::KeyCode::KEY_A:
  case cocos2d::EventKeyboard::KeyCode::KEY_D:
    // ֹͣ�˶�
    // Todo
	isMove = false;
    break;
  case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:   // ����������С����
	fire();
    break;

  default:
    break;
  }
}

// ��ײ���
// Todo
bool HitBrick::onConcactBegin(PhysicsContact & contact) {
	auto c1 = contact.getShapeA(), c2 = contact.getShapeB();
	auto nodeA = c1->getBody()->getNode();
	auto nodeB = c2->getBody()->getNode();

	if (nodeA && nodeB) {
		if (nodeA->getTag() == 3) {
			nodeA->removeFromParent();
			ball->getPhysicsBody()->setVelocity(Vec2(cocos2d::random(-200, 200), ball->getPhysicsBody()->getVelocity().y));
		}
		else if (nodeB->getTag() == 3) {
			nodeB->removeFromParent();
			ball->getPhysicsBody()->setVelocity(Vec2(cocos2d::random(-200, 200), ball->getPhysicsBody()->getVelocity().y));
		}
	}

	//ball->getPhysicsBody()->setVelocity(Vec2(cocos2d::random(-200, 200), ball->getPhysicsBody()->getVelocity().y));

	return true;
}


void HitBrick::GameOver() {

	_eventDispatcher->removeAllEventListeners();
	ball->getPhysicsBody()->setVelocity(Vec2(0, 0));
	player->getPhysicsBody()->setVelocity(Vec2(0, 0));
  SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgm.mp3");
  SimpleAudioEngine::getInstance()->playEffect("gameover.mp3", false);

  auto label1 = Label::createWithTTF("Game Over~", "fonts/STXINWEI.TTF", 60);
  label1->setColor(Color3B(0, 0, 0));
  label1->setPosition(visibleSize.width / 2, visibleSize.height / 2);
  this->addChild(label1);

  auto label2 = Label::createWithTTF("����", "fonts/STXINWEI.TTF", 40);
  label2->setColor(Color3B(0, 0, 0));
  auto replayBtn = MenuItemLabel::create(label2, CC_CALLBACK_1(HitBrick::replayCallback, this));
  Menu* replay = Menu::create(replayBtn, NULL);
  replay->setPosition(visibleSize.width / 2 - 80, visibleSize.height / 2 - 100);
  this->addChild(replay);

  auto label3 = Label::createWithTTF("�˳�", "fonts/STXINWEI.TTF", 40);
  label3->setColor(Color3B(0, 0, 0));
  auto exitBtn = MenuItemLabel::create(label3, CC_CALLBACK_1(HitBrick::exitCallback, this));
  Menu* exit = Menu::create(exitBtn, NULL);
  exit->setPosition(visibleSize.width / 2 + 90, visibleSize.height / 2 - 100);
  this->addChild(exit);
}

// ���������水ť��Ӧ����
void HitBrick::replayCallback(Ref * pSender) {
  Director::getInstance()->replaceScene(HitBrick::createScene());
}

// �˳�
void HitBrick::exitCallback(Ref * pSender) {
  Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  exit(0);
#endif
}

void HitBrick::fire() {
	if (!shooted) {
		shooted = true;
		springJoint->removeFormWorld();
		ball->getPhysicsBody()->setVelocity(Vec2(cocos2d::random(-200, 200), speed * 30));
	}
}

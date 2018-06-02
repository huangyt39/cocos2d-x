#pragma once
#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void aCallBack(Ref * pSender);
	void sCallBack(Ref * pSender);
	void dCallBack(Ref * pSender);
	void wCallBack(Ref * pSender);
	void xCallBack(Ref * pSender);
	void yCallBack(Ref * pSender);
	void hitByMonster(float dt);

	void updateCustom(float dt);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> runback;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	cocos2d::Label* HittedNum;
	cocos2d::SpriteFrame* frame0;
	cocos2d::SpriteFrame* frameDead;
	int dtime;
	bool neverDead;
	int hittedNum;
	cocos2d::ProgressTimer* pT;
};

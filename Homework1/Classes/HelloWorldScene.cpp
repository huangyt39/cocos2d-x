#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#pragma execution_character_set("utf-8") //Ĭ��ʹ��UTF8 

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

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...
	auto closeItem2 = MenuItemImage::create(
		"Button.png",
		"ButtonSelect.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback2, this));

	if (closeItem2 == nullptr ||
		closeItem2->getContentSize().width <= 0 ||
		closeItem2->getContentSize().height <= 0)
	{
		problemLoading("'Button.png' and 'ButtonSelect.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem2->getContentSize().width / 2;
		float y = origin.y + closeItem2->getContentSize().height / 2 + 50;
		closeItem2->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	menu = Menu::create(closeItem2, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("��Զ�", "fonts/msyh.ttf", 48);
    if (label == nullptr)
    {
        problemLoading("'fonts/msyh.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

	auto label2 = Label::createWithTTF("16340086", "fonts/Marker Felt.ttf", 48);
	if (label2 == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
	}
	else
	{
		label2->setName("Label2");
		// position the label on the center of the screen
		label2->setPosition(Vec2(origin.x + visibleSize.width / 2,
			origin.y + visibleSize.height - label2->getContentSize().height * 3));

		// add the label as a child to this layer
		this->addChild(label2, 1);
	}

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("Test.png");
    if (sprite == nullptr)
    {
        problemLoading("'Test.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::menuCloseCallback2(Ref* pSender) {
	auto temp = this->getChildByName("Label2");
	temp->setVisible(!temp->isVisible());
}

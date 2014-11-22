#include "HelloWorldScene.h"
#include "PlayScene.h"
#include "ThirdPartyHelper.h"
#include "UIButton.h"
#include "DeveloperInfoScence.h"
#include "LHLeaderBoard.h"
#include "UILayout.h"
#include "LHShareButton.h"
#include "UIText.h"
#include "LHMacros.h"
#include "LocalizedString.h"
#include "SimpleAudioEngine.h"
#include "GuideScene.h"
USING_NS_CC;
USING_NS_UM_SOCIAL;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->initWithColor(Color4B::WHITE);
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto name = ui::Text::create(LHLocalizedCString("appname"), Common_Font, 70);
    name->setColor(Color3B(64, 119, 215));
    name->setPosition(Vec2(visibleSize.width/2, visibleSize.height/3*2) );
    this->addChild(name);
    
    auto play = ui::Button::create("blue.png");
    play->setTitleFontSize(40);
    play->setTitleText(LHLocalizedCString("play"));
    play->setPosition(Vec2(name->getPositionX(), visibleSize.height/2));
    play->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->replaceScene(PlayScene::createScene(nullptr));
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pop.wav");
        }
    });
    this->addChild(play);
    
    auto dbt = DeveloperInfo::DevInfoButton("devinfo.png");
    dbt->setPosition(Vec2(visibleSize.width - 80, visibleSize.height - 80));
    this->addChild(dbt);
    
    auto gbt = GuideScene::guideButton("q.png");
    gbt->setPosition(Vec2(dbt->getPosition().x - gbt->getContentSize().width/2*3, visibleSize.height - 80));
    this->addChild(gbt);
    
    if (UserDefault::getInstance()->getBoolForKey("guide", true)) {
        auto gt = ui::Text::create(LHLocalizedCString("guidetext"), Common_Font, 35);
        gt->setColor(Color3B(250, 201, 11));
        gt->setPosition(Vec2(gbt->getPosition().x - gt->getContentSize().width/2 - gbt->getContentSize().width/2 - 2, gbt->getPosition().y));
        
        MoveBy *byl = MoveBy::create(0.4, Vec2(-20, 0));
        MoveBy *byr = MoveBy::create(0.2, Vec2(20, 0));
        Sequence *sq = Sequence::create(byl,byr,NULL);
        RepeatForever *rp = RepeatForever::create(sq);
        gt->runAction(rp);
        
        this->addChild(gt);
        UserDefault::getInstance()->setBoolForKey("guide", false);
    }
    
    return true;
}

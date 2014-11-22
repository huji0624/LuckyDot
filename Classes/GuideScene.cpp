//------.cpp
#include "GuideScene.h"
#include "LHMacros.h"
#include "GameHelper.h"
USING_NS_CC;
Scene* GuideScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = GuideScene::create();
	layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

GuideScene::~GuideScene(){
}


ui::Button* GuideScene::guideButton(const std::string &name){
    auto bt = ui::Button::create(name);
    bt->addTouchEventListener([](Ref *rf,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            auto tran = TransitionSlideInR::create(0.3, GuideScene::createScene(nullptr));
            Director::getInstance()->pushScene(tran);
        }
    
    });
    bt->setPosition(Vec2(100, 200));
    return bt;
}

void GuideScene::addGuide(const char *info, int *ef){
    Size vs = Director::getInstance()->getVisibleSize();
    auto lo = GameHelper::guideOfEffect(info, ef, MAX_BOX_MEMBER);
    guideList->pushBackCustomItem(lo);
}

bool GuideScene::initDict(cocos2d::CCDictionary *dic)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    float margin = 18;
    
    auto back = ui::Button::create("black.png");
    back->setPosition(Vec2(visibleSize.width/2, back->getContentSize().height/2));
    back->setTitleFontSize(40);
    back->setTitleText(LHLocalizedCString("back"));
    back->addTouchEventListener([](Ref *ps,ui::Widget::TouchEventType type){
        if (type == ui::Widget::TouchEventType::ENDED) {
            Director::getInstance()->popScene();
        }
    });
    this->addChild(back);
    
    auto layout = ui::ListView::create();
    layout->setBackGroundColor(Color3B::WHITE);
    layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    layout->setSize(Size(visibleSize.width ,visibleSize.height - margin - back->getContentSize().height));
    layout->setPosition(Vec2(0, margin/2 + back->getContentSize().height));
    layout->setItemsMargin(10);
    layout->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    this->addChild(layout);
    guideList = layout;
    
    addGuide(INFO_EF_Freeze, EF_Freeze);
    addGuide(INFO_EF_FIXDT,EF_FIXDT);
    addGuide(INFO_EF_ADDMUL, EF_ADDMUL);
    
    addGuide(INFO_EF_FIRE,EF_FIRE);
    addGuide(INFO_EF_FIREG,EF_FIREG);
    addGuide(INFO_EF_FIREB, EF_FIREB);
    
    addGuide(INFO_EF_HEAL, EF_HEAL);
    addGuide(INFO_EF_HEAL_BLUE, EF_HEAL_BLUE);
    addGuide(INFO_EF_HEAL_RED, EF_HEAL_RED);
    
    addGuide(INFO_EF_ROLLOVER, EF_ROLLOVER);
    addGuide(INFO_EF_BLACK,EF_BLACK);
    
	return true;
}

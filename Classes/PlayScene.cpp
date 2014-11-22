//------.cpp
#include "PlayScene.h"
#include "GameOverScene.h"
#include "LHMacros.h"
#include "LocalizedString.h"
#include "SimpleAudioEngine.h"
#include "GameHelper.h"

Scene* PlayScene::createScene(cocos2d::CCDictionary *dic)
{
	auto scene = Scene::create();
	auto layer = PlayScene::create();
    layer->initWithColor(Color4B::WHITE);
	layer->initDict(dic);
	scene->addChild(layer);
	return scene;
}

PlayScene::PlayScene(){
    
}

PlayScene::~PlayScene(){
}

bool PlayScene::initDict(cocos2d::CCDictionary *dic)
{
    Size vs = Director::getInstance()->getVisibleSize();
    
    score = 0;
    started = false;
    dtm = 1.5f;
    multiplyer = 1;
    multiTimer = 0.0f;
    
    ui::Text *tip = ui::Text::create(LHLocalizedCString("tapplay"), Common_Font, 40);
    tip->setColor(Color3B::GRAY);
    this->addChild(tip);

    auto layout = ui::Layout::create();
    layout->setPosition(Vec2(0, 0));
    this->addChild(layout);
    dotlayout = layout;
    
    float margin = 8.0f;
    int lineCount = 4;
    int columnCount = 4;
    float bw = (vs.width - margin*(columnCount+1))/columnCount;
    float bh = bw;
    
    layout->setSize(Size(bw*columnCount,bh*lineCount));
    
    for (int i =0 ; i<lineCount*columnCount; i++) {
        
        int line = i/columnCount;
        int col = i%columnCount;
        
        std::string normal;
       
        int tag = 0;
        float rand = arc4random()%100;
        if (rand < 33) {
            tag = T_BLUE;
        }else if (rand < 66){
            tag = T_RED;
        }else {
            tag = T_GREEN;
        }
        normal = GameHelper::textureNameFromTag(tag);
        
        auto tbt = ui::Button::create(normal);
        tbt->ignoreContentAdaptWithSize(false);
        tbt->setSize(Size(bw, bh));
        tbt->setTag(tag);
        tbt->setPosition(Vec2(bw/2+col*(bw+margin) + margin, (bh+margin)/2 + line*(bh+margin)));
        tbt->addTouchEventListener([this,tip](Ref* charbt,ui::Widget::TouchEventType type){
            
            if(started==false){
                tip->removeFromParent();
                started = true;
                this->scheduleUpdate();
                collectbox->setVisible(true);
            }
            ui::Button *bt = (ui::Button*)charbt;
            if (type == ui::Widget::TouchEventType::BEGAN) {
                if (bt->getTag() == T_BLACK) {
                    this->gameOver();
                    return;
                }
                
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
                collectbox->addMember(bt->getTag());
                this->dotClick(charbt);
            }
        });
        layout->addChild(tbt);
    }
    
    auto proback = Sprite::create("rgray.png");
    proback->setPosition(Vec2(vs.width/2, vs.height - proback->getContentSize().height));
    this->addChild(proback);
    
    auto sp = Sprite::create("rblue.png");
    ProgressTimer *pro = ProgressTimer::create(sp);
    pro->setPosition(Vec2(vs.width/2, vs.height - pro->getContentSize().height));
    pro->setPercentage(50.0f);
    pro->setBarChangeRate(Vec2(1, 0));
    pro->setMidpoint(Vec2(0, 0));
    pro->setType(cocos2d::ProgressTimer::Type::BAR);
    this->addChild(pro);
    progress = pro;
    
    timeReduceLabel = ui::Text::create(StringUtils::format("Δt=%.2f",dtm), Common_Font, 30);
    timeReduceLabel->setColor(Color3B(64, 119, 215));
    timeReduceLabel->setPosition(Vec2(pro->getPositionX(),pro->getPositionY()-timeReduceLabel->getContentSize().height - 2));
    this->addChild(timeReduceLabel);
    
    scoreLabel = ui::Text::create(StringUtils::format("%d",score), Common_Font, 50);
    scoreLabel->setColor(Color3B(64, 119, 215));
    scoreLabel->setPosition(Vec2(vs.width - scoreLabel->getContentSize().width/2-60,vs.height-scoreLabel->getContentSize().height/2-20));
    this->addChild(scoreLabel);
    
    multiLabel = ui::Text::create(StringUtils::format("x%d",multiplyer), Common_Font, 25);
    multiLabel->setColor(Color3B(223, 45, 47));
    multiLabel->setPosition(Vec2(scoreLabel->getPositionX(),scoreLabel->getPositionY()-multiLabel->getContentSize().height - 2));
    this->addChild(multiLabel);
    
    tip->setPosition(Vec2(vs.width/2,(dotlayout->getSize().height + timeReduceLabel->getPositionY())/2));
    
    collectbox = CollectBox::create();
    collectbox->initDict(nullptr);
    collectbox->setSize(Size(bw*3,bh));
    collectbox->setDelegate(this);
    collectbox->setVisible(false);
    collectbox->setPosition(Vec2(vs.width/2 - collectbox->getSize().width/2, tip->getPosition().y - collectbox->getSize().height/2));
    collectbox->setClippingEnabled(true);
    this->addChild(collectbox);
    
	return true;
}

//type is the type that you want.support &
ui::Button* PlayScene::randomDotWithType(int type){
    ui::Button *rtdot = nullptr;
    
    Vector<Node*> pool;
    const Vector<Node*> children = dotlayout->getChildren();
    for (int i = 0 ; i<children.size(); i++) {
        Node *child = children.at(i);
        if (child->getTag() & type) {
            pool.pushBack(child);
        }
    }
    if (pool.size()>0) {
        int randindex = CCRANDOM_0_1()*pool.size();
        rtdot =(ui::Button*) pool.at(randindex);
    } 
    return rtdot;
}

bool PlayScene::onEffectHappen(CollectBoxDelegate::EffectType ef){
    
    const Vector<Node*> children = dotlayout->getChildren();
    if (ef == CollectBoxDelegate::EffectType::HEAL) {
        //将2个黑色转化为灰色
        auto dot = this->randomDotWithType(T_BLACK);
        changeDotTo(dot, T_GRAY);
        
        dot = this->randomDotWithType(T_BLACK);
        changeDotTo(dot, T_GRAY);
        
        this->playEffect("heal", ef);
    }else if (ef == CollectBoxDelegate::EffectType::HEAL_BLUE) {
        //将一个黑色转化为蓝色
        auto dot = this->randomDotWithType(T_BLACK);
        changeDotTo(dot, T_BLUE);
        
        this->playEffect("heal", ef);
    }else if (ef == CollectBoxDelegate::EffectType::HEAL_RED) {
        //将一个黑色转化为红色
        auto dot = this->randomDotWithType(T_BLACK);
        changeDotTo(dot, T_RED);
        
        this->playEffect("heal", ef);
    }else if(ef == CollectBoxDelegate::EffectType::FREEZE){
        //增加时间，10秒内减慢时间递减速度
        float cup = progress->getPercentage();
        progress->setPercentage(cup+15);
        
        this->playEffect("freeze", ef);
    }else if (ef == CollectBoxDelegate::EffectType::FIRE){
        //得分 黑色*4
        int cb = 0;
        for (int i =0 ; i<children.size(); i++) {
            Node *child = children.at(i);
            if (child->getTag() == T_BLACK) {
                cb ++;
            }
        }
        cb++;
        int dscore = cb * 4;
        
        score += dscore;
        this->setScore(score);
        
        this->playEffect("fire", ef);
    }else if (ef == CollectBoxDelegate::EffectType::FIREG){
        //得分 绿色*3
        int cb = 0;
        for (int i =0 ; i<children.size(); i++) {
            Node *child = children.at(i);
            if (child->getTag() == T_GREEN) {
                cb ++;
            }
        }
        cb++;
        int dscore = cb * 3;
        
        score += dscore;
        this->setScore(score);
        
        this->playEffect("fire", ef);
    }else if (ef == CollectBoxDelegate::EffectType::FIREB){
        //得分 黑色*3 减少时间
        int cb = 0;
        for (int i =0 ; i<children.size(); i++) {
            Node *child = children.at(i);
            if (child->getTag() == T_BLUE) {
                cb ++;
            }
        }
        cb++;
        int dscore = cb * 3;
        
        score += dscore;
        this->setScore(score);
        
        this->playEffect("fire", ef);
    }else if (ef == CollectBoxDelegate::EffectType::BLACK){
        //产生一个黑色
        auto dot = this->randomDotWithType(T_BLUE+T_RED+T_GREEN);
        changeDotTo(dot, T_BLACK);
        
        this->playEffect("toblack", ef);
    }else if (ef == CollectBoxDelegate::EffectType::ROLLOVER){
        //颜色翻转 蓝色->红色 红色->绿色 绿色->蓝色
        for (int i =0 ; i<children.size(); i++) {
            Node *child = children.at(i);
            int tag = child->getTag();
            if (tag == T_BLUE){
                this->changeDotTo(child, T_RED);
            }else if (tag == T_RED){
                this->changeDotTo(child, T_GREEN);
            }else if (tag == T_GREEN){
                this->changeDotTo(child, T_BLUE);
            }
        }
        this->playEffect("rollover", ef);
    }else if(ef == CollectBoxDelegate::EffectType::FIXDT){
        
        dtm -= 0.05;
        timeReduceLabel->setString(StringUtils::format("Δt=%.2f",dtm));
        
        this->playEffect("fixdt", ef);
    }else if(ef == CollectBoxDelegate::EffectType::ADDMUL){
        
        multiplyer += 3;
        multiLabel->setString(StringUtils::format("x%d",multiplyer));
        
        this->playEffect("addmul", ef);
    }
    
    return true;
}

//void PlayScene::showGuideWhenFirstHappen(const char *efinfo, int *ef){
//    auto ud = UserDefault::getInstance();
//    bool b = ud->getBoolForKey(efinfo, false);
//    if (!b) {
//        Size vs = Director::getInstance()->getVisibleSize();
//        auto bglo = ui::Layout::create();
//        bglo->setBackGroundColor(Color3B::BLACK);
//        bglo->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//        bglo->setOpacity(180);
//        bglo->setSize(vs);
//        
//        auto guide = GameHelper::guideOfEffect(efinfo, ef, MAX_BOX_MEMBER);
//        guide->setPosition(Vec2(vs.width/2 - guide->getSize().width/2, vs.height/2-guide->getSize().height/2));
//        bglo->addChild(guide);
//        
//        this->addChild(bglo);
////        ud->setBoolForKey(efinfo, true);
//    }
//}

void PlayScene::playEffect(const char *content, CollectBoxDelegate::EffectType ef){
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("ef.wav");
    
    Size vs = Director::getInstance()->getVisibleSize();
    
    Vector<Node*> efmembers = collectbox->clearMember();
    for (int i=0; i<efmembers.size(); i++) {
        Sprite *sp = (Sprite*)efmembers.at(i);
        Vec2 wd = collectbox->convertToWorldSpace(sp->getPosition());
        Vec2 nd = this->convertToNodeSpace(wd);
        sp->setPosition(nd);
        this->addChild(sp);
        MoveTo *mv = MoveTo::create(0.08, Vec2(vs.width/2, collectbox->getPositionY() + collectbox->getSize().height/2));
        CallFuncN *cf = CallFuncN::create([](Node *nd){
            nd->removeFromParent();
        });
        Sequence *efsq = nullptr;
        if (i==0) {
            CallFuncN *mcf = CallFuncN::create([this,content](Node *nd){
                ui::Text *eft = ui::Text::create(LHLocalizedCString(content),Common_Font,50);
                eft->setColor(Color3B(64, 119, 215));
                eft->setPosition(nd->getPosition());
                this->addChild(eft);
                ScaleTo *st = ScaleTo::create(0.05, 2.0f);
                ScaleTo *stb = ScaleTo::create(0.03f, 1.0f);
                FadeTo *fdto = FadeTo::create(1.0, 0);
                CallFuncN *fdtocf = CallFuncN::create([](Node *nd){
                    nd->removeFromParent();
                });
                Sequence *tmpsq = Sequence::create(st,stb,fdto,fdtocf, NULL);
                eft->runAction(tmpsq);
            });
            efsq = Sequence::create(mv,mcf,cf, NULL);
        }else{
            efsq = Sequence::create(mv,cf, NULL);
        }
        sp->runAction(efsq);
    }

}

void PlayScene::update(float delta){
    float cup = progress->getPercentage();
    float top = cup - delta*dtm;
    if (top<=0) {
        this->gameOver();
        return;
    }else if (top>=100){
        return;
    }
    progress->setPercentage(top);
    
    //multiply timer.
    if(multiplyer>1){
        multiTimer += delta;
        if(multiTimer>5.0f){
            multiTimer = 0.0f;
            multiplyer --;
            multiLabel->setString(StringUtils::format("x%d",multiplyer));
        }
    }
}

void PlayScene::gameOver(){
    unscheduleUpdate();
    CCDictionary *dic = CCDictionary::create();
    dic->setObject(CCInteger::create(score), "score");
    Director::getInstance()->replaceScene(GameOverScene::createScene(dic));
}

void PlayScene::changeDotTo(cocos2d::Ref *dot, int tag){
    if (dot==nullptr) {
        return;
    }
    ui::Button *tbt = (ui::Button*)dot;
    ScaleTo *st = ScaleTo::create(0.1, 1.2f);
    tbt->setTag(tag);
    CallFuncN *cn1 = CallFuncN::create([this,tbt,tag](Node* nd){
        tbt->loadTextureNormal(GameHelper::textureNameFromTag(tag));
    });
    ScaleTo *stb = ScaleTo::create(0.1f, 1.0f);
    Sequence *sq = Sequence::create(st,cn1,stb,NULL);
    tbt->runAction(sq);
    
}

void PlayScene::dotClick(cocos2d::Ref *ps){
    ui::Button *bt = (ui::Button*)ps;
    
    score += multiplyer;
    this->setScore(score);
    
    dtm += 0.01;
    timeReduceLabel->setString(StringUtils::format("Δt=%.2f",dtm));
   
    int tag = GameHelper::randomTag();
    
    changeDotTo(bt, tag);
}

void PlayScene::setScore(int toscore){
    ScaleTo *st = ScaleTo::create(0.1, 2.0f);
    CallFuncN *cn1 = CallFuncN::create([this,toscore](Node* nd){
        this->scoreLabel->setString(StringUtils::format("%d",toscore));
    });
    ScaleTo *stb = ScaleTo::create(0.1f, 1.0f);
    Sequence *sq = Sequence::create(st,cn1,stb,NULL);
    scoreLabel->runAction(sq);
}
//
//  GameHelper.cpp
//  LHCocosFirstGame
//
//  Created by huji on 14-8-12.
//
//

#include "GameHelper.h"
#include "LHMacros.h"
#include "UIText.h"

std::string GameHelper::textureNameFromTag(int tag){
    if (tag == T_BLACK) {
        return "black.png";
    }else if (tag == T_BLUE){
        return "blue.png";
    }else if (tag == T_GRAY){
        return "gray.png";
    }else if (tag == T_GREEN){
        return "green.png";
    }else if (tag == T_RED){
        return "red.png";
    }else{
        return nullptr;
    }
}

int GameHelper::randomTag(){
    float rand = arc4random()%100;
    if (rand < R_BLUE) {
        return T_BLUE;
    }else if (rand < R_RED){
        return T_RED;
    }else if (rand < R_GREEN){
        return T_GREEN;
    }else if (rand < R_BLAK){
        return T_BLACK;
    }else{
        return T_GRAY;
    }
}

cocos2d::ui::Layout* GameHelper::guideOfEffect(const char *info, int *ef, int len){
    Size vs = Director::getInstance()->getVisibleSize();
    auto layout = cocos2d::ui::Layout::create();
    layout->setBackGroundImage("frame.png");
    layout->setBackGroundImageScale9Enabled(true);
    float margin = 10;
    
    Sprite *sp = Sprite::create(GameHelper::textureNameFromTag(T_BLACK));
    float circleh = sp->getContentSize().width;
    
   
    ui::Text *infoT = ui::Text::create(info, Common_Font, 30);
    infoT->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    infoT->setTag(1);
    infoT->setColor(Color3B::BLACK);
    
    layout->setSize(Size(vs.width/10*9,infoT->getContentSize().height + circleh + 3*margin));
    
    Size lsz = layout->getSize();
    infoT->setPosition(Vec2(lsz.width/2, margin + infoT->getContentSize().height/2));
    
   
    for (int i=0; i<len; i++) {
        int tag = ef[i];
        Sprite *sp = Sprite::create(GameHelper::textureNameFromTag(tag));
        float x = 0;
        if (i ==0 ) {
            x = lsz.width/4;
        }else if(i==1){
            x = lsz.width/2;
        }else if (i==2){
            x = lsz.width/4*3;
        }
        sp->setPosition(Vec2(x, layout->getSize().height - sp->getContentSize().height/2 - margin));
        sp->setTag(2);
        layout->addChild(sp);
    }
    
    layout->addChild(infoT);
    return layout;
}
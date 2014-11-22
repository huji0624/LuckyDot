//
//  GameHelper.h
//  LHCocosFirstGame
//
//  Created by huji on 14-8-12.
//
//

#ifndef __LHCocosFirstGame__GameHelper__
#define __LHCocosFirstGame__GameHelper__

#include <iostream>
#include "cocos2d.h"
#include "CollectBox.h"
#include "UILayout.h"

USING_NS_CC;

class GameHelper{
public:
    static std::string textureNameFromTag(int tag);
    static int randomTag();
    
    static cocos2d::ui::Layout* guideOfEffect(const char *info,int ef[],int len);
};

#endif /* defined(__LHCocosFirstGame__GameHelper__) */

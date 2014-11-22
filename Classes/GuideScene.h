//------.h
#ifndef __GUIDESCENE__
#define __GUIDESCENE__
#include "cocos2d.h"
#include "UIButton.h"
#include "UIListView.h"
class GuideScene : public cocos2d::LayerColor
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
	virtual ~GuideScene();
	CREATE_FUNC(GuideScene);
     static cocos2d::ui::Button* guideButton(const std::string &name);
    
private:
    void addGuide(const char *info,int ef[]);
    cocos2d::ui::ListView *guideList;
};
#endif

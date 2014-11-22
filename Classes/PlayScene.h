//------.h
#ifndef __PLAYSCENE__
#define __PLAYSCENE__
#include "cocos2d.h"
#include "UILayout.h"
#include "UIText.h"
#include "CollectBox.h"
#include "UIButton.h"
USING_NS_CC;
class PlayScene : public cocos2d::LayerColor,public CollectBoxDelegate
{
public:
	static cocos2d::Scene* createScene(cocos2d::CCDictionary *dic);
	bool initDict(cocos2d::CCDictionary *dic);
    PlayScene();
	virtual ~PlayScene();
    virtual void update(float delta);
    virtual bool onEffectHappen(CollectBoxDelegate::EffectType ef);
    void dotClick(Ref *ps);
	CREATE_FUNC(PlayScene);
private:
    void changeDotTo(Ref *dot,int tag);
    void setScore(int toscore);
    ui::Button* randomDotWithType(int type);
    void playEffect(const char *content,CollectBoxDelegate::EffectType ef);
//    void showGuideWhenFirstHappen(const char *efinfo,int ef[]);
    
    void gameOver();
    ui::Layout *dotlayout;
    CollectBox *collectbox;
    ProgressTimer *progress;
    ui::Text *scoreLabel;
    ui::Text *timeReduceLabel;
    ui::Text *multiLabel;
   
    float dtm;
    int multiplyer;
    float multiTimer;
    int score;
    bool started;
};
#endif

//------.cpp
#include "CollectBox.h"
#include "GameHelper.h"
USING_NS_CC;

CollectBox::~CollectBox(){
}
bool CollectBox::initDict(cocos2d::CCDictionary *dic)
{
    this->setBackGroundImage("frame.png");
    this->setBackGroundImageScale9Enabled(true);
	return true;
}

void CollectBox::setDelegate(CollectBoxDelegate *delegate){
    _delegate = delegate;
}

#define AC_DU 0.15

void CollectBox::addMember(int member){
    
    float sw = getSize().width/3;
//    float sh = getSize().height;
    
    float insertx = sw/2;
    float inserty = getSize().height/2;
    
    Vector<Node*> children = getChildren();
    insertx = children.size()*sw + insertx;
    
    if (children.size()>=MAX_BOX_MEMBER) {
        float lastx = 0;
        
        for (int i =0; i<children.size(); i++) {
            Node *child = children.at(i);
            if (i==0) {
                child->removeFromParent();
                lastx = child->getPosition().x;
            }else{
                Vec2 cp = child->getPosition();
                MoveTo *mv = MoveTo::create(AC_DU, Vec2(lastx, cp.y));
                child->runAction(mv);
                lastx = cp.x;
            }
        }
        
        insertx = insertx - sw;
    }
    
    Sprite *dot = Sprite::create(GameHelper::textureNameFromTag(member));
    dot->setPosition(Vec2(insertx + sw, inserty));
    MoveTo *mv = MoveTo::create(AC_DU, Vec2(insertx, inserty));
    dot->runAction(mv);
    dot->setTag(member);
    this->addChild(dot);
    
    this->computeEffect();
}

cocos2d::Vector<cocos2d::Node*> CollectBox::clearMember(){
    cocos2d::Vector<cocos2d::Node*> members;
    Vector<Node*> children = getChildren();
    members.pushBack(children);
    removeAllChildren();
    return members;
}

static bool eqEffect(int src[],int effect[]){
    
    int cpe[MAX_BOX_MEMBER];
    memcpy(cpe, effect, MAX_BOX_MEMBER*sizeof(int));
    
    int count = 0;
    for (int i =0 ; i<MAX_BOX_MEMBER; i++) {
        int tag = src[i];
        for (int j=0; j<MAX_BOX_MEMBER; j++) {
            if (tag == cpe[j]) {
                cpe[j] = -1;
                count++;
                break;
            }
        }
    }
    
    if (count==MAX_BOX_MEMBER) {
        return true;
    }
    
    return false;
}

void CollectBox::computeEffect(){
    if (getChildrenCount()!=MAX_BOX_MEMBER) {
        return;
    }
    
    Vector<Node*> children = getChildren();
    int src[3];
    for (int i=0; i<MAX_BOX_MEMBER; i++) {
        src[i] = children.at(i)->getTag();
    }
    
    CollectBoxDelegate::EffectType eftype = CollectBoxDelegate::EffectType::NONE;
    
    if (eqEffect(src, EF_Freeze)) {
        eftype = CollectBoxDelegate::EffectType::FREEZE;
    }else if (eqEffect(src, EF_FIRE)){
        eftype = CollectBoxDelegate::EffectType::FIRE;
    }else if(eqEffect(src, EF_FIREG)){
        eftype = CollectBoxDelegate::EffectType::FIREG;
    }else if(eqEffect(src, EF_FIREB)){
        eftype = CollectBoxDelegate::EffectType::FIREB;
    }else if (eqEffect(src, EF_HEAL)){
        eftype = CollectBoxDelegate::EffectType::HEAL;
    }else if(eqEffect(src, EF_BLACK)){
        eftype = CollectBoxDelegate::EffectType::BLACK;
    }else if(eqEffect(src, EF_ROLLOVER)){
        eftype = CollectBoxDelegate::EffectType::ROLLOVER;
    }else if(eqEffect(src, EF_HEAL_BLUE)){
        eftype = CollectBoxDelegate::EffectType::HEAL_BLUE;
    }else if(eqEffect(src, EF_HEAL_RED)){
        eftype = CollectBoxDelegate::EffectType::HEAL_RED;
    }else if(eqEffect(src, EF_FIXDT)){
        eftype = CollectBoxDelegate::EffectType::FIXDT;
    }else if(eqEffect(src, EF_ADDMUL)){
        eftype = CollectBoxDelegate::EffectType::ADDMUL;
    }
    
    _delegate->onEffectHappen(eftype);
}


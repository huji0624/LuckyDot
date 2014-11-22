//------.h
#ifndef __COLLECTBOX__
#define __COLLECTBOX__
#include "cocos2d.h"
#include "UILayout.h"
#include "LHMacros.h"
#include "LocalizedString.h"

#define MAX_BOX_MEMBER 3

#define INFO_EF_Freeze LHLocalizedCString("info_ef_freeze")
static int EF_Freeze[] = {T_BLUE,T_BLUE,T_BLUE};

#define INFO_EF_FIRE LHLocalizedCString("info_ef_fire")
static int EF_FIRE[] = {T_RED,T_RED,T_RED};

#define INFO_EF_FIREG LHLocalizedCString("info_ef_fireg")
static int EF_FIREG[] = {T_RED,T_RED,T_GREEN};

#define INFO_EF_FIREB LHLocalizedCString("info_ef_fireb")
static int EF_FIREB[] = {T_RED,T_RED,T_BLUE};

#define INFO_EF_HEAL LHLocalizedCString("info_ef_heal")
static int EF_HEAL[] = {T_GREEN,T_GREEN,T_GREEN};

#define INFO_EF_HEAL_BLUE LHLocalizedCString("info_ef_heal_blue")
static int EF_HEAL_BLUE[] = {T_GREEN,T_GREEN,T_BLUE};

#define INFO_EF_HEAL_RED LHLocalizedCString("info_ef_heal_red")
static int EF_HEAL_RED[] = {T_GREEN,T_GREEN,T_RED};

#define INFO_EF_BLACK LHLocalizedCString("info_ef_black")
static int EF_BLACK[] = {T_GRAY,T_GRAY,T_GRAY};

#define INFO_EF_ROLLOVER LHLocalizedCString("info_ef_rollover")
static int EF_ROLLOVER[] = {T_BLUE,T_RED,T_GREEN};

#define INFO_EF_FIXDT LHLocalizedCString("info_ef_fixdt")
static int EF_FIXDT[] = {T_BLUE,T_BLUE,T_GREEN};

#define INFO_EF_ADDMUL LHLocalizedCString("info_ef_addmul")
static int EF_ADDMUL[] = {T_BLUE,T_BLUE,T_RED};

class CollectBoxDelegate{
public:
    enum class EffectType{
        NONE = -1,
        FIRE,
        FIREG,
        FIREB,
        FREEZE,
        BLACK,
        ROLLOVER,
        HEAL,
        HEAL_BLUE,
        HEAL_RED,
        FIXDT,
        ADDMUL
    };
    
    virtual bool onEffectHappen(EffectType eftype){return true;};
};

class CollectBox : public cocos2d::ui::Layout
{
public:
	bool initDict(cocos2d::CCDictionary *dic);
    void setDelegate(CollectBoxDelegate *delegate);
    void addMember(int member);
    cocos2d::Vector<cocos2d::Node*> clearMember();
	virtual ~CollectBox();
	CREATE_FUNC(CollectBox);
private:
    CollectBoxDelegate *_delegate;
    void computeEffect();
};
#endif
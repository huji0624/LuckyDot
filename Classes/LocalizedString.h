#ifndef __LocalizedString_H__
#define __LocalizedString_H__

#include "cocos2d.h"

using namespace cocos2d;

#define LHLocalizedString(__KEY__) LocalizedString::localizedString(__KEY__)
#define LHLocalizedCString(__KEY__) LocalizedString::localizedString(__KEY__)->getCString()

class LocalizedString {
    
public:
    LocalizedString();
    
    virtual ~LocalizedString();
    
    static __String* localizedString(const char *key);
    
private:
    static const char* getSystemLang();
    
};

#endif
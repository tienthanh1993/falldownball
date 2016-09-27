//
//  Define.h
//  DownWardBall
//
//  Created by Pham Thanh on 7/6/16.
//
//

#ifndef Define_h
#define Define_h

#include <cocos2d.h>
#include <ui/CocosGUI.h>
#include <cocostudio/CocoStudio.h>

using namespace cocos2d;
using namespace ui;
using namespace cocostudio;

#define CREATE_INSTANCE_FUNC(__CLASS__) \
private:\
static __CLASS__ *_gInstance;\
public:\
static __CLASS__* getInstance()\
{\
if(!_gInstance)\
{\
_gInstance = new __CLASS__();\
if(_gInstance && _gInstance->init())\
{\
_gInstance->autorelease();\
_gInstance->retain();\
}\
else\
{\
CC_SAFE_DELETE(_gInstance);\
}\
}\
return _gInstance;\
}

#define DESTROY_INSTANCE_FUNC()\
static void destroyInstance()\
{\
CC_SAFE_RELEASE_NULL(_gInstance);\
}

#define NOTIFY cocos2d::__NotificationCenter::getInstance()



#endif /* Define_h */

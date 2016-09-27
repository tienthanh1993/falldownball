#ifndef __CLOUD_H__
#define __CLOUD_H__

#include "cocos2d.h"

USING_NS_CC;

class Cloud : public Sprite
{
public:
	Cloud();
	~Cloud();

    virtual bool init();
    CREATE_FUNC(Cloud);

private:
	void moveTimer(float dt);


	CC_SYNTHESIZE(float,m_hSpeed,HSpeed);
	CC_SYNTHESIZE(float,m_vSpeed,VSpeed);

private:
};

#endif // __CLOUD_H__

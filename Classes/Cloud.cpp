#include "Cloud.h"

Cloud::Cloud()
	:m_hSpeed(-1.0f)
	,m_vSpeed(0)
{
}

Cloud::~Cloud()
{
}

bool Cloud::init()
{
	if ( !Sprite::initWithSpriteFrameName("cloud.png") )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	this->schedule(SEL_SCHEDULE(&Cloud::moveTimer));

	auto rotate = RotateBy::create(1.0f,30.0f);
	//this->runAction(RepeatForever::create(rotate));
	return true;
}


void Cloud::moveTimer(float dt)
{
	this->setPosition(this->getPosition() + Vec2(m_hSpeed,m_vSpeed));
}


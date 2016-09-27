#include "GuideLayer.h"


#define CMD_GAME_START				"game_start"		



bool GuideLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_winSize = Director::getInstance()->getVisibleSize();

	auto bg = Sprite::createWithSpriteFrameName("guide.png");
	CCASSERT(bg,"bg is create faied");
	bg->setPosition(m_winSize.width/2,m_winSize.height/2);
	this->addChild(bg);

    auto button = Button::create("know.png","","",TextureResType::PLIST);
	button->setPosition(Vec2(m_winSize.width*0.8,m_winSize.height*0.1));
	this->addChild(button);
	button->addTouchEventListener(this,SEL_TouchEvent(&GuideLayer::onButtonClicked));
	return true;
}

void GuideLayer::onButtonClicked(Ref *pSender,TouchEventType type)
{
	if (type != TOUCH_EVENT_ENDED)
	{
		return;
	}

	this->layerFlyOut();
	NotificationCenter::getInstance()->postNotification(CMD_GAME_START);

}

void GuideLayer::layerFlyIn()
{
	this->setPosition(-m_winSize.width,0);

	auto moveBy = MoveBy::create(0.5f,Vec2(m_winSize.width,0));
	auto ease = EaseExponentialOut::create(moveBy);
	auto fadeIn = FadeIn::create(0.5);
	auto spawn = Spawn::create(fadeIn,ease,NULL);
	this->runAction(spawn);
}

void GuideLayer::layerFlyOut()
{
	this->setPosition(Vec2::ZERO);

	auto moveBy = MoveBy::create(0.5f,Vec2(m_winSize.width,0));
	auto ease = EaseExponentialOut::create(moveBy);
	auto fadeOut = FadeOut::create(0.5);
	auto spawn = Spawn::create(fadeOut,ease,NULL);
	this->runAction(Sequence::create(spawn,RemoveSelf::create(),NULL));
}

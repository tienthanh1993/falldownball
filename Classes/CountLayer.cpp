#include "CountLayer.h"

#define CMD_GET_SERVER_DATA		"get_server_data"


void CountLayer::onEnter()
{
	Layer::onEnter();
	NotificationCenter::getInstance()->addObserver(this,SEL_CallFuncO(&CountLayer::getServerDataHandle),CMD_GET_SERVER_DATA,this);
}

void CountLayer::onExit()
{
	Layer::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

CountLayer::CountLayer()
{
}

CountLayer::~CountLayer()
{
}

bool CountLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
//    151.000000 457.000000,
//    score 186.000000 270.000000,
//    score 186.000000 102.000000,
    m_winSize = Director::getInstance()->getVisibleSize();
   
    auto backBtn = Button::create("backbtn.png","","",TextureResType::PLIST);
    backBtn->setPosition(Vec2(95,103));
    addChild(backBtn);
	CCASSERT(backBtn,"back button is not found");
	Vec2 desPos = backBtn->getPosition();
	backBtn->setPositionX(-backBtn->getContentSize().width/2);
	backBtn->addTouchEventListener(this,SEL_TouchEvent(&CountLayer::backBtnCallback));

	m_backBtnAction = MoveTo::create(1.0f,desPos);
	m_backBtnAction->retain();
    m_backBtnActionReverse = MoveTo::create(1.0f,backBtn->getPosition());
    m_backBtnActionReverse->retain();
	backBtn->runAction(EaseExponentialOut::create(m_backBtnAction));

    
    
     spr = Sprite::createWithSpriteFrameName("topscore.png");
    addChild(spr);
  
      spr->setPosition(Vec2(m_winSize.width/2,m_winSize.height/2+spr->getContentSize().height/2));
	spr->setPositionY(m_winSize.height+spr->getContentSize().height/2);

	m_panelAction = MoveTo::create(0.7f,Vec2(m_winSize.width/2,m_winSize.height/2+spr->getContentSize().height/4));
	m_panelAction->retain();
    m_panelActionReverse =MoveTo::create(0.7f,spr->getPosition());
    m_panelActionReverse->retain();
	spr->runAction(EaseExponentialOut::create(m_panelAction));

	// 分数文本
	m_scoreText =  TextAtlas::create("0123456789", "numrank.png", 27, 38, "0");
    m_scoreText->setPosition(Vec2(186,60));
    spr->addChild(m_scoreText);
	CCASSERT(m_scoreText,"score text is not found");
	m_scoreText->setString(StringUtils::format("%d",UserDefault::getInstance()->getIntegerForKey("score")));


    return true;
}

void CountLayer::backBtnCallback(Ref *pSender,TouchEventType type)
{
	if (type != TOUCH_EVENT_ENDED)
	{
		return;
	}

	NotificationCenter::getInstance()->postNotification(CMD_SHOW_MAIN_MENU);

	auto backBtn = (Button *)pSender;
	backBtn->runAction(EaseExponentialOut::create(m_backBtnActionReverse));

	spr->runAction(Sequence::createWithTwoActions(EaseExponentialOut::create(m_panelActionReverse),CallFunc::create([&](){
		this->removeFromParent();
	})));

}

void CountLayer::updateRankText(int rank)
{
//	m_rankText->setString(StringUtils::format("%d",rank));
}

/* get rank data 消息处理函数 */
void CountLayer::getServerDataHandle(Ref *pSender)
{
///	updateRankText(UserDefault::getInstance()->getIntegerForKey("score"));
}
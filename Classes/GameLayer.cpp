#include "GameLayer.h"
#include "CountLayer.h"
#include "SimpleAudioEngine.h"
#include "GuideLayer.h"
#include "AdsIntergrate.h"

using namespace CocosDenshion;


extern bool g_musicToogle;

const int Tag_start_btn = 20;
const int Tag_music_btn = 21;
const int Tag_rank_btn = 22;

GameLayer::GameLayer()
	:m_score(nullptr)
	,m_startBtn(nullptr)
	,m_rankBtn(nullptr)
	,m_musicBtn(nullptr)
{

}

GameLayer::~GameLayer()
{

}
void GameLayer::adViewDidReceiveAd(const std::string &name) {
//    if (name == "home") {
//        Ads::showAdmob("home");
//    }
}
void GameLayer::onEnter()
{
	Layer::onEnter();
	NotificationCenter::getInstance()->addObserver(this,SEL_CallFuncO(&GameLayer::gameOverHandle),CMD_GAME_OVER,this);
	NotificationCenter::getInstance()->addObserver(this,SEL_CallFuncO(&GameLayer::showMainMenuHandle),CMD_SHOW_MAIN_MENU,this);

	updateScoreText(UserDefault::getInstance()->getIntegerForKey("score",0));

}

void GameLayer::onExit()
{
	Layer::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

bool GameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
#ifdef SDKBOX_ENABLED
    sdkbox::PluginAdMob::setListener(this);
#endif
    Ads::preload();
//    start btn 332.000000 517.000000
//    rank btn 553.000000 98.000000
//    music btn 77.000000 95.000000
    m_winSize = Director::getInstance()->getVisibleSize();
   
	/* 开始按钮 */
    m_startBtn = Button::create("playbtn.png","","",TextureResType::PLIST);
    m_startBtn->setPosition(Vec2(332,517));
    addChild(m_startBtn);
	CCASSERT(m_startBtn,"start button is not found");
	m_startBtn->setTouchEnabled(false);
	m_startBtn->setTag(Tag_start_btn);
	Vec2 desPos = m_startBtn->getPosition();
    log("start btn %f %f",desPos.x,desPos.y);
    
	m_startBtn->addTouchEventListener(this,SEL_TouchEvent(&GameLayer::onButtonClicked));
	m_startBtn->setPositionX(m_winSize.width+m_startBtn->getContentSize().width/2);
    m_startBtnAnimationReverse = MoveTo::create(1.0f, m_startBtn->getPosition());
    m_startBtnAnimationReverse->retain();
	m_startBtnAnimation = MoveTo::create(1.0f,desPos);
	m_startBtnAnimation->retain();
	auto ease = EaseExponentialOut::create(m_startBtnAnimation);
	m_startBtn->runAction(Sequence::create(ease,CallFunc::create([&](){
		NotificationCenter::getInstance()->postNotification(CMD_GAME_LOAD_FINISH);
	}),NULL));

	/* 排行按钮 */
    m_rankBtn = Button::create("rankbtn.png","","",TextureResType::PLIST);//dynamic_cast<Button *>(Helper::seekWidgetByName(uiwidget,"rankbtn"));
    addChild(m_rankBtn);
    CCASSERT(m_rankBtn,"rank button is not found");
    m_rankBtn->setPosition(Vec2(553,98));
	m_rankBtn->setTouchEnabled(false);
	m_rankBtn->setTag(Tag_rank_btn);
	desPos = m_rankBtn->getPosition();
	m_rankBtn->addTouchEventListener(this,SEL_TouchEvent(&GameLayer::onButtonClicked));
	m_rankBtn->setPositionY(-m_rankBtn->getContentSize().height/2);

	m_rankBtnAnimation = MoveTo::create(1.0f,desPos);
	m_rankBtnAnimation->retain();
    m_rankBtnAnimationReverse = MoveTo::create(1.0f, m_rankBtn->getPosition());
    m_rankBtnAnimationReverse->retain();
	ease = EaseExponentialOut::create(m_rankBtnAnimation);
	m_rankBtn->runAction(ease);

	/* 音乐按钮 */
    m_musicBtn = Button::create("soundbtn_e.png","soundbtn_d.png","",TextureResType::PLIST);
    m_musicBtn->setPosition(Vec2(77,95));
    addChild(m_musicBtn);
    
	CCASSERT(m_musicBtn,"music button is not found");
	m_musicBtn->setTouchEnabled(false);
	m_musicBtn->setTag(Tag_music_btn);
    if (g_musicToogle)
        m_musicBtn->setBright(true);
    else
        m_musicBtn->setBright(false);
	desPos = m_musicBtn->getPosition();
	m_musicBtn->addTouchEventListener(this,SEL_TouchEvent(&GameLayer::onButtonClicked));
	m_musicBtn->setPositionX(-m_musicBtn->getContentSize().width/2);

	m_musicBtnAnimation = MoveTo::create(1.0f,desPos);
	m_musicBtnAnimation->retain();
    m_musicBtnAnimationReverse = MoveTo::create(1.0f,m_musicBtn->getPosition());
    m_musicBtnAnimationReverse->retain();
    
	ease = EaseExponentialOut::create(m_musicBtnAnimation);
	m_musicBtn->runAction(ease);
//561.000000 1081.000000
	/* 分数面板 */
    m_score = TextAtlas::create("0123456789", "numrank.png", 27, 38, "0");
    addChild(m_score);
    m_score->setTag(8);
    m_score->setPosition(Vec2(561,1081));
	CCASSERT(m_score,"socre text is not found");

    m_Logo = Sprite::createWithSpriteFrameName("logo.png");
    m_Logo->setPosition(m_winSize.width/2,m_winSize.height*0.7);
    addChild(m_Logo);
    

    
    
	m_logoAnimation = MoveTo::create(1.5f,Vec2(m_winSize.width/2,m_winSize.height*1.1));
	m_logoAnimation->retain();
    m_logoAnimationReverse = MoveTo::create(1.5f,m_Logo->getPosition());
    m_logoAnimationReverse->retain();
    
    
    auto callback = CallFuncN::create(
                                      [&](Node* sender)
                                      {
                                          m_startBtn->setTouchEnabled(true);
                                          m_rankBtn->setTouchEnabled(true);
                                          m_musicBtn->setTouchEnabled(true);
                                      });
    m_Logo->runAction(Sequence::create(EaseExponentialOut::create(m_logoAnimationReverse),callback, NULL));
    return true;
}

void GameLayer::updateScoreText(int score)
{
	m_score->setString(StringUtils::format("%d",score));
}

void GameLayer::onButtonClicked(Ref *pSender,TouchEventType type)
{
	if (type != TOUCH_EVENT_ENDED)
	{
		return;
	}

	int tag = ((Button *)pSender)->getTag();

	switch (tag)
	{
	case  Tag_start_btn :
		{

			m_startBtn->runAction(EaseExponentialOut::create(m_startBtnAnimationReverse));
			m_startBtn->setTouchEnabled(false);
			m_rankBtn->runAction(EaseExponentialOut::create(m_rankBtnAnimationReverse));
			m_rankBtn->setTouchEnabled(false);
			m_musicBtn->runAction(EaseExponentialOut::create(m_musicBtnAnimationReverse));
			m_musicBtn->setTouchEnabled(false);
			m_Logo->runAction(EaseExponentialOut::create(m_logoAnimation));

			// 判断如果是第一次进入游戏显示指引
			if (UserDefault::getInstance()->getBoolForKey("first_game",true))
			{
				UserDefault::getInstance()->setBoolForKey("first_game",false);
				auto guideLayer = GuideLayer::create();
				guideLayer->setPosition(-m_winSize.width,0);
				this->addChild(guideLayer);

				guideLayer->layerFlyIn();
			}
			else
			{
				NotificationCenter::getInstance()->postNotification(CMD_GAME_START);
			}
		}
		break;
	case Tag_music_btn :
		{
       
			if (g_musicToogle)
			{
                UserDefault::getInstance()->setBoolForKey("sound",false);
				g_musicToogle = false;
				m_musicBtn->setBright(false);
				SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
				SimpleAudioEngine::getInstance()->stopAllEffects();
			}
			else
			{
                UserDefault::getInstance()->setBoolForKey("sound",true);
				g_musicToogle = true;
				m_musicBtn->setBright(true);
                
                    SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg.mp3",true);
            
			}
		}
		break;
	case Tag_rank_btn :
		{
			auto countLayer = CountLayer::create();
			this->addChild(countLayer);

			m_startBtn->runAction(EaseExponentialOut::create(m_startBtnAnimationReverse));
			m_rankBtn->runAction(EaseExponentialOut::create(m_rankBtnAnimationReverse));
			m_musicBtn->runAction(EaseExponentialOut::create(m_musicBtnAnimationReverse));
			m_Logo->runAction(EaseExponentialOut::create(m_logoAnimation));

		}
		break;
	default:
		break;
	}

	
}

void GameLayer::gameOverHandle(Ref *pSender)
{
    Ads::FULLSCREEN_COUNT++;
    if (Ads::FULLSCREEN_COUNT%3== 0 && Ads::isAdmobAvailable("gameover")) {
            Ads::showAdmob("gameover");
    }
    
	updateScoreText(UserDefault::getInstance()->getIntegerForKey("score"));

	m_startBtn->runAction(Sequence::create(EaseExponentialOut::create(m_startBtnAnimation),CallFunc::create([&](){
		NotificationCenter::getInstance()->postNotification(CMD_GAME_LOAD_FINISH);
	}),NULL));

	m_rankBtn->runAction(EaseExponentialOut::create(m_rankBtnAnimation));
	m_musicBtn->runAction(EaseExponentialOut::create(m_musicBtnAnimation));
    
    auto callback = CallFuncN::create(
                                      [&](Node* sender)
    {
        
        m_startBtn->setTouchEnabled(true);
        m_rankBtn->setTouchEnabled(true);
        m_musicBtn->setTouchEnabled(true);
    });
    m_Logo->runAction(Sequence::create(EaseExponentialOut::create(m_logoAnimationReverse),callback, NULL));
//	m_Logo->setPosition(m_winSize.width/2,m_winSize.height*0.7);
	//m_Logo->getAnimation()->play("In");
}

void GameLayer::playEndCallfnuc(Armature *armature, MovementEventType type, const std::string &str)
{
	if (type == MovementEventType::COMPLETE)
	{
		m_startBtn->setTouchEnabled(true);
		m_rankBtn->setTouchEnabled(true);
		m_musicBtn->setTouchEnabled(true);


	}
}

void GameLayer::showMainMenuHandle(Ref *pSender)
{
	m_startBtn->runAction(EaseExponentialOut::create(m_startBtnAnimation));
	m_rankBtn->runAction(EaseExponentialOut::create(m_rankBtnAnimation));
	m_musicBtn->runAction(EaseExponentialOut::create(m_musicBtnAnimation));

	m_Logo->runAction(EaseExponentialOut::create(m_logoAnimationReverse));
}


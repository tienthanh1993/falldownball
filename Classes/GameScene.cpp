#include "GameScene.h"
#include "BorderPool.h"

/*  */
const float g_borderSize = 50.0;

/* 每层得分 */
const int g_scorePerLayer = 2;

/* 全局的声音开关 */
bool g_musicToogle = true;

/* 各个层的 Z_order */
const int Z_order_bg			= -255;		// 背景
const int Z_order_cloud		= 10;		// 白云
const int Z_order_border   = 20;		// 踏板
const int Z_order_player    = 30;		// 玩家
const int Z_order_ceiling    = 40;	// 天花板
const int Z_order_UI         = 255;	// UI层

const int g_base_speed_increment = 20.0f;  // 速度增量

GameScene::GameScene()
	:m_player(nullptr)
	,m_gameLayer(nullptr)
	,m_curScore(0)
	,m_curLayerIndex(0)
	,m_prevLayerIndex(0)
	,m_vSpeed(150)
	,m_curLevel(0)
	,m_world(nullptr)
	,m_gameState(GAME_OVER)
	,m_exitGame(false)
{
	m_borders.clear();

	m_clouds.clear();

	m_destroySet.clear();
}

GameScene::~GameScene()
{
	m_borders.clear();
	m_clouds.clear();
	CC_SAFE_DELETE(m_world);
	m_destroySet.clear();
}

void GameScene::onEnter()
{
	Scene::onEnter();
  
	BorderPool::getInstance()->initPool(50,m_world);

	NotificationCenter::getInstance()->addObserver(this,SEL_CallFuncO(&GameScene::gameStartHandle),CMD_GAME_START,NULL);
	NotificationCenter::getInstance()->addObserver(this,SEL_CallFuncO(&GameScene::gameLoadFinishHandle),CMD_GAME_LOAD_FINISH,NULL);
    if(g_musicToogle)
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bg.mp3",true);


}

void GameScene::onExit()
{
	Scene::onExit();
	NotificationCenter::getInstance()->removeAllObservers(this);
}

void GameScene::preloadSource() {
    _pSourceVec->push_back("common");
     _pSourceVec->push_back("topscene");
 

    for(auto &iterSourceName : *_pSourceVec)
        Director::getInstance()->getTextureCache()->addImageAsync(iterSourceName + ".png", [&](Texture2D *pTexture)
                                                                  {
                                                                      ++this->_iSourceCount;
       SpriteFrameCache::getInstance()->addSpriteFramesWithFile(iterSourceName + ".plist" , pTexture);
                                                                      if(this->_iSourceCount == _pSourceVec->size())
                                                                      {
                                                                              Ads::init();
                                                                          SimpleAudioEngine::getInstance()->preloadBackgroundMusic("Music/bg.mp3");
                                                                          this->addLayer();
                                                                         NotificationCenter::getInstance()->postNotification("loadFinish", static_cast<Ref*>(this));
                                                                      }
                                                                  });
}

void GameScene::addLayer() {
    m_winSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 初始化物理世界
    initPhysics();
    
    m_gameLayer = GameLayer::create();
    m_gameLayer->setZOrder(Z_order_UI);
    this->addChild(m_gameLayer);
    
    /* 主角 */
    m_player = Player::create(m_world);
    this->addChild(m_player,Z_order_player);
    auto playerBody = m_player->getBody();
    playerBody->SetActive(false);
    
    
    
    /* 背景 */
    auto bgImg = Sprite::createWithSpriteFrameName("bg.png");
    bgImg->setPosition(m_winSize.width/2,m_winSize.height/2);
    bgImg->setZOrder(Z_order_bg);
    this->addChild(bgImg);
    
    /* 初始化随机种子 */
    timeval psv;
    gettimeofday(&psv,NULL);
    unsigned int tsrans = psv.tv_sec*1000 + psv.tv_usec/1000;
    srand(tsrans);
    
    /* 触摸事件监听器 */
    m_touchEventListener = EventListenerTouchOneByOne::create();
    m_touchEventListener->retain();
    m_touchEventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
    m_touchEventListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded,this);
    
    /* 键盘事件监听器 */
    m_keyBoardListener = EventListenerKeyboard::create();
    m_keyBoardListener->retain();
    m_keyBoardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed,this);
    m_keyBoardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_keyBoardListener,this);
    
    /* 启动创建白云定时器 */
    this->schedule(SEL_SCHEDULE(&GameScene::createCloudTimer),5.0f);
    
    this->schedule(SEL_SCHEDULE(&GameScene::checkCloudLeaveScreen));
    
    //this->schedule(SEL_SCHEDULE(&GameScene::updateRankTimer),5.0f);
    
    
    scheduleUpdate();
}
bool GameScene::init()
{
 
	if ( !Scene::init() )
	{
		return false;
	}
    _pSourceVec = new std::vector<std::string>;
	g_musicToogle = UserDefault::getInstance()->getBoolForKey("sound",true);
    preloadSource();
	return true;
}

void GameScene::initPhysics()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	b2Vec2 gravity;
	gravity.Set(0,-9.8*5);
	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(true);
	m_world->SetContinuousPhysics(true);
	m_world->SetContactListener(this);

#if(DEBUG_DRAW == 1)
	_debugDraw = new (std::nothrow) GLESDebugDraw( PTM_RATIO );
	m_world->SetDebugDraw(_debugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	//        flags += b2Draw::e_jointBit;
	//        flags += b2Draw::e_aabbBit;
	//        flags += b2Draw::e_pairBit;
	//        flags += b2Draw::e_centerOfMassBit;
	_debugDraw->SetFlags(flags);
#endif

	b2BodyDef bodyDef;
	b2EdgeShape edgeShape;
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;
	

	// 死亡区域 ===========》》
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0,0);
	auto deadBody = m_world->CreateBody(&bodyDef);
	deadBody->SetGravityScale(0);
	auto deadBodyNode = Sprite::create();
	deadBodyNode->setTag(Tag_deadline);

	// 底部
	polygonShape.SetAsBox(visibleSize.width/(2 * PTM_RATIO),g_borderSize/(2 * PTM_RATIO),b2Vec2(visibleSize.width/(2 * PTM_RATIO),(-200)/PTM_RATIO),0);
	fixtureDef.shape = &polygonShape;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = DEAD_FIELD;
	fixtureDef.filter.maskBits = BALL | BORDER;
	deadBody->CreateFixture(&fixtureDef);
	fixtureDef.isSensor = false;

	// 顶部
	polygonShape.SetAsBox(visibleSize.width/(2 * PTM_RATIO),g_borderSize/(2 * PTM_RATIO),b2Vec2(visibleSize.width/(2 * PTM_RATIO),(visibleSize.height+200)/PTM_RATIO),0);
	fixtureDef.shape = &polygonShape;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = DEAD_FIELD;
	fixtureDef.filter.maskBits = BALL | BORDER;
	deadBody->CreateFixture(&fixtureDef);
	fixtureDef.isSensor = false;

	// 游戏边界 ============》》
	bodyDef.type = b2_staticBody;
	auto worldBody = m_world->CreateBody(&bodyDef);
	auto worldBodyNode = Sprite::create();
	worldBodyNode->retain();
	worldBodyNode->setTag(Tag_game_border);

	// 左侧
	edgeShape.Set(b2Vec2(0,-200/PTM_RATIO),b2Vec2(0,(visibleSize.height+200)/PTM_RATIO));
	fixtureDef.shape = &edgeShape;
	fixtureDef.friction = 0;
	fixtureDef.filter.categoryBits = WORLD_BORDER;
	fixtureDef.filter.maskBits = BALL;
	worldBody->CreateFixture(&fixtureDef);

	// 右侧
	edgeShape.Set(b2Vec2(visibleSize.width/PTM_RATIO,-200/PTM_RATIO),b2Vec2(visibleSize.width/PTM_RATIO,(visibleSize.height+200)/PTM_RATIO));
	fixtureDef.shape = &edgeShape;
	fixtureDef.friction = 0;
	fixtureDef.filter.categoryBits = WORLD_BORDER;
	fixtureDef.filter.maskBits = BALL;
	worldBody->CreateFixture(&fixtureDef);
	//《《 ==============

	/* 天花板 */
	auto ceiling = Sprite::createWithSpriteFrameName("ceiling.png");
	ceiling->setTag(Tag_ceiling);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(m_winSize.width/(2*PTM_RATIO),(m_winSize.height-ceiling->getContentSize().height/2)/PTM_RATIO);
	polygonShape.SetAsBox(ceiling->getContentSize().width/(PTM_RATIO * 2),ceiling->getContentSize().height/(PTM_RATIO * 2));
	fixtureDef.shape = &polygonShape;
	fixtureDef.filter.categoryBits = CEILLING;
	fixtureDef.filter.maskBits = BALL;
	auto ceilingBody = m_world->CreateBody(&bodyDef);
	ceilingBody->CreateFixture(&fixtureDef);
	ceilingBody->SetUserData(ceiling);

	ceiling->setPosition(m_winSize.width/2,m_winSize.height-ceiling->getContentSize().height/2);
	this->addChild(ceiling,Z_order_ceiling);
}

void GameScene::BeginContact(b2Contact* contact)
{
	auto fixtureA = contact->GetFixtureA();
	auto fixtureB = contact->GetFixtureB();

	int addMask = fixtureA->GetFilterData().categoryBits + fixtureB->GetFilterData().categoryBits;
	if (addMask == 6 || addMask == 10)
	{  // 玩家和死亡区域相撞 或 玩家和天花板相撞
           if(g_musicToogle)
			SimpleAudioEngine::getInstance()->playEffect("Music/pop.mp3");
		
		if (m_curScore > UserDefault::getInstance()->getIntegerForKey("score"))
		{
			UserDefault::getInstance()->setIntegerForKey("score",m_curScore);
		}

		

		m_gameState = GAME_OVER;

		// 清空踏板数组
		for (auto iter : m_borders)
		{
			if (iter != nullptr)
			{
				m_destroySet.push_back(iter);
			}
		}
		m_borders.clear();
		
		// 移除事件监听器
		_eventDispatcher->removeEventListener(m_touchEventListener);

		// 关闭定时器 
		this->unschedule(SEL_SCHEDULE(&GameScene::createBorderTimer));

		// 白云停止向上运动
		for (auto & citer : m_clouds)
		{
			citer->setVSpeed(0);
		}

		NotificationCenter::getInstance()->postNotification(CMD_GAME_OVER);
	}
	else if (addMask == 18)
	{  // 玩家和踏板相撞
		if (contact->GetManifold()->localNormal.y > 0)
		{
			auto border = (fixtureA->GetFilterData().categoryBits == 16) ? ((Border *)fixtureA->GetBody()->GetUserData()) : ((Border *)fixtureB->GetBody()->GetUserData());

			if (m_gameState == GAME_START && !border->getColliend())
			{
                if(g_musicToogle)
					SimpleAudioEngine::getInstance()->playEffect("Music/jump.mp3");
				// 将踏板设置为已碰撞
				border->setColliend(true);

				m_curScore += (border->getLayerIndex() - m_prevLayerIndex) * g_scorePerLayer;
				m_gameLayer->updateScoreText(m_curScore);
				/*if (m_curScore > UserDefault::getInstance()->getIntegerForKey("score"))
				{
					UserDefault::getInstance()->setIntegerForKey("score",m_curScore);
				}*/

				m_prevLayerIndex = border->getLayerIndex();

				int level = (m_curScore / 100);

				// 速度等级提升了
				if (level > m_curLevel)
				{
					m_curLevel = level;
					m_vSpeed += (m_curLevel * g_base_speed_increment);
					levelIncrement();
				}
			}
		}
		
	}
	else if (addMask == 20)
	{  // 踏板和死亡区域相撞
		auto border = (fixtureA->GetFilterData().categoryBits == 16) ? ((Border *)fixtureA->GetBody()->GetUserData()) : ((Border *)fixtureB->GetBody()->GetUserData());
		if (border != nullptr)
		{
			m_destroySet.push_back(border);
			for(auto iter = m_borders.begin();iter != m_borders.end();iter++)
			{
				if (*iter != nullptr && *iter == border)
				{
					m_borders.erase(iter);
					break;
				}
			}
		}
	}
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	Vec2 pos = touch->getLocation();
	if (pos.x < m_winSize.width/2)
	{
		m_player->setMoveState(Player::MS_LEFT);
	}
	else
	{
		m_player->setMoveState(Player::MS_RIGHT);
	}
	return true;
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	m_player->setMoveState(Player::MS_STOP);
}


void GameScene::update(float dt)
{
	if (m_gameState == GAME_EXIT)
	{
		Director::getInstance()->end();
	}

	float timeStep = 1/60.0;
	int32 velocityInterations = 8;
	int32 positionInterations = 1;

	if (m_gameState != GAME_EXIT)
	{
		m_world->Step(timeStep,velocityInterations,positionInterations);
	}

	for (auto body = m_world->GetBodyList();body;body = body->GetNext())
	{
		if (body->GetUserData() != nullptr)
		{
			auto node = (Sprite *)body->GetUserData();
			node->setPosition(body->GetPosition().x*PTM_RATIO,body->GetPosition().y*PTM_RATIO);
			node->setRotation(-1*CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}
	}

	for (auto iter : m_destroySet)
	{
		if (iter != nullptr)
		{
			((Border *)iter)->removeFromParent();

			sBorderPool->pushOneBorder((Border *)iter);
		}
	}
	m_destroySet.clear();

	if (m_gameState == GAME_OVER)
	{
        m_player->setTexture(__String::createWithFormat("Player/p%d.png",cocos2d::random(1, 32))->getCString());
		m_player->setMoveState(Player::MS_STOP);
		auto body = m_player->getBody();
		body->SetLinearVelocity(b2Vec2(0,0));
		body->SetAngularVelocity(0);
		body->SetTransform(b2Vec2(m_winSize.width/(2 * PTM_RATIO),-m_player->getContentSize().height/ PTM_RATIO),0);
		body->SetActive(false);
	}

	
}


#if(DEBUG_DRAW == 1)
void GameScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	m_world->DrawDebugData();
}
#endif

void GameScene::createBorderTimer(float dt)
{
	m_curLayerIndex++;
	int i = rand_0_1()*4+1;

	auto border = sBorderPool->popBorder();
	border->setLayerIndex(m_curLayerIndex);
	this->addChild(border,Z_order_border);

	auto body = border->getBody();
	body->SetTransform(b2Vec2((m_winSize.width * (2*i-1))/(8 * PTM_RATIO),-border->getContentSize().height/(2 * PTM_RATIO)),0);
	body->SetLinearVelocity(b2Vec2(0,m_vSpeed/PTM_RATIO));
	body->SetActive(true);
	m_borders.push_back(border);
}

void GameScene::createCloudTimer(float dt)
{
	int rand = rand_0_1()*16 - 5;

	auto cloud = Cloud::create();
	cloud->setPosition(m_winSize.width*1.2,m_winSize.height*0.1*rand);
	if (m_gameState == GAME_START)
	{
		cloud->setVSpeed(m_vSpeed/100);
	}
	this->addChild(cloud,Z_order_cloud);
	m_clouds.pushBack(cloud);
}

void GameScene::checkCloudLeaveScreen(float dt)
{
	for (auto iter = m_clouds.begin();iter != m_clouds.end();)
	{
		auto cloud = (Cloud *)(*iter);
		if (cloud->getPositionX() < -m_winSize.width*0.2)
		{
			cloud->removeFromParent();
			iter = m_clouds.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event *event)
{
	if (m_gameState != GAME_START)
	{
		return;
	}
	if (code == EventKeyboard::KeyCode::KEY_A)
	{
		m_player->setMoveState(Player::MS_LEFT);
	}
	else if (code == EventKeyboard::KeyCode::KEY_D)
	{
		m_player->setMoveState(Player::MS_RIGHT);
	}
}

void GameScene::showTips()
{
	auto tips = Sprite::createWithSpriteFrameName("quit.png");
	tips->setPosition(m_winSize.width/2,m_winSize.height*0.1f);
	this->addChild(tips,Z_order_UI);

	auto moveBy = MoveBy::create(0.5f,Vec2(0,100));

	auto ease = EaseBackOut::create(moveBy);
	auto fadeIn = FadeIn::create(0.5f);
	auto spawn = Spawn::createWithTwoActions(fadeIn,ease);

	auto callfunc = CallFunc::create([=](){
		auto fadeout = FadeOut::create(0.5f);
		tips->runAction(Sequence::create(fadeout,RemoveSelf::create(),NULL));
		m_exitGame = false;
	});

	tips->runAction(Sequence::create(spawn,DelayTime::create(1.0f),callfunc,NULL));

}

void GameScene::updateRankTimer(float dt)
{

}


void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event *event)
{
	if (code == EventKeyboard::KeyCode::KEY_BACK)
	{
		/*if (NetTest::getInstance()->spotViewIsVisible())
		{
			NetTest::getInstance()->setSpotAdViewVisible(false);
			return;
		}*/
		if (m_exitGame)
		{
			m_gameState = GAME_EXIT;
		}
		else
		{
			// 显示提示
			m_exitGame = true;
			showTips();
		}
	}
	if (m_gameState == GAME_START)
	{
		m_player->setMoveState(Player::MS_STOP);
	}
}

void GameScene::gameStartHandle(Ref *pSender)
{
	m_gameState = GAME_START;
	m_curScore = 0;
	m_curLayerIndex = 0;
	m_curLevel = 0;
	m_prevLayerIndex = 0;
	m_vSpeed = 150;

	// 注册监听事件
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchEventListener,this);
	
	levelIncrement();

	// 启动产生border的定时器
	this->schedule(SEL_SCHEDULE(&GameScene::createBorderTimer),2.5f);

	m_gameLayer->updateScoreText(0);

	// 显示广告
}

void GameScene::gameLoadFinishHandle(Ref *pSender)
{
	m_gameState = GAME_PREPARED;

	auto border = sBorderPool->popBorder();
	border->setLayerIndex(0);
	this->addChild(border,Z_order_border);
	m_borders.push_back(border);
	border->moveToPoint(b2Vec2((m_winSize.width*1.1)/PTM_RATIO,(m_winSize.height*0.08)/PTM_RATIO),b2Vec2(m_winSize.width/(2 * PTM_RATIO),(m_winSize.height*0.08)/PTM_RATIO),0.5f);
	border->getBody()->SetActive(true);

	auto playerBody = m_player->getBody();
	playerBody->SetLinearVelocity(b2Vec2(0,0));
	playerBody->SetAngularVelocity(0);
	playerBody->SetTransform(b2Vec2(m_winSize.width/(PTM_RATIO * 2),(m_winSize.height*0.45f)/PTM_RATIO),0);
	playerBody->SetActive(true);
}

void GameScene::levelIncrement()
{
	for (auto &iter : m_borders)
	{
		auto border = (Border *)iter;
		border->getBody()->SetLinearVelocity(b2Vec2(0,m_vSpeed/PTM_RATIO));
	}

	for (auto & citer : m_clouds)
	{
		citer->setVSpeed(m_vSpeed/100);
	}

}




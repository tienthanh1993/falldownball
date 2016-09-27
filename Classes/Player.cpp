#include "Player.h"
#include "GameScene.h"

#define PTM_RATIO 32


Player::Player()
	:m_blink(nullptr)
	,m_body(nullptr)
	,m_world(nullptr)
{
}

Player::~Player()
{
}

Player *Player::create(b2World *world)
{
	auto bRet = new Player;
	if (bRet && bRet->init(world))
	{
		bRet->autorelease();
		return bRet;
	}
	CC_SAFE_DELETE(bRet);
	return nullptr;

}

bool Player::init(b2World *world)
{
    if ( !Sprite::initWithFile(__String::createWithFormat("Player/p%d.png",cocos2d::random(1, 32))->getCString()))
    {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(visibleSize.width/(2 * PTM_RATIO),-this->getContentSize().height/PTM_RATIO);
	this->setTag(Tag_player);

	b2CircleShape circleShape;
	circleShape.m_radius = getContentSize().width/(PTM_RATIO * 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 0.06f;
	fixtureDef.restitution = 0.2f;
	fixtureDef.friction = 1.0f;

	// 我是小球，我可以和踏板，边界，死亡区域，天花板相撞
	fixtureDef.filter.categoryBits = BALL;
	fixtureDef.filter.maskBits = BORDER | WORLD_BORDER | DEAD_FIELD | CEILLING;

	m_body = world->CreateBody(&bodyDef);
	m_body->CreateFixture(&fixtureDef);
	m_body->SetUserData(this);
	m_moveState = MS_STOP;
    
	this->schedule(SEL_SCHEDULE(&Player::moveTimer));

	return true;
}

void Player::blinkTimer(float dt)
{
}

void Player::moveTimer(float dt)
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	float desiredVel = 0;
	switch ( m_moveState )
	{
	case MS_LEFT:  desiredVel = b2Max( vel.x - 1.0f, -20.0f ); break;
	case MS_STOP:  desiredVel = vel.x * 0.98f; break;
	case MS_RIGHT: desiredVel = b2Min( vel.x + 1.0f,  20.0f ); break;
	}
	float velChange = desiredVel - vel.x;
	float impulse = m_body->GetMass() * velChange; //disregard time factor
	m_body->ApplyLinearImpulse( b2Vec2(impulse,0), m_body->GetWorldCenter() ,true);
}



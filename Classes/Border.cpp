#include "Border.h"
#include "Box2D/Box2D.h"
#include "GameScene.h"

#define PTM_RATIO 32

Border::Border()
	:m_layerIndex(0)
	,m_colliend(false)
	,m_vSpeed(0.0f)
	,m_body(nullptr)
{

}

Border::~Border()
{

}

Border *Border::create(b2World *world)
{
	auto bRet = new Border;
	if (bRet && bRet->init(world))
	{
		bRet->autorelease();
		return bRet;
	}

	CC_SAFE_DELETE(bRet);
	return nullptr;
}

bool Border::init(b2World *world)
{
	if ( !Sprite::initWithSpriteFrameName("border.png") )
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	b2BodyDef bodyDef;
	bodyDef.position.Set(visibleSize.width/(2 * PTM_RATIO),-this->getContentSize().height/(2 * PTM_RATIO));
	bodyDef.type = b2_kinematicBody;
	this->setTag(Tag_border);

	b2PolygonShape shape;
	shape.SetAsBox(this->getContentSize().width/(PTM_RATIO*2),(this->getContentSize().height - 14)/(PTM_RATIO*2));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.friction = 1.0f;



	// 我是踏板，我可以和小球，死亡区域相撞
	fixtureDef.filter.categoryBits = BORDER;
	fixtureDef.filter.maskBits = BALL | DEAD_FIELD;

	m_body = world->CreateBody(&bodyDef);
	m_body->CreateFixture(&fixtureDef);
	m_body->SetUserData(this);

	return true;
}

void Border::moveToPoint(const b2Vec2 &from,const b2Vec2 &to,float interval)
{
	m_body->SetTransform(from,m_body->GetAngle());
	m_startPoint = from;
	m_desPoint = to;
	m_times = interval * 60;
	m_hSpeedDelta = (to.x - from.x)/(m_times);
	m_vSpeedDelta = (to.y - from.y)/(m_times);

	this->schedule(SEL_SCHEDULE(&Border::moveTimer));
}

void Border::moveTimer(float dt)
{
	if (m_times != 0)
	{
		m_times--;
		b2Vec2 vel = m_body->GetPosition();
		vel.Set(vel.x + m_hSpeedDelta,vel.y + m_vSpeedDelta);
		m_body->SetTransform( vel,m_body->GetAngle() );
	}
	else
	{
		this->unschedule(SEL_SCHEDULE(&Border::moveTimer));
	}
}




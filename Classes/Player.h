#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class Player : public Sprite
{
public:
	enum moveState {
		MS_STOP,
		MS_LEFT,
		MS_RIGHT,
	};

public:
	Player();
	~Player();

	virtual bool init(b2World *world);
	static Player *create(b2World *world);

	b2Body *getBody(){return m_body;}

	void setMoveState(moveState state){m_moveState = state;}

private:
	/* 眨眼定时器 */
	void blinkTimer(float dt);
	/* 移动定时器 */
	void moveTimer(float dt);

private:
	Animation *m_blink;

	b2World *m_world;
	b2Body *m_body;

	moveState m_moveState;
};

#endif // __PLAYER_H__

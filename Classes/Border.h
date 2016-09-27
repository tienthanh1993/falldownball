#ifndef __BORDER_H__
#define __BORDER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class Border : public Sprite
{
public:
	Border();
	~Border();

	virtual bool init(b2World *world);
	static Border *create(b2World *world);

	b2Body *getBody(){return m_body;}

	void moveToPoint(const b2Vec2 &from,const b2Vec2 &to,float interval);
	void moveTimer(float dt);

	/* 设置移动速度 */
	CC_SYNTHESIZE(float,m_vSpeed,VSpeed);

	/* 层索引 */
	CC_SYNTHESIZE(int,m_layerIndex,LayerIndex);	

	/* 是否已经碰撞了 */
	CC_SYNTHESIZE(bool,m_colliend,Colliend);

private:
	b2Body *m_body;

	b2Vec2 m_startPoint;	// 起始位置，移动时候有效
	b2Vec2 m_desPoint;		// 目标点，移动时候有效
	float m_hSpeedDelta;	// 横向速度增量
	float m_vSpeedDelta;	// 纵向速度增量
	int m_times;			// 次数
};

#endif // __BORDER_H__

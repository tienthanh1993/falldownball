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

	/* �����ƶ��ٶ� */
	CC_SYNTHESIZE(float,m_vSpeed,VSpeed);

	/* ������ */
	CC_SYNTHESIZE(int,m_layerIndex,LayerIndex);	

	/* �Ƿ��Ѿ���ײ�� */
	CC_SYNTHESIZE(bool,m_colliend,Colliend);

private:
	b2Body *m_body;

	b2Vec2 m_startPoint;	// ��ʼλ�ã��ƶ�ʱ����Ч
	b2Vec2 m_desPoint;		// Ŀ��㣬�ƶ�ʱ����Ч
	float m_hSpeedDelta;	// �����ٶ�����
	float m_vSpeedDelta;	// �����ٶ�����
	int m_times;			// ����
};

#endif // __BORDER_H__

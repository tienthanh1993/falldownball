#ifndef __BORDER_POOL_H__
#define __BORDER_POOL_H__

#include "cocos2d.h"
#include "Border.h"
#include "Box2D/Box2D.h"

using namespace std;

class BorderPool 
{
public:
	struct PoolObj
	{
		Border *border;
		bool active;
	};

	static BorderPool *getInstance();

	/* ��ʼ������� */ 
	void initPool(int size,b2World *world);

	/* ȡ��һ������ */
	Border *popBorder();

	/* �Ż�һ������ */
	void pushOneBorder(Border *border);


private:
	BorderPool();
	~BorderPool();

private:
	static BorderPool *m_singleton;

	deque<PoolObj *> m_pool;

	b2World *m_world;
 
};

#define sBorderPool (BorderPool::getInstance())

#endif // __BORDER_POOL_H__

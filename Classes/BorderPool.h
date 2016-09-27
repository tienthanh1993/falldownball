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

	/* 初始化对象池 */ 
	void initPool(int size,b2World *world);

	/* 取出一个对象 */
	Border *popBorder();

	/* 放回一个对象 */
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

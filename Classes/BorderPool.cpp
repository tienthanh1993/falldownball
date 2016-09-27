#include "BorderPool.h"

#define PTM_RATIO 32

BorderPool *BorderPool::m_singleton = nullptr;

BorderPool::BorderPool()
	:m_world(nullptr)
{
	m_pool.clear();
}

BorderPool::~BorderPool()
{
	for (auto &iter : m_pool)
	{
		iter->border->release();
		iter->border = nullptr;

		delete iter;
		iter = nullptr;
	}
}

BorderPool *BorderPool::getInstance()
{
	if (m_singleton == nullptr)
	{
		m_singleton = new BorderPool;
	}

	return m_singleton;
}

void BorderPool::initPool(int size,b2World *world)
{
	if (world == nullptr)
	{
		return;
	}

	m_world = world;

	for (int i=0;i<size;i++)
	{
		PoolObj *pObj = new PoolObj;
		pObj->border = Border::create(world);
		pObj->border->getBody()->SetActive(false);
		pObj->border->retain();
		pObj->active = false;

		m_pool.push_back(pObj);
	}

}

Border *BorderPool::popBorder()
{
	for (auto & iter : m_pool)
	{
		if (!iter->active)
		{
			iter->active = true;
			return iter->border;
		}
	}

	auto pObj = new PoolObj;
	pObj->active = false;
	pObj->border = Border::create(m_world);
	pObj->border->getBody()->SetActive(false);
	pObj->border->retain();

	m_pool.push_back(pObj);
	return pObj->border;

}

void BorderPool::pushOneBorder(Border *border)
{
	b2Body *body = nullptr;
	Size  visibleSize = Director::getInstance()->getVisibleSize();

	for (auto & iter : m_pool)
	{
		if (iter->border == border)
		{
			iter->active = false;
			iter->border->setLayerIndex(0);
			iter->border->setColliend(false);
			body = iter->border->getBody();
			body->SetLinearVelocity(b2Vec2(0,0));
			body->SetTransform(b2Vec2(visibleSize.width/(2 * PTM_RATIO),-iter->border->getContentSize().height/(2 * PTM_RATIO)),0);
			body->SetActive(false);
			return;
		}
	}
}

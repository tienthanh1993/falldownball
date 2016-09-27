#ifndef __GUIDE_LAYER_H__
#define __GUIDE_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class GuideLayer : public Layer
{
public:
	CREATE_FUNC(GuideLayer);
	virtual bool init();

	void onButtonClicked(Ref *pSender,TouchEventType type);

	void layerFlyIn();
	void layerFlyOut();

private:
	Size m_winSize;

};

#endif // __GUIDE_LAYER_H__

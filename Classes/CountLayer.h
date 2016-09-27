#ifndef __COUNT_LAYER_H__
#define __COUNT_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define CMD_SHOW_MAIN_MENU	"show_main_menu"

class CountLayer : public Layer
{
public:
	CountLayer();
	~CountLayer();

	void onEnter();
	void onExit();

    virtual bool init();
    CREATE_FUNC(CountLayer);

	/* get server data 消息处理函数 */
	void getServerDataHandle(Ref *pSender);

private:
	void backBtnCallback(Ref *pSender,TouchEventType type);

	/* 更新排行面板 */
	void updateRankText(int rank);
private:
	Size m_winSize;

	ActionInterval *m_panelAction;
	ActionInterval *m_backBtnAction;
    ActionInterval *m_panelActionReverse;
    ActionInterval *m_backBtnActionReverse;
	TextAtlas *m_scoreText;
	TextAtlas *m_rankText;
    Sprite* spr;
    Node* m_Tab;
	Layout *m_panel;

};

#endif // __COUNT_LAYER_H__

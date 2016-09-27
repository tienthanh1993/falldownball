#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "AdsIntergrate.h"
#define CMD_GAME_START				"game_start"		
#define CMD_GAME_OVER					"game_over"
#define CMD_GAME_LOAD_FINISH		"game_load_finish"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class GameLayer : public Layer
#ifdef SDKBOX_ENABLED
, sdkbox::AdMobListener

#endif
{
public:
	GameLayer();
	~GameLayer();
    
	void onEnter();
	void onExit();

    virtual bool init();
    CREATE_FUNC(GameLayer);

	/* 更新分数面板 */
	void updateScoreText(int score);
    void adViewDidReceiveAd(const std::string &name);
private:
	void onButtonClicked(Ref *pSender,TouchEventType type);

	/* gameover 消息处理函数 */
	void gameOverHandle(Ref *pSender);

	/* logo动画播放完毕回调函数 */
	void playEndCallfnuc(Armature *armature, MovementEventType type, const std::string &str);

	/* show main menu 消息处理函数 */
	void showMainMenuHandle(Ref *pSender);
    
private:
	Size m_winSize;

	TextAtlas *m_score;
	Button *m_startBtn;
	Button *m_rankBtn;
	Button *m_musicBtn;
    Sprite *m_Logo;
	//Armature *m_logo;


	ActionInterval *m_startBtnAnimation;
	ActionInterval *m_rankBtnAnimation;
	ActionInterval *m_musicBtnAnimation;
	ActionInterval *m_logoAnimation;

    ActionInterval *m_startBtnAnimationReverse;
    ActionInterval *m_rankBtnAnimationReverse;
    ActionInterval *m_musicBtnAnimationReverse;
    ActionInterval *m_logoAnimationReverse;
};

#endif // __GAME_LAYER_H__

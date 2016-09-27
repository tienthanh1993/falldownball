#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Border.h"
#include "GameLayer.h"
#include "Cloud.h"
#include "Player.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"


// 是否开启调试绘制 =========================================>>>>
#define DEBUG_DRAW       0


USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

#define PTM_RATIO 32

enum _CategoryMask
{
	WORLD_BORDER = 0x0001,		// 世界边界
	BALL         = 0x0002,		// 小球
	DEAD_FIELD   = 0x0004,		// 死亡区域
	CEILLING     = 0x0008,		// 天花板
	BORDER       = 0x0010,		// 踏板
};

enum _EntityTag
{
	Tag_border      = 10,		      // 踏板
	Tag_player      = 11,			  // 玩家
	Tag_ceiling     = 12,		      // 天花板
	Tag_game_border = 13,		      // 边界
	Tag_deadline    = 14,			  // 死亡区域
};

typedef enum _GameState
{
	GAME_OVER = 1,
	GAME_PREPARED,
	GAME_START,
	GAME_EXIT,
}GameState;


class GameScene : public cocos2d::Scene , b2ContactListener
{
public:

	GameScene();
	~GameScene();

	void onEnter();
	void onExit();

	virtual bool init();

	CREATE_FUNC(GameScene);

	void initPhysics();

	void update(float dt);

	virtual void BeginContact(b2Contact* contact); 
    int _iSourceCount = 0;

#if(DEBUG_DRAW == 1)
	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
private:
	GLESDebugDraw* _debugDraw;
#endif

private:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void onKeyPressed(EventKeyboard::KeyCode code, Event *event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event *event);

	/* 游戏开始消息回调 */
	void gameStartHandle(Ref *pSender);

	/* 界面加载完成消息回调 */
	void gameLoadFinishHandle(Ref *pSender);

	// 创建踏板的定时器
	void createBorderTimer(float dt);

	/* 创建白云定时器 */
	void createCloudTimer(float dt);

	/* 检测白云是否越界 */
	void checkCloudLeaveScreen(float dt);

	/* 等级提升 */
	void levelIncrement();
	void moveToSpecifiedPoint(b2Body *body,b2Vec2 &point);

	/* 第一块踏板移动定时器 */
	void firstBorderMoveTimer(float dt);

	// 显示提示
	void showTips();

	// 排名定时器
	void updateRankTimer(float dt);
    
    std::vector<std::string> *_pSourceVec = nullptr;
    
    void preloadSource();
    
    void addLayer();
private:
	Size m_winSize;
	Player *m_player;

	int m_curScore;	    // 当前得分

	GameLayer *m_gameLayer;

	vector<Border *> m_borders;

	EventListenerKeyboard *m_keyBoardListener;
	EventListenerTouchOneByOne *m_touchEventListener;

	int m_curLayerIndex;	// 当前踩的踏板索引
	int m_prevLayerIndex;	// 上一个踏板索引

	Vector<Cloud *> m_clouds;

	float m_vSpeed;	// 踏板当前的移动速度

	int m_curLevel;	// 当前的速度等级

	b2World *m_world;

	vector<Node *> m_destroySet;

	GameState m_gameState;

	bool m_exitGame;

};

#endif // __GAME_SCENE_H__

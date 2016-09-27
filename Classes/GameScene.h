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


// �Ƿ������Ի��� =========================================>>>>
#define DEBUG_DRAW       0


USING_NS_CC;
using namespace std;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

#define PTM_RATIO 32

enum _CategoryMask
{
	WORLD_BORDER = 0x0001,		// ����߽�
	BALL         = 0x0002,		// С��
	DEAD_FIELD   = 0x0004,		// ��������
	CEILLING     = 0x0008,		// �컨��
	BORDER       = 0x0010,		// ̤��
};

enum _EntityTag
{
	Tag_border      = 10,		      // ̤��
	Tag_player      = 11,			  // ���
	Tag_ceiling     = 12,		      // �컨��
	Tag_game_border = 13,		      // �߽�
	Tag_deadline    = 14,			  // ��������
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

	/* ��Ϸ��ʼ��Ϣ�ص� */
	void gameStartHandle(Ref *pSender);

	/* ������������Ϣ�ص� */
	void gameLoadFinishHandle(Ref *pSender);

	// ����̤��Ķ�ʱ��
	void createBorderTimer(float dt);

	/* �������ƶ�ʱ�� */
	void createCloudTimer(float dt);

	/* �������Ƿ�Խ�� */
	void checkCloudLeaveScreen(float dt);

	/* �ȼ����� */
	void levelIncrement();
	void moveToSpecifiedPoint(b2Body *body,b2Vec2 &point);

	/* ��һ��̤���ƶ���ʱ�� */
	void firstBorderMoveTimer(float dt);

	// ��ʾ��ʾ
	void showTips();

	// ������ʱ��
	void updateRankTimer(float dt);
    
    std::vector<std::string> *_pSourceVec = nullptr;
    
    void preloadSource();
    
    void addLayer();
private:
	Size m_winSize;
	Player *m_player;

	int m_curScore;	    // ��ǰ�÷�

	GameLayer *m_gameLayer;

	vector<Border *> m_borders;

	EventListenerKeyboard *m_keyBoardListener;
	EventListenerTouchOneByOne *m_touchEventListener;

	int m_curLayerIndex;	// ��ǰ�ȵ�̤������
	int m_prevLayerIndex;	// ��һ��̤������

	Vector<Cloud *> m_clouds;

	float m_vSpeed;	// ̤�嵱ǰ���ƶ��ٶ�

	int m_curLevel;	// ��ǰ���ٶȵȼ�

	b2World *m_world;

	vector<Node *> m_destroySet;

	GameState m_gameState;

	bool m_exitGame;

};

#endif // __GAME_SCENE_H__

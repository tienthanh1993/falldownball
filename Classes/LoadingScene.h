//
//  LoadingScene.h
//  DownWardBall
//
//  Created by Pham Thanh on 7/6/16.
//
//

#ifndef LoadingScene_hpp
#define LoadingScene_hpp

#include <cocos2d.h>
USING_NS_CC;
class GameScene;

class LoadingScene : public Scene {
public:
    CREATE_FUNC(LoadingScene);
    virtual ~LoadingScene();
    Layer* _pLoadingSceneLayer;
protected:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void sourceLoadCallBack(Ref *pData);
    GameScene *_pGameScene = nullptr;
};

#endif /* LoadingScene_hpp */

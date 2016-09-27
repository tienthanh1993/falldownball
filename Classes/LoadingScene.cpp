//
//  LoadingScene.cpp
//  DownWardBall
//
//  Created by Pham Thanh on 7/6/16.
//
//
#include "Define.h"

#include "LoadingScene.h"
#include "GameScene.h"

LoadingScene::~LoadingScene() {
    CC_SAFE_RELEASE(_pLoadingSceneLayer);
}


bool LoadingScene::init() {
    bool bRet = false;
    
    do
    {
        CC_BREAK_IF(!Scene::init());
        auto pBg = Sprite::create("bg.png");
        pBg->setAnchorPoint(Vec2::ZERO);
        addChild(pBg);
        bRet = true;
    }
    while (0);
    
    return bRet;
}

void LoadingScene::onEnter() {
    Scene::onEnter();
    NOTIFY->addObserver(this, callfuncO_selector(LoadingScene::sourceLoadCallBack), "loadFinish", nullptr);
    _pGameScene = GameScene::create();
    _pGameScene->retain();

}
void LoadingScene::sourceLoadCallBack(Ref *pData) {
    auto pTransScene = TransitionFade::create(0.2f, dynamic_cast<Scene*>(pData));
    Director::getInstance()->replaceScene(pTransScene);
}
void LoadingScene::onExit() {
    NOTIFY->removeAllObservers(this);
    CC_SAFE_RELEASE_NULL(_pGameScene);
    Scene::onExit();
    
}
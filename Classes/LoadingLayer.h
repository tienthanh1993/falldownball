//
//  LoadingLayer.h
//  DownWardBall
//
//  Created by Pham Thanh on 7/6/16.
//
//

#ifndef LoadingLayer_h
#define LoadingLayer_h

#include <cocos2d.h>
USING_NS_CC;

class LoadingLayer : public Layer {
public:
    CREATE_FUNC(LoadingLayer);
    virtual bool init();
    virtual ~LoadingLayer();
private:
    void LoadingCallback();
};

#endif /* LoadingLayer_h */

//
//  AdsIntergrate.cpp
//  Skeleton
//
//  Created by Pham Thanh on 4/13/16.
//
//

#include "AdsIntergrate.h"
#include <cocos2d.h>
USING_NS_CC;
int Ads::FULLSCREEN_COUNT =0;

void Ads::init() {
 //   CCLOG("Ads::init");
#ifdef SDKBOX_ENABLED
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        sdkbox::init("899f405747744a4cb5ca7df97ea1da5c","814a84ddf9925106");
         sdkbox::PluginAdMob::init();

    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
       sdkbox::init("aa0b7f1c867342d6e77c09e3174f39aa","af48b6719b41ff82");
        sdkbox::PluginAdMob::init();

    #endif
   
#endif
}

void Ads::showAdmob(std::string unit) {
#ifdef SDKBOX_ENABLED
    if (sdkbox::PluginAdMob::isAvailable(unit)) {
          sdkbox::PluginAdMob::show(unit);
    }
#endif
}

bool Ads::isAdmobAvailable(std::string unit) {
#ifdef SDKBOX_ENABLED
    return sdkbox::PluginAdMob::isAvailable(unit);
#endif
    return false;
}
void Ads::hideAdmob(std::string unit) {
#ifdef SDKBOX_ENABLED
    sdkbox::PluginAdMob::hide(unit);
#endif
}

void Ads::preload() {
    preloadAdmob();
}
void Ads::preloadAdmob() {
#ifdef SDKBOX_ENABLED
//    sdkbox::PluginAdMob::cache("home");
    sdkbox::PluginAdMob::cache("gameover");
#endif
}


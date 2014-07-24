#ifndef __PROFILE_TEST_H__
#define __PROFILE_TEST_H__
#include <random>
#include "cocos2d.h"
#include "../testBasic.h"
#include "../BaseTest.h"

template<typename T>
T randomBetween(T min, T max)
{
    std::random_device gen;
    std::uniform_int_distribution<T> dist(min, max);
    return dist(gen);
}

class ProfileMainLayer : public Layer
{
public:
    virtual void onEnter() override;
    
    bool onTouchBegan(Touch* touches, Event  *event);
    void onTouchMoved(Touch* touches, Event  *event);
    
    void onMouseScroll(Event *event);
protected:
    Vec2 _beginPos;
    Menu* _itemMenu;
    
    static Vec2 _CurrentPos;
};

class ProfileLayer : public BaseTest
{
public:
    CREATE_FUNC(ProfileLayer);
    void onEnter() override;
    
protected:
    void onQuantityPrev(Ref* sender) { if (_cbQuantityPrev) { _cbQuantityPrev(sender); recreate(); } }
    void onQuantityNext(Ref* sender) { if (_cbQuantityPrev) { _cbQuantityNext(sender); recreate(); } }
    void onPositionPrev(Ref* sender) { if (_cbPositionPrev) { _cbPositionPrev(sender); recreate(); } }
    void onPositionNext(Ref* sender) { if (_cbPositionPrev) { _cbPositionNext(sender); recreate(); } }
    void onVisibilityPrev(Ref* sender) { if (_cbVisibilityPrev) { _cbVisibilityPrev(sender); recreate(); } }
    void onVisibilityNext(Ref* sender) { if (_cbVisibilityPrev) { _cbVisibilityNext(sender); recreate(); } }
    
    void setHint() { if (_hintLabel) _hintLabel->setString(hint()); }
    
    void createTweaker(const std::string& labelText, const Vec2& position, Label*& display, ccMenuCallback cbPrev, ccMenuCallback cbNext);
    virtual void recreate() {};
    virtual std::string hint() const { return ""; }
    
protected:
    ccMenuCallback _cbQuantityPrev;
    ccMenuCallback _cbQuantityNext;
    ccMenuCallback _cbPositionPrev;
    ccMenuCallback _cbPositionNext;
    ccMenuCallback _cbVisibilityPrev;
    ccMenuCallback _cbVisibilityNext;
    
    Label* _quantityLabel;
    Label* _positionLabel;
    Label* _visibilityLabel;
    
    Label* _hintLabel;
};

class ProfileTestScene : public TestScene
{
public:
    CREATE_FUNC(ProfileTestScene);
    virtual void runThisTest();
};

#endif

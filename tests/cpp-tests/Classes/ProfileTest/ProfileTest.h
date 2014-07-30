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

struct AutoTweaker
{
    AutoTweaker(const std::function<void (int)>& f, int mi, int ma):func(f), min(mi), max(ma) {}
    std::function<void (int)> func;
    int min;
    int max;
};

class FrameProfiler
{
    long _totalFrames;
    long _totalTime;
    long _maxDuration;
    long _minDuration;
    long _averageDuration;
    
    std::string _desc;
    
public:
    FrameProfiler():_totalFrames(0), _totalTime(0), _maxDuration(0), _minDuration(std::numeric_limits<long>::max()), _averageDuration(0)
    {
    }
    void reset();
    void sample(long dt);
    std::string getResult() const;
    void setDesc(const std::string& desc) { _desc = desc; }
    std::string getDesc() const { return _desc; }
};

class ProfileLayer : public BaseTest
{
public:
    CREATE_FUNC(ProfileLayer);
    void onEnter() override;
    void onExit() override;
    
    void doAutoTest();
    void updateProfiler(float dt);
    virtual std::string getDescription() const { return "Please describe the specification in the derived class."; };
    void dumpAutoTest();
    
protected:
    void onQuantityPrev(Ref* sender) { if (_cbQuantityPrev) { _cbQuantityPrev(sender); recreate(); } }
    void onQuantityNext(Ref* sender) { if (_cbQuantityPrev) { _cbQuantityNext(sender); recreate(); } }
    void onPositionPrev(Ref* sender) { if (_cbPositionPrev) { _cbPositionPrev(sender); recreate(); } }
    void onPositionNext(Ref* sender) { if (_cbPositionPrev) { _cbPositionNext(sender); recreate(); } }
    void onVisibilityPrev(Ref* sender) { if (_cbVisibilityPrev) { _cbVisibilityPrev(sender); recreate(); } }
    void onVisibilityNext(Ref* sender) { if (_cbVisibilityPrev) { _cbVisibilityNext(sender); recreate(); } }
    
    void onAutoTest(Ref*);
    
    void setHint() { if (_hintLabel) _hintLabel->setString(hint()); }
    
    void createTrigger(const std::string& hint, const Vec2& position, Label*& display, ccMenuCallback callback);
    void createTweaker(const std::string& labelText, const Vec2& position, Label*& display, ccMenuCallback cbPrev, ccMenuCallback cbNext);
    virtual void setupTweakers();
    virtual void recreate() {};
    virtual std::string hint() const { return ""; }
    
    virtual void setupAutoTweakers() {};
    void createAutoTweaker(const std::function<void (int)>& func, int min, int max);
    
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
    
    std::stack<AutoTweaker> _autoTweakers;
    std::thread* _autoTestThread;
    std::atomic<bool> _needRecreate;
    std::atomic<bool> _autoTestRunning;
    std::chrono::high_resolution_clock::time_point _lastTime;
    
    std::vector<FrameProfiler> _frameProfilers;
};

class ProfileTestScene : public TestScene
{
public:
    CREATE_FUNC(ProfileTestScene);
    virtual void runThisTest();
};

#endif

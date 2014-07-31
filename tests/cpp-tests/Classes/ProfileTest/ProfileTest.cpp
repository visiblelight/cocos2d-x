#include "ProfileTest.h"
#include "../testResource.h"
#include "ProfileNodeTest.h"
#include "ProfileSpriteTest.h"
#include "ProfileParticleTest.h"

enum
{
    LINE_SPACE = 40,
    kItemTagBasic = 1000,
};

struct {
	const char *name;
	std::function<void(Ref*)> callback;
} g_testsName[] =
{
    { "Profile Node", [](Ref*) { runProfileNodeTest(); } },
    { "Profile Sprite", [](Ref*) { runProfileSpriteTest(); } },
    { "Profile Particle", [](Ref*) { runProfileParticleTest(); }},
};

static const int g_testMax = sizeof(g_testsName)/sizeof(g_testsName[0]);

Vec2 ProfileMainLayer::_CurrentPos = Vec2::ZERO;

void ProfileMainLayer::onEnter()
{
    Layer::onEnter();
    
    auto s = Director::getInstance()->getWinSize();
    
    _itemMenu = Menu::create();
    _itemMenu->setPosition(_CurrentPos);
    MenuItemFont::setFontName("fonts/arial.ttf");
    MenuItemFont::setFontSize(24);
    for (int i = 0; i < g_testMax; ++i)
    {
        auto pItem = MenuItemFont::create(g_testsName[i].name, g_testsName[i].callback);
        pItem->setPosition(Vec2(s.width / 2, s.height - (i + 1) * LINE_SPACE));
        _itemMenu->addChild(pItem, kItemTagBasic + i);
    }
    
    addChild(_itemMenu);
    
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(ProfileMainLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ProfileMainLayer::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = CC_CALLBACK_1(ProfileMainLayer::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

bool ProfileMainLayer::onTouchBegan(Touch* touches, Event  *event)
{
    _beginPos = touches->getLocation();
    return true;
}
void ProfileMainLayer::onTouchMoved(Touch* touches, Event  *event)
{
//    auto touchLocation = touches->getLocation();
//    float nMoveY = touchLocation.y - _beginPos.y;
//    
//    auto curPos  = _itemMenu->getPosition();
//    auto nextPos = Vec2(curPos.x, curPos.y + nMoveY);
//    
//    if (nextPos.y < 0.0f)
//    {
//        _itemMenu->setPosition(Vec2::ZERO);
//        return;
//    }
//    
//    if (nextPos.y > ((g_testMax + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
//    {
//        _itemMenu->setPosition(Vec2(0, ((g_testMax + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
//        return;
//    }
//    
//    _itemMenu->setPosition(nextPos);
//    _beginPos = touchLocation;
//    _CurrentPos = nextPos;
}

void ProfileMainLayer::onMouseScroll(Event *event)
{
    auto mouseEvent = static_cast<EventMouse*>(event);
    float nMoveY = mouseEvent->getScrollY() * 6;
    
    auto curPos  = _itemMenu->getPosition();
    auto nextPos = Vec2(curPos.x, curPos.y + nMoveY);
    
    if (nextPos.y < 0.0f)
    {
        _itemMenu->setPosition(Vec2::ZERO);
        return;
    }
    
    if (nextPos.y > ((g_testMax + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height))
    {
        _itemMenu->setPosition(Vec2(0, ((g_testMax + 1)* LINE_SPACE - VisibleRect::getVisibleRect().size.height)));
        return;
    }
    
    _itemMenu->setPosition(nextPos);
    _CurrentPos = nextPos;
}

void FrameProfiler::reset()
{
    _totalFrames = 0;
    _totalTime = 0;
    _maxDuration = 0;
    _minDuration = std::numeric_limits<long>::max();
    _averageDuration = 0;
    _totalMemory = 0;
    _maxMemory = 0;
    _minMemory = std::numeric_limits<long>::max();
    _averageMemory = 0;
}

void FrameProfiler::sample(long dt, long memory)
{
    ++_totalFrames;
    _totalTime += dt;
    _totalMemory += memory;
    
    if (dt > _maxDuration)
    {
        _maxDuration = dt;
    }
    
    if (dt < _minDuration);
    {
        _minDuration = dt;
    }
    
    _averageDuration = _totalTime / _totalFrames;
    
    if (memory > _maxMemory)
    {
        _maxMemory = memory;
    }
    
    if (memory < _minMemory)
    {
        _minMemory = memory;
    }
    
    _averageMemory = _totalMemory / _totalFrames;
}

std::string FrameProfiler::getResult() const
{
    return StringUtils::format("Time{Max: %ld, Min: %ld, Average: %ld}|Memory{Max: %ld, Min: %ld, Average: %ld}",
                               _maxDuration, _minDuration, _averageDuration, _maxMemory, _minMemory, _averageMemory);
}

void ProfileLayer::createTrigger(const std::string& hint, const cocos2d::Vec2& position, cocos2d::Label*& display, ccMenuCallback callback)
{
    auto label = Label::createWithTTF(hint, "fonts/arial.ttf", 12);
    label->setColor(Color3B::YELLOW);
    auto item = MenuItemLabel::create(label, callback);
    auto menu = Menu::create(item, nullptr);
    menu->setPosition(position);
    addChild(menu);
    display = Label::createWithTTF("", "fonts/arial.ttf", 12);
    display->setPosition(position - Vec2(80, 0));
    display->setColor(Color3B::YELLOW);
    addChild(display);
}

void ProfileLayer::createTweaker(const std::string& labelText, const Vec2& position, Label*& display, ccMenuCallback cbPrev, ccMenuCallback cbNext)
{
    auto label = Label::createWithTTF(labelText, "fonts/arial.ttf", 12);
    label->setColor(Color3B::RED);
    addChild(label);
    label->setPosition(position);
    auto prevLabel = Label::createWithTTF("<=", "fonts/arial.ttf", 16);
    prevLabel->setColor(Color3B::RED);
    auto nextLabel = Label::createWithTTF("=>", "fonts/arial.ttf", 16);
    nextLabel->setColor(Color3B::RED);
    auto prev = MenuItemLabel::create(prevLabel, cbPrev);
    prev->setPosition(Vec2(14, 0));
    auto next = MenuItemLabel::create(nextLabel, cbNext);
    next->setPosition(Vec2(40, 0));
    auto menu = Menu::create(prev, next, nullptr);
    addChild(menu);
    menu->setPosition(position + Vec2(32, 0));
    auto displayText = Label::createWithTTF("", "fonts/arial.ttf", 12);
    displayText->setColor(Color3B::RED);
    addChild(displayText);
    displayText->setPosition(position + Vec2(128, 0));
    display = displayText;
}

void ProfileLayer::createAutoTweaker(const std::function<void (int)>& func, int min, int max)
{
    _autoTweakers.push(AutoTweaker(func, min, max));
}

void ProfileLayer::doAutoTest()
{
    if (!_autoTweakers.empty())
    {
        auto tweaker = _autoTweakers.top();
        _autoTweakers.pop();
        for (int i = tweaker.min; i < tweaker.max; ++i)
        {
            tweaker.func(i);
            doAutoTest();
        }
        _autoTweakers.push(tweaker);
    }
    else
    {
        _needRecreate = true;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

void ProfileLayer::onAutoTest(Ref* sender)
{
    if (_autoTestRunning) return;
    if (_autoTestThread)
    {
        _autoTestThread->join();
        CC_SAFE_DELETE(_autoTestThread);
    }
    
    _autoTestThread = new std::thread([&]()
    {
        _autoTestRunning = true;
        doAutoTest();
        _autoTestRunning = false;
        dumpAutoTest();
        _autoItem->setString("AutoTest");
    });
    // delete later
}

void ProfileLayer::dumpAutoTest()
{
    for(const auto& profiler : _frameProfilers)
    {
        CCLOG("==========================================================================");
        CCLOG("%s", profiler.getDesc().c_str());
        CCLOG("%s", profiler.getResult().c_str());
        CCLOG("==========================================================================");
    }
}

void ProfileLayer::updateProfiler(float dt)
{
    if (_needRecreate)
    {
        recreate();
        _needRecreate = false;
        _frameProfilers.push_back(FrameProfiler());
        _frameProfilers.back().setDesc(getDescription());
        if (_autoTestRunning) _autoItem->setString("AutoTesting...");
    }
    
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    if (_autoTestRunning)
    {
        long duration = static_cast<long>(std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastTime).count());
        _frameProfilers.back().sample(duration, Device::getUsedMemory() / 1024);
    }
    _lastTime = now;
}

void ProfileLayer::onEnter()
{
    BaseTest::onEnter();
    
    setupTweakers();
    
    _hintLabel = Label::createWithTTF(hint(), "fonts/arial.ttf", 12);
    _hintLabel->setPosition(Director::getInstance()->getWinSize().width / 2, 100);
    _hintLabel->setColor(Color3B::GREEN);
    addChild(_hintLabel);
    
    // Auto test button
    auto autoLabel = Label::createWithTTF("AutoTest", "fonts/arial.ttf", 16);
    auto autoItem = MenuItemLabel::create(autoLabel, CC_CALLBACK_1(ProfileLayer::onAutoTest, this));
    auto autoMenu = Menu::create(autoItem, nullptr);
    addChild(autoMenu);
    autoMenu->setPosition(Director::getInstance()->getWinSize().width / 2, 120);
    _autoItem = autoItem;
    
    schedule(schedule_selector(ProfileLayer::updateProfiler));
    _needRecreate = false;
    _lastTime = std::chrono::high_resolution_clock::now();
    _autoTestThread = nullptr;
}

void ProfileLayer::onExit()
{
    if (_autoTestThread)
    {
        _autoTestThread->join();
        CC_SAFE_DELETE(_autoTestThread);
    }
    BaseTest::onExit();
}

void ProfileLayer::setupTweakers()
{
    createTweaker("Quantity", Vec2(40, 260), _quantityLabel,
                  CC_CALLBACK_1(ProfileLayer::onQuantityPrev, this), CC_CALLBACK_1(ProfileLayer::onQuantityNext, this));
    createTweaker("Positions", Vec2(40, 240), _positionLabel,
                  CC_CALLBACK_1(ProfileLayer::onPositionPrev, this), CC_CALLBACK_1(ProfileLayer::onPositionNext, this));
    createTweaker("Visibility", Vec2(40, 220), _visibilityLabel,
                  CC_CALLBACK_1(ProfileLayer::onVisibilityPrev, this), CC_CALLBACK_1(ProfileLayer::onVisibilityNext, this));
}

void ProfileTestScene::runThisTest()
{
    auto layer = new ProfileMainLayer();
    addChild(layer);
    layer->release();
    
    Director::getInstance()->replaceScene(this);
}
#include "ProfileTest.h"
#include "../testResource.h"
#include "ProfileNodeTest.h"
#include "ProfileSpriteTest.h"

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
    { "Profile Sprite", [](Ref*sender) { runProfileSpriteTest(); } },
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
    auto touchLocation = touches->getLocation();
    float nMoveY = touchLocation.y - _beginPos.y;
    
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
    _beginPos = touchLocation;
    _CurrentPos = nextPos;
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

void ProfileLayer::onEnter()
{
    BaseTest::onEnter();
    
    createTweaker("Quantity", Vec2(40, 260), _quantityLabel,
               CC_CALLBACK_1(ProfileLayer::onQuantityPrev, this), CC_CALLBACK_1(ProfileLayer::onQuantityNext, this));
    createTweaker("Positions", Vec2(40, 240), _positionLabel,
               CC_CALLBACK_1(ProfileLayer::onPositionPrev, this), CC_CALLBACK_1(ProfileLayer::onPositionNext, this));
    createTweaker("Visibility", Vec2(40, 220), _visibilityLabel,
               CC_CALLBACK_1(ProfileLayer::onVisibilityPrev, this), CC_CALLBACK_1(ProfileLayer::onVisibilityNext, this));
    
    _hintLabel = Label::createWithTTF(hint(), "fonts/arial.ttf", 12);
    _hintLabel->setPosition(Director::getInstance()->getWinSize().width / 2, 100);
    _hintLabel->setColor(Color3B::GREEN);
    addChild(_hintLabel);
}

void ProfileTestScene::runThisTest()
{
    auto layer = new ProfileMainLayer();
    addChild(layer);
    layer->release();
    
    Director::getInstance()->replaceScene(this);
}
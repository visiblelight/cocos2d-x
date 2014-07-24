#include "ProfileNodeTest.h"

void runProfileNodeTest(int testIndex)
{
    auto scene = new ProfileNodeScene();
    if (testIndex == 0)
    {
        auto layer = ProfileNodeBasicTest::create();
        scene->addChild(layer);
    }
    else if (testIndex == 1)
    {
        auto layer = ProfileNodeOperationTest::create();
        scene->addChild(layer);
    }
    else
    {
        CCASSERT(false, "Invalid test index.");
    }
    Director::getInstance()->replaceScene(scene);
    scene->release();

}

void ProfileNodeScene::runThisTest()
{
}

void ProfileNodeScene::onEnter()
{
    Scene::onEnter();
    
    TTFConfig ttfConfig("fonts/arial.ttf", 20);
    auto label = Label::createWithTTF(ttfConfig,"Back");
    auto menuItem = MenuItemLabel::create(label, [](Ref*)
    {
        auto scene = ProfileTestScene::create();
        scene->runThisTest();
    });
    auto menu = Menu::create(menuItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    menuItem->setPosition(Vec2(VisibleRect::right().x - 50, VisibleRect::bottom().y + 25));
    addChild(menu, 1);
}

int ProfileNodeTest::s_testIndex = 0;

void ProfileNodeTest::restartCallback(Ref*)
{
    runProfileNodeTest(s_testIndex);
}

void ProfileNodeTest::nextCallback(Ref*)
{
    s_testIndex = ++s_testIndex % TestIndexCount;
    runProfileNodeTest(s_testIndex);
}

void ProfileNodeTest::backCallback(Ref*)
{
    s_testIndex = --s_testIndex < 0 ? TestIndexCount - 1 : s_testIndex;
    runProfileNodeTest(s_testIndex);
}

void ProfileNodeBasicTest::onEnter()
{
    _cbQuantityPrev = CC_CALLBACK_1(ProfileNodeBasicTest::onQuantityPrev, this);
    _cbQuantityNext = CC_CALLBACK_1(ProfileNodeBasicTest::onQuantityNext, this);
    _cbPositionPrev = CC_CALLBACK_1(ProfileNodeBasicTest::onPositionPrev, this);
    _cbPositionNext = CC_CALLBACK_1(ProfileNodeBasicTest::onPositionNext, this);
    _cbVisibilityPrev = CC_CALLBACK_1(ProfileNodeBasicTest::onVisibilityPrev, this);
    _cbVisibilityNext = CC_CALLBACK_1(ProfileNodeBasicTest::onVisibilityNext, this);
    
    _quantityIndex = 0;
    _positionIndex = 0;
    _visibilityIndex = 0;
    
    recreate();
}

void ProfileNodeBasicTest::recreate()
{
    removeAllChildren();
    
    int quantity = ProfileNodeTestPolicy::Quantities[_quantityIndex];
    Size winSize = Director::getInstance()->getWinSize();
    
    for (int i = 0; i < quantity; ++i)
    {
        auto node = Node::create();
        addChild(node);
        
        // position
        if (_positionIndex == 0)
        {
            // Inside CCV
            node->setPosition(winSize.width * CCRANDOM_0_1(), winSize.height * CCRANDOM_0_1());
        }
        else if (_positionIndex == 1)
        {
            // 50% inside CCV
            if (i < quantity/2)
            {
                node->setPosition(winSize.width * CCRANDOM_0_1(), winSize.height * CCRANDOM_0_1());
            }
            else
            {
                node->setPosition(100000, 100000);
            }
        }
        else if (_positionIndex == 2)
        {
            // Outside CCV
            node->setPosition(100000, 100000);
        }
        else
        {
            CCASSERT(false, "Invalid position policy.");
        }
        
        // visibility
        if (_visibilityIndex == 0)
        {
            // visible
            node->setVisible(true);
        }
        else if (_visibilityIndex == 1)
        {
            // 50% visible
            if (i < quantity/2)
            {
                node->setVisible(true);
            }
            else
            {
                node->setVisible(false);
            }
        }
        else if (_visibilityIndex == 2)
        {
            // invisible
            node->setVisible(false);
        }
        else
        {
            CCASSERT(false, "Invalid visibility policy.");
        }
    }
    
    ProfileLayer::onEnter();
    
    // update the displays
    _quantityLabel->setString(StringUtils::format("%d", ProfileNodeTestPolicy::Quantities[_quantityIndex]));
    _positionLabel->setString(ProfileNodeTestPolicy::PositionPolicy[_positionIndex]);
    _visibilityLabel->setString(ProfileNodeTestPolicy::VisibilityPolicy[_visibilityIndex]);
    setHint();
}

std::string ProfileNodeBasicTest::hint() const
{
    return "Profile basic scene graph efficiency.";
}

void ProfileNodeBasicTest::onQuantityPrev(Ref*)
{
    _quantityIndex = --_quantityIndex < 0 ? ProfileNodeTestPolicy::QuantityPolicyCount - 1 : _quantityIndex;
}

void ProfileNodeBasicTest::onQuantityNext(Ref*)
{
    _quantityIndex = ++_quantityIndex % ProfileNodeTestPolicy::QuantityPolicyCount;
}

void ProfileNodeBasicTest::onPositionPrev(Ref*)
{
    _positionIndex = --_positionIndex < 0 ? ProfileNodeTestPolicy::PositionPolicyCount - 1 : _positionIndex;
}

void ProfileNodeBasicTest::onPositionNext(Ref*)
{
    _positionIndex = ++_positionIndex % ProfileNodeTestPolicy::PositionPolicyCount;
}

void ProfileNodeBasicTest::onVisibilityPrev(Ref*)
{
    _visibilityIndex = --_visibilityIndex < 0 ? ProfileNodeTestPolicy::VisibilityPolicyCount - 1 : _visibilityIndex;
}

void ProfileNodeBasicTest::onVisibilityNext(Ref*)
{
    _visibilityIndex = ++_visibilityIndex % ProfileNodeTestPolicy::VisibilityPolicyCount;
}

void ProfileNodeOperationTest::update(float dt)
{
    // remove children
    for (int i = 0; i < ProfileNodeTestPolicy::Quantities[_quantityIndex]; ++i)
    {
        removeChildByTag(i);
    }
    
    // add children
    for (int i = 0; i < ProfileNodeTestPolicy::Quantities[_quantityIndex]; ++i)
    {
        auto node = Node::create();
        addChild(node, 0, i);
    }
}

void ProfileNodeOperationTest::recreate()
{
    removeAllChildren();
    
    int quantity = ProfileNodeTestPolicy::Quantities[_quantityIndex];
    Size winSize = Director::getInstance()->getWinSize();
    
    for (int i = 0; i < quantity; ++i)
    {
        auto node = Node::create();
        addChild(node, 0, i);
        
        // position
        if (_positionIndex == 0)
        {
            // Inside CCV
            node->setPosition(winSize.width * CCRANDOM_0_1(), winSize.height * CCRANDOM_0_1());
        }
        else if (_positionIndex == 1)
        {
            // 50% inside CCV
            if (i < quantity/2)
            {
                node->setPosition(winSize.width * CCRANDOM_0_1(), winSize.height * CCRANDOM_0_1());
            }
            else
            {
                node->setPosition(100000, 100000);
            }
        }
        else if (_positionIndex == 2)
        {
            // Outside CCV
            node->setPosition(100000, 100000);
        }
        else
        {
            CCASSERT(false, "Invalid position policy.");
        }
        
        // visibility
        if (_visibilityIndex == 0)
        {
            // visible
            node->setVisible(true);
        }
        else if (_visibilityIndex == 1)
        {
            // 50% visible
            if (i < quantity/2)
            {
                node->setVisible(true);
            }
            else
            {
                node->setVisible(false);
            }
        }
        else if (_visibilityIndex == 2)
        {
            // invisible
            node->setVisible(false);
        }
        else
        {
            CCASSERT(false, "Invalid visibility policy.");
        }
    }
    
    ProfileLayer::onEnter();
    
    // update the displays
    _quantityLabel->setString(StringUtils::format("%d", ProfileNodeTestPolicy::Quantities[_quantityIndex]));
    _positionLabel->setString(ProfileNodeTestPolicy::PositionPolicy[_positionIndex]);
    _visibilityLabel->setString(ProfileNodeTestPolicy::VisibilityPolicy[_visibilityIndex]);
    setHint();
    
    scheduleUpdate();
}

std::string ProfileNodeOperationTest::hint() const
{
    return "Profile scene graph add/remove children.";
}

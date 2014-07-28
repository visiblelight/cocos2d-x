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
    
    // trigger
    createTrigger("Find", Vec2(420, 260), _operationTimeLabel, [&](Ref*){ this->scheduleOnce(schedule_selector(ProfileNodeOperationTest::doOperations), 0.05f); });
    
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

void ProfileNodeOperationTest::doOperations(float)
{
    // Stark Family
    auto p0 = Node::create();
    p0->setName("eddard-stark");
    addChild(p0);
    
    auto p01 = Node::create();
    p01->setName("robb-stark");
    p0->addChild(p01);
    
    auto p02 = Node::create();
    p02->setName("sansa-stark");
    p0->addChild(p02);
    
    auto p03 = Node::create();
    p03->setName("arya-stark");
    p0->addChild(p03);
    
    auto p04 = Node::create();
    p04->setName("bran-stark");
    p0->addChild(p04);
    
    auto p05 = Node::create();
    p05->setName("rickon-stark");
    p0->addChild(p05);
    
    auto p06 = Node::create();
    p06->setName("jon-snow");
    p0->addChild(p06);
    auto p061 = Node::create();
    p061->setName("ghost");
    p06->addChild(p061);
    
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    
    // enumerate
    enumerateChildren("//.*-stark$", [](Node* node)->bool { /*CCLOG("%s: Winter is coming.", node->getName().c_str());*/ return false; });
    
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    _operationTimeLabel->setString(StringUtils::format("%ld us", static_cast<long>(duration.count())));
}

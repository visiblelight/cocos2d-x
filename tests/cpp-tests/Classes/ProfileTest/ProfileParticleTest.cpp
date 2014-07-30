#include "ProfileParticleTest.h"

void runProfileParticleTest(int testIndex)
{
    auto scene = new ProfileParticleScene();
    if (testIndex == 0)
    {
        auto layer = ProfileParticleBasicTest::create();
        scene->addChild(layer);
    }
    else
    {
        CCASSERT(false, "Invalid test index.");
    }
    Director::getInstance()->replaceScene(scene);
    scene->release();
    
}

void ProfileParticleScene::runThisTest()
{
}

void ProfileParticleScene::onEnter()
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

int ProfileParticleTest::s_testIndex = 0;

void ProfileParticleTest::restartCallback(Ref*)
{
    runProfileParticleTest(s_testIndex);
}

void ProfileParticleTest::nextCallback(Ref*)
{
    s_testIndex = ++s_testIndex % TestIndexCount;
    runProfileParticleTest(s_testIndex);
}

void ProfileParticleTest::backCallback(Ref*)
{
    s_testIndex = --s_testIndex < 0 ? TestIndexCount - 1 : s_testIndex;
    runProfileParticleTest(s_testIndex);
}

void ProfileParticleBasicTest::onEnter()
{
    _cbQuantityPrev = CC_CALLBACK_1(ProfileParticleBasicTest::onQuantityPrev, this);
    _cbQuantityNext = CC_CALLBACK_1(ProfileParticleBasicTest::onQuantityNext, this);
    _cbPositionPrev = CC_CALLBACK_1(ProfileParticleBasicTest::onPositionPrev, this);
    _cbPositionNext = CC_CALLBACK_1(ProfileParticleBasicTest::onPositionNext, this);
    _cbVisibilityPrev = CC_CALLBACK_1(ProfileParticleBasicTest::onVisibilityPrev, this);
    _cbVisibilityNext = CC_CALLBACK_1(ProfileParticleBasicTest::onVisibilityNext, this);
    
    _quantityIndex = 0;
    _positionIndex = 0;
    _visibilityIndex = 0;
    _nParticleIndex = 0;//ProfileParticleTestPolicy::NumberOfParticlesPolicyCount - 1;
    
    recreate();
}

void ProfileParticleBasicTest::recreate()
{
    removeAllChildren();
    
    int quantity = ProfileParticleTestPolicy::Quantities[_quantityIndex];
    Size winSize = Director::getInstance()->getWinSize();
    
    for (int i = 0; i < quantity; ++i)
    {
        //auto node = Node::create();
        auto node = ParticleSystemQuad::createWithTotalParticles(ProfileParticleTestPolicy::NumberOfParticlesPolicy[_nParticleIndex]);
        node->setTexture(Director::getInstance()->getTextureCache()->addImage("Images/fire.png"));
        //auto node = ParticleSystemQuad::create("Particles/Spiral.plist");;
        addChild(node, 0, i);
        
        // duration
        node->setDuration(-1);
        
        // gravity
        node->setGravity(Vec2(0,-90));
        
        // angle
        node->setAngle(90);
        node->setAngleVar(0);
        
        // radial
        node->setRadialAccel(0);
        node->setRadialAccelVar(0);
        
        // speed of particles
        node->setSpeed(180);
        node->setSpeedVar(50);
        
        // emitter position
        //node->setPosition(Vec2(winSize.width/2, 100));
        node->setPosVar(Vec2(winSize.width/8,0));
        
        // life of particles
        node->setLife(2.0f);
        node->setLifeVar(1);
        
        // emits per frame
        node->setEmissionRate(node->getTotalParticles() /node->getLife());
        
        // color of particles
        Color4F startColor(0.5f, 0.5f, 0.5f, 1.0f);
        node->setStartColor(startColor);
        
        Color4F startColorVar(0.5f, 0.5f, 0.5f, 1.0f);
        node->setStartColorVar( startColorVar);
        
        Color4F endColor(0.1f, 0.1f, 0.1f, 0.2f);
        node->setEndColor(endColor);
        
        Color4F endColorVar(0.1f, 0.1f, 0.1f, 0.2f);
        node->setEndColorVar(endColorVar);
        
        // size, in pixels
        node->setEndSize(4.0f);
        node->setStartSize(4.0f);
        node->setEndSizeVar(0);
        node->setStartSizeVar(0);
        
        // additive
        node->setBlendAdditive(false);

        
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
    
    createTweaker("NParticles", Vec2(40, 200), _nParticleLabel, CC_CALLBACK_1(ProfileParticleBasicTest::onNofParticlesPrev, this), CC_CALLBACK_1(ProfileParticleBasicTest::onNofParticlesNext, this));
    
    // update the displays
    _quantityLabel->setString(StringUtils::format("%d", ProfileParticleTestPolicy::Quantities[_quantityIndex]));
    _positionLabel->setString(ProfileParticleTestPolicy::PositionPolicy[_positionIndex]);
    _visibilityLabel->setString(ProfileParticleTestPolicy::VisibilityPolicy[_visibilityIndex]);
    _nParticleLabel->setString(StringUtils::format("%d", ProfileParticleTestPolicy::NumberOfParticlesPolicy[_nParticleIndex]));
    setHint();
}

std::string ProfileParticleBasicTest::hint() const
{
    return "Profile basic particle.";
}

void ProfileParticleBasicTest::onQuantityPrev(Ref*)
{
    _quantityIndex = --_quantityIndex < 0 ? ProfileParticleTestPolicy::QuantityPolicyCount - 1 : _quantityIndex;
}

void ProfileParticleBasicTest::onQuantityNext(Ref*)
{
    _quantityIndex = ++_quantityIndex % ProfileParticleTestPolicy::QuantityPolicyCount;
}

void ProfileParticleBasicTest::onPositionPrev(Ref*)
{
    _positionIndex = --_positionIndex < 0 ? ProfileParticleTestPolicy::PositionPolicyCount - 1 : _positionIndex;
}

void ProfileParticleBasicTest::onPositionNext(Ref*)
{
    _positionIndex = ++_positionIndex % ProfileParticleTestPolicy::PositionPolicyCount;
}

void ProfileParticleBasicTest::onVisibilityPrev(Ref*)
{
    _visibilityIndex = --_visibilityIndex < 0 ? ProfileParticleTestPolicy::VisibilityPolicyCount - 1 : _visibilityIndex;
}

void ProfileParticleBasicTest::onVisibilityNext(Ref*)
{
    _visibilityIndex = ++_visibilityIndex % ProfileParticleTestPolicy::VisibilityPolicyCount;
}

void ProfileParticleBasicTest::onNofParticlesPrev(Ref*)
{
    _nParticleIndex = --_nParticleIndex < 0 ? ProfileParticleTestPolicy::NumberOfParticlesPolicyCount - 1 : _nParticleIndex;
    toggleNofParticles();
    //recreate();
}

void ProfileParticleBasicTest::onNofParticlesNext(Ref*)
{
    _nParticleIndex = ++ _nParticleIndex % ProfileParticleTestPolicy::NumberOfParticlesPolicyCount;
    toggleNofParticles();
    //recreate();
}

void ProfileParticleBasicTest::toggleNofParticles()
{
    for (int i = 0; i < ProfileParticleTestPolicy::Quantities[_quantityIndex]; ++i)
    {
        auto node = getChildByTag(i);
        static_cast<ParticleSystemQuad*>(node)->setTotalParticles(ProfileParticleTestPolicy::NumberOfParticlesPolicy[_nParticleIndex]);
    }
    _nParticleLabel->setString(StringUtils::format("%d", ProfileParticleTestPolicy::NumberOfParticlesPolicy[_nParticleIndex]));
}


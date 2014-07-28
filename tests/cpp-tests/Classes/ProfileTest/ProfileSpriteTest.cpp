#include "testResource.h"
#include "ProfileSpriteTest.h"

namespace ProfileSpriteTestPolicy
{
    // texture size
    const int TextureSizePolicyCount = 5;
    const std::string TextureSizePolicy[TextureSizePolicyCount] = {"16x16", "32x32", "64x64", "100x100", "misc"};
    std::vector<std::string> image16x16({"ball.png"});
    std::vector<std::string> image32x32({"fire.png", "snow.png", "stars.png", "close.png", "hole_effect.png", "hole_stencil.png"});
    std::vector<std::string> image64x64({"streak.png", "Comet.png", "pattern1.png", "stars2.png"});
    std::vector<std::string> image100x100({"YellowSquare.png", "MagentaSquare.png", "CyanSquare.png", "CyanTriangle.png", "YellowTriangle.png"});
    std::vector<std::string> imageMisc({"powered.png", "Icon.png", "grossinis_sister1.png", "grossinis_sister2.png", "close.png", "grossini.png", "Pea.png", "SpookyPeas.png", "stone.png", "test_image.jpeg", "blocks9.png"});
    std::vector<std::vector<std::string>> TextureSizeImage({image16x16, image32x32, image64x64, image100x100, imageMisc});
    
    // texture pixel format
    const int TexturePixelFormatCount = 7;
    const std::string TexturePixelFormatPolicy[TexturePixelFormatCount] = {"Auto", "RGBA8888", "RGB888", "RGB565", "I8", "AI88", "RGBA4444"};
    const Texture2D::PixelFormat TexturePixelFormat[TexturePixelFormatCount] = {Texture2D::PixelFormat::AUTO, Texture2D::PixelFormat::RGBA8888, Texture2D::PixelFormat::RGB888, Texture2D::PixelFormat::RGB565, Texture2D::PixelFormat::I8, Texture2D::PixelFormat::AI88, Texture2D::PixelFormat::RGBA4444};
    
    // action status
    const int ActionStatusCount = 2;
    const std::string ActionStatusPolicy[ActionStatusCount] = {"Running", "Stopped"};
    
    // batch
    const int BatchPolicyCount = 3;
    const std::string BatchPolicy[BatchPolicyCount] = {"BatchNode", "Auto Batch", "None"};
}

void runProfileSpriteTest(int testIndex)
{
    auto scene = new ProfileSpriteScene();
    if (testIndex == 0)
    {
        auto layer = ProfileSpriteBasicTest::create();
        scene->addChild(layer);
    }
    else if (testIndex == 1)
    {
        auto layer = ProfileSpriteOperationTest::create();
        scene->addChild(layer);
    }
    else if (testIndex == 2)
    {
        auto layer = ProfileSpriteActionTest::create();
        scene->addChild(layer);
    }
    else if (testIndex == 3)
    {
        auto layer = ProfileSpriteBatchTest::create();
        scene->addChild(layer);
    }
    else
    {
        CCASSERT(false, "Invalid test index.");
    }
    Director::getInstance()->replaceScene(scene);
    scene->release();
}

void ProfileSpriteScene::runThisTest()
{
}

void ProfileSpriteScene::onEnter()
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

int ProfileSpriteTest::s_testIndex = 0;

void ProfileSpriteTest::restartCallback(Ref*)
{
    runProfileSpriteTest(s_testIndex);
}

void ProfileSpriteTest::nextCallback(Ref*)
{
    s_testIndex = ++s_testIndex % TestIndexCount;
    runProfileSpriteTest(s_testIndex);
}

void ProfileSpriteTest::backCallback(Ref*)
{
    s_testIndex = --s_testIndex < 0 ? TestIndexCount - 1 : s_testIndex;
    runProfileSpriteTest(s_testIndex);
}

void ProfileSpriteBasicTest::onEnter()
{
    _cbQuantityPrev = CC_CALLBACK_1(ProfileSpriteBasicTest::onQuantityPrev, this);
    _cbQuantityNext = CC_CALLBACK_1(ProfileSpriteBasicTest::onQuantityNext, this);
    _cbPositionPrev = CC_CALLBACK_1(ProfileSpriteBasicTest::onPositionPrev, this);
    _cbPositionNext = CC_CALLBACK_1(ProfileSpriteBasicTest::onPositionNext, this);
    _cbVisibilityPrev = CC_CALLBACK_1(ProfileSpriteBasicTest::onVisibilityPrev, this);
    _cbVisibilityNext = CC_CALLBACK_1(ProfileSpriteBasicTest::onVisibilityNext, this);
    
    _quantityIndex = 0;
    _positionIndex = 0;
    _visibilityIndex = 0;
    _textureSizeIndex = 0;
    _texturePixelFormatIndex = 0;
    
    recreate();
}

void ProfileSpriteBasicTest::recreate()
{
    removeAllChildren();
    
    int quantity = ProfileSpriteTestPolicy::Quantities[_quantityIndex];
    Size winSize = Director::getInstance()->getWinSize();
    
    for (int i = 0; i < quantity; ++i)
    {
        int randomIndex = randomBetween<int>(0, (int)ProfileSpriteTestPolicy::TextureSizeImage[_textureSizeIndex].size() - 1);
        std::string imageFilename = "Images/" + ProfileSpriteTestPolicy::TextureSizeImage[_textureSizeIndex][randomIndex];
        auto node = Sprite::create(imageFilename);
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
    
    // other tweakers
    createTweaker("TextureSize", Vec2(40, 200), _textureSizeLabel,
                  CC_CALLBACK_1(ProfileSpriteBasicTest::onTextureSizePrev, this), CC_CALLBACK_1(ProfileSpriteBasicTest::onTextureSizeNext, this));
    createTweaker("PixelFormat", Vec2(40, 180), _texturePixelFormatLabel, CC_CALLBACK_1(ProfileSpriteBasicTest::onTexturePixelFormatPrev, this), CC_CALLBACK_1(ProfileSpriteBasicTest::onTexturePixelFormatNext, this));
    
    // update the displays
    _quantityLabel->setString(StringUtils::format("%d", ProfileSpriteTestPolicy::Quantities[_quantityIndex]));
    _positionLabel->setString(ProfileSpriteTestPolicy::PositionPolicy[_positionIndex]);
    _visibilityLabel->setString(ProfileSpriteTestPolicy::VisibilityPolicy[_visibilityIndex]);
    _textureSizeLabel->setString(ProfileSpriteTestPolicy::TextureSizePolicy[_textureSizeIndex]);
    _texturePixelFormatLabel->setString(ProfileSpriteTestPolicy::TexturePixelFormatPolicy[_texturePixelFormatIndex]);
    setHint();
}

std::string ProfileSpriteBasicTest::hint() const
{
    return "Profile render sprite ability.";
}

void ProfileSpriteBasicTest::onQuantityPrev(Ref*)
{
    _quantityIndex = --_quantityIndex < 0 ? ProfileSpriteTestPolicy::QuantityPolicyCount - 1 : _quantityIndex;
}

void ProfileSpriteBasicTest::onQuantityNext(Ref*)
{
    _quantityIndex = ++_quantityIndex % ProfileSpriteTestPolicy::QuantityPolicyCount;
}

void ProfileSpriteBasicTest::onPositionPrev(Ref*)
{
    _positionIndex = --_positionIndex < 0 ? ProfileSpriteTestPolicy::PositionPolicyCount - 1 : _positionIndex;
}

void ProfileSpriteBasicTest::onPositionNext(Ref*)
{
    _positionIndex = ++_positionIndex % ProfileSpriteTestPolicy::PositionPolicyCount;
}

void ProfileSpriteBasicTest::onVisibilityPrev(Ref*)
{
    _visibilityIndex = --_visibilityIndex < 0 ? ProfileSpriteTestPolicy::VisibilityPolicyCount - 1 : _visibilityIndex;
}

void ProfileSpriteBasicTest::onVisibilityNext(Ref*)
{
    _visibilityIndex = ++_visibilityIndex % ProfileSpriteTestPolicy::VisibilityPolicyCount;
}

void ProfileSpriteBasicTest::onTextureSizePrev(Ref*)
{
    _textureSizeIndex = --_textureSizeIndex < 0 ? ProfileSpriteTestPolicy::TextureSizePolicyCount - 1 : _textureSizeIndex;
    recreate();
}

void ProfileSpriteBasicTest::onTextureSizeNext(Ref*)
{
    _textureSizeIndex = ++_textureSizeIndex % ProfileSpriteTestPolicy::TextureSizePolicyCount;
    recreate();
}

void ProfileSpriteBasicTest::onTexturePixelFormatPrev(Ref*)
{
    _texturePixelFormatIndex = --_texturePixelFormatIndex < 0 ? ProfileSpriteTestPolicy::TexturePixelFormatCount - 1 : _texturePixelFormatIndex;
    Texture2D::setDefaultAlphaPixelFormat(ProfileSpriteTestPolicy::TexturePixelFormat[_texturePixelFormatIndex]);
    Director::getInstance()->getTextureCache()->removeAllTextures();
    recreate();
}

void ProfileSpriteBasicTest::onTexturePixelFormatNext(Ref*)
{
    _texturePixelFormatIndex = ++_texturePixelFormatIndex % ProfileSpriteTestPolicy::TexturePixelFormatCount;
    Texture2D::setDefaultAlphaPixelFormat(ProfileSpriteTestPolicy::TexturePixelFormat[_texturePixelFormatIndex]);
    Director::getInstance()->getTextureCache()->removeAllTextures();
    recreate();
}

std::string ProfileSpriteOperationTest::hint() const
{
    return "Profile sprite add/remove children.";
}

void ProfileSpriteOperationTest::update(float dt)
{
}

std::string ProfileSpriteActionTest::hint() const
{
    return "Profile sprite actions.";
}

void ProfileSpriteActionTest::recreate()
{
    removeAllChildren();
    
    int quantity = ProfileSpriteTestPolicy::Quantities[_quantityIndex];
    Size winSize = Director::getInstance()->getWinSize();
    
    for (int i = 0; i < quantity; ++i)
    {
        int randomIndex = randomBetween<int>(0, (int)ProfileSpriteTestPolicy::TextureSizeImage[_textureSizeIndex].size() - 1);
        std::string imageFilename = "Images/" + ProfileSpriteTestPolicy::TextureSizeImage[_textureSizeIndex][randomIndex];
        auto node = Sprite::create(imageFilename);
        addChild(node, 0, i);
        
        node->runAction(getOneAction());
        
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
    
    // other tweakers
    createTweaker("TextureSize", Vec2(40, 200), _textureSizeLabel,
                  CC_CALLBACK_1(ProfileSpriteBasicTest::onTextureSizePrev, this), CC_CALLBACK_1(ProfileSpriteBasicTest::onTextureSizeNext, this));
    createTweaker("PixelFormat", Vec2(40, 180), _texturePixelFormatLabel, CC_CALLBACK_1(ProfileSpriteBasicTest::onTexturePixelFormatPrev, this), CC_CALLBACK_1(ProfileSpriteBasicTest::onTexturePixelFormatNext, this));
    createTweaker("Action", Vec2(40, 160), _actionStatusLabel, CC_CALLBACK_1(ProfileSpriteActionTest::onActionStatusPrev, this), CC_CALLBACK_1(ProfileSpriteActionTest::onActionStatusNext, this));
    
    _actionStatusIndex = 0;
    
    // update the displays
    _quantityLabel->setString(StringUtils::format("%d", ProfileSpriteTestPolicy::Quantities[_quantityIndex]));
    _positionLabel->setString(ProfileSpriteTestPolicy::PositionPolicy[_positionIndex]);
    _visibilityLabel->setString(ProfileSpriteTestPolicy::VisibilityPolicy[_visibilityIndex]);
    _textureSizeLabel->setString(ProfileSpriteTestPolicy::TextureSizePolicy[_textureSizeIndex]);
    _texturePixelFormatLabel->setString(ProfileSpriteTestPolicy::TexturePixelFormatPolicy[_texturePixelFormatIndex]);
    _actionStatusLabel->setString(ProfileSpriteTestPolicy::ActionStatusPolicy[_actionStatusIndex]);
    setHint();
}

Action* ProfileSpriteActionTest::getOneAction() const
{
    int choice = randomBetween<int>(0, 3);
    Size size = Director::getInstance()->getWinSize();
    float timeDisp = CCRANDOM_0_1();
    float positionDispX = CCRANDOM_0_1();
    float positionDispY = CCRANDOM_0_1();
    
    Sequence* seq;
    if (choice == 0)
    {
        auto move = MoveBy::create(10.0f * timeDisp, Vec2(size.width * positionDispX, size.height * positionDispY));
        seq = Sequence::create(move, move->reverse(), nullptr);
    }
    else if (choice == 1)
    {
        auto rotate = RotateBy::create(10.0f * timeDisp, 360.0f);
        seq = Sequence::create(rotate, nullptr);
    }
    else if (choice == 2)
    {
        auto scale = ScaleBy::create(10.0f * timeDisp, 10.0f * positionDispX);
        seq = Sequence::create(scale, scale->reverse(), nullptr);
    }
    else if (choice == 3)
    {
        auto skew = SkewBy::create(10.0f * timeDisp, 90.0f + 30.0f * positionDispX, 30.0f * positionDispY);
        seq = Sequence::create(skew, skew->reverse(), nullptr);
    }
    else
    {
        CCASSERT(false, "Invalid choice.");
    }
    
    return RepeatForever::create(seq);
}

void ProfileSpriteActionTest::onActionStatusPrev(Ref*)
{
    _actionStatusIndex = --_actionStatusIndex < 0 ? ProfileSpriteTestPolicy::ActionStatusCount - 1 : _actionStatusIndex;
    toggleActionStatus();
}

void ProfileSpriteActionTest::onActionStatusNext(Ref*)
{
    _actionStatusIndex = ++_actionStatusIndex % ProfileSpriteTestPolicy::ActionStatusCount;
    toggleActionStatus();
}

void ProfileSpriteActionTest::toggleActionStatus()
{
    int quantity = ProfileSpriteTestPolicy::Quantities[_quantityIndex];
    for (int i = 0; i < quantity; ++i)
    {
        auto child = getChildByTag(i);
        if (_actionStatusIndex == 0)
        {
            child->runAction(getOneAction());
        }
        else if (_actionStatusIndex == 1)
        {
            child->stopAllActions();
        }
        else
        {
            CCASSERT(false, "Invalide action status index.");
        }
    }
    _actionStatusLabel->setString(ProfileSpriteTestPolicy::ActionStatusPolicy[_actionStatusIndex]);
}

std::string ProfileSpriteBatchTest::hint() const
{
    return "Profile sprite batch ability.";
}

void ProfileSpriteBatchTest::onEnter()
{
    _cbQuantityPrev = CC_CALLBACK_1(ProfileSpriteBasicTest::onQuantityPrev, this);
    _cbQuantityNext = CC_CALLBACK_1(ProfileSpriteBasicTest::onQuantityNext, this);
    _cbPositionPrev = CC_CALLBACK_1(ProfileSpriteBasicTest::onPositionPrev, this);
    _cbPositionNext = CC_CALLBACK_1(ProfileSpriteBasicTest::onPositionNext, this);
    _cbVisibilityPrev = CC_CALLBACK_1(ProfileSpriteBasicTest::onVisibilityPrev, this);
    _cbVisibilityNext = CC_CALLBACK_1(ProfileSpriteBasicTest::onVisibilityNext, this);
    
    _quantityIndex = 0;
    _positionIndex = 0;
    _visibilityIndex = 0;
    _batchIndex = 0;
    
    recreate();
}

void ProfileSpriteBatchTest::recreate()
{
    removeAllChildren();
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_s9s_blocks9_plist);
    _batchNode = SpriteBatchNode::create(s_s9s_blocks9);
    addChild(_batchNode);
    
    const int BatchNodeImagesCount = 5;
    std::string BatchNodeImages[BatchNodeImagesCount] = {"blocks9.png", "blocks9r.png", "grossini_dance_01.png", "grossini_dance_02.png", "grossini_dance_03.png"};
    
    int quantity = ProfileSpriteTestPolicy::Quantities[_quantityIndex];
    Size winSize = Director::getInstance()->getWinSize();
    
    for (int i = 0; i < quantity; ++i)
    {
        Sprite* node(nullptr);
        
        if (_batchIndex == 0)
        {
            // use batch node
            int randomIndex = randomBetween<int>(0, BatchNodeImagesCount - 1);
            std::string imageFilename = BatchNodeImages[randomIndex];
            node = Sprite::createWithSpriteFrameName(imageFilename);
            _batchNode->addChild(node);
        }
        else if (_batchIndex == 1)
        {
            // try auto batch
            int fifth = quantity / 5;
            int index = i / fifth;
            index = std::max(std::min(index, 4), 0);
            std::string imageFilename = BatchNodeImages[index];
            node = Sprite::create("Images/" + imageFilename);
            addChild(node);
        }
        else if (_batchIndex == 2)
        {
            // no batch
            int randomIndex = randomBetween<int>(0, BatchNodeImagesCount - 1);
            std::string imageFilename = BatchNodeImages[randomIndex];
            node = Sprite::create("Images/" + imageFilename);
            addChild(node);
        }
        else
        {
            CCASSERT(false, "Invalid batch policy.");
        }
        
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
    
    createTweaker("Batch", Vec2(40, 200), _batchLabel, CC_CALLBACK_1(ProfileSpriteBatchTest::onBatchPrev, this), CC_CALLBACK_1(ProfileSpriteBatchTest::onBatchNext, this));
    
    // update the displays
    _quantityLabel->setString(StringUtils::format("%d", ProfileSpriteTestPolicy::Quantities[_quantityIndex]));
    _positionLabel->setString(ProfileSpriteTestPolicy::PositionPolicy[_positionIndex]);
    _visibilityLabel->setString(ProfileSpriteTestPolicy::VisibilityPolicy[_visibilityIndex]);
    _batchLabel->setString(ProfileSpriteTestPolicy::BatchPolicy[_batchIndex]);
}

void ProfileSpriteBatchTest::onBatchPrev(Ref*)
{
    _batchIndex = --_batchIndex < 0 ? ProfileSpriteTestPolicy::BatchPolicyCount - 1 : _batchIndex;
    recreate();
}

void ProfileSpriteBatchTest::onBatchNext(Ref*)
{
    _batchIndex = ++_batchIndex % ProfileSpriteTestPolicy::BatchPolicyCount;
    recreate();
}


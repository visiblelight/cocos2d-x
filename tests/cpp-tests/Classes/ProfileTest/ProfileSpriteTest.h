#ifndef _PROFILE_SPRITE_TEST_HEADER_
#define _PROFILE_SPRITE_TEST_HEADER_
#include <vector>
#include "cocos2d.h"
#include "../testBasic.h"
#include "../BaseTest.h"
#include "ProfileTest.h"

void runProfileSpriteTest(int testIndex = 0);

class ProfileSpriteScene : public TestScene
{
public:
    virtual void runThisTest();
    void onEnter() override;
};

namespace ProfileSpriteTestPolicy
{
    // basic test
    const int QuantityPolicyCount = 11;
    const int Quantities[QuantityPolicyCount] = {0, 5, 10, 20, 40, 100, 200, 400, 800, 2000, 50000};
    const int PositionPolicyCount = 3;
    const std::string PositionPolicy[PositionPolicyCount] = {"All inside CCV", "Half inside CCV", "All outside CCV"};
    const int VisibilityPolicyCount = 3;
    const std::string VisibilityPolicy[VisibilityPolicyCount] = {"All Visible", "Half Visible", "All Invisible"};
    
    // operation test
    const int OperationPolicyCount = 3;
    const std::string OperationPolicy[OperationPolicyCount] = {"Add/Remove", "Enumerate"};
}

class ProfileSpriteTest : public ProfileLayer
{
    static int s_testIndex;
    static const int TestIndexCount = 4;
public:
    void nextCallback(Ref*) override;
    void backCallback(Ref*) override;
    void restartCallback(Ref*) override;
};

class ProfileSpriteBasicTest : public ProfileSpriteTest
{
public:
    CREATE_FUNC(ProfileSpriteBasicTest);
    void onEnter() override;
    virtual ~ProfileSpriteBasicTest() { Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::AUTO); }
    
protected:
    void recreate() override;
    std::string hint() const override;
    
public:
    // not override, might be referred in the derived class
    void onQuantityPrev(Ref*);
    void onQuantityNext(Ref*);
    void onPositionPrev(Ref*);
    void onPositionNext(Ref*);
    void onVisibilityPrev(Ref*);
    void onVisibilityNext(Ref*);
    void onTextureSizePrev(Ref*);
    void onTextureSizeNext(Ref*);
    void onTexturePixelFormatPrev(Ref*);
    void onTexturePixelFormatNext(Ref*);
    
protected:
    int _quantityIndex;
    int _positionIndex;
    int _visibilityIndex;
    int _textureSizeIndex;
    int _texturePixelFormatIndex;
    
    Label* _textureSizeLabel;
    Label* _texturePixelFormatLabel;
};

class ProfileSpriteOperationTest : public ProfileSpriteBasicTest
{
public:
    CREATE_FUNC(ProfileSpriteOperationTest);
    
protected:
    void update(float dt) override;
    //void recreate() override;
    std::string hint() const override;
};

class ProfileSpriteActionTest : public ProfileSpriteBasicTest
{
public:
    CREATE_FUNC(ProfileSpriteActionTest);
    
protected:
    std::string hint() const override;
    void recreate() override;
    Action* getOneAction() const;
    
    void onActionStatusPrev(Ref*);
    void onActionStatusNext(Ref*);
    void toggleActionStatus();
    
    int _actionStatusIndex;
    Label* _actionStatusLabel;
};

class ProfileSpriteBatchTest : public ProfileSpriteBasicTest
{
public:
    CREATE_FUNC(ProfileSpriteBatchTest);
    void onEnter() override;
    
protected:
    std::string hint() const override;
    void recreate() override;
    void onBatchPrev(Ref*);
    void onBatchNext(Ref*);
    
protected:
    SpriteBatchNode* _batchNode;
    
    int _batchIndex;
    Label* _batchLabel;
};

#endif

#ifndef _PROFILE_NODE_TEST_HEADER_
#define _PROFILE_NODE_TEST_HEADER_
#include "cocos2d.h"
#include "../testBasic.h"
#include "../BaseTest.h"
#include "ProfileTest.h"

//**************************************************************************/
// Profile Node Test
// This test mainly focus on the efficiency of scene graph.
//**************************************************************************/

void runProfileNodeTest(int testIndex = 0);

class ProfileNodeScene : public TestScene
{
public:
    virtual void runThisTest();
    void onEnter() override;
};

namespace ProfileNodeTestPolicy
{
    // basic test
    const int QuantityPolicyCount = 9;
    const int Quantities[QuantityPolicyCount] = {0, 5, 10, 20, 40, 100, 1000, 10000, 100000};
    const int PositionPolicyCount = 3;
    const std::string PositionPolicy[PositionPolicyCount] = {"All inside CCV", "Half inside CCV", "All outside CCV"};
    const int VisibilityPolicyCount = 3;
    const std::string VisibilityPolicy[VisibilityPolicyCount] = {"All Visible", "Half Visible", "All Invisible"};
    
    // operation test
    const int OperationPolicyCount = 3;
    const std::string OperationPolicy[OperationPolicyCount] = {"Add/Remove", "Enumerate"};
}

class ProfileNodeTest : public ProfileLayer
{
    static int s_testIndex;
    static const int TestIndexCount = 2;
public:
    void nextCallback(Ref*) override;
    void backCallback(Ref*) override;
    void restartCallback(Ref*) override;
};

class ProfileNodeBasicTest : public ProfileNodeTest
{
public:
    CREATE_FUNC(ProfileNodeBasicTest);
    void onEnter() override;
    
protected:
    // not override
    void onQuantityPrev(Ref*);
    void onQuantityNext(Ref*);
    void onPositionPrev(Ref*);
    void onPositionNext(Ref*);
    void onVisibilityPrev(Ref*);
    void onVisibilityNext(Ref*);
    
    void recreate() override;
    std::string hint() const override;
    
protected:
    int _quantityIndex;
    int _positionIndex;
    int _visibilityIndex;
};

class ProfileNodeOperationTest : public ProfileNodeBasicTest
{
public:
    CREATE_FUNC(ProfileNodeOperationTest);
    
protected:
    void update(float dt) override;
    void recreate() override;
    std::string hint() const override;
};

#endif

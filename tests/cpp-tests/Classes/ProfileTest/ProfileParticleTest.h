#ifndef _PROFILE_PARTICLE_TEST_HEADER_
#define _PROFILE_PARTICLE_TEST_HEADER_
#include "cocos2d.h"
#include "../testBasic.h"
#include "../BaseTest.h"
#include "ProfileTest.h"

void runProfileParticleTest(int testIndex = 0);

class ProfileParticleScene : public TestScene
{
public:
    virtual void runThisTest();
    void onEnter() override;
};

class ProfileParticleTest : public ProfileLayer
{
    static int s_testIndex;
    static const int TestIndexCount = 1;
public:
    void nextCallback(Ref*) override;
    void backCallback(Ref*) override;
    void restartCallback(Ref*) override;
};

namespace ProfileParticleTestPolicy
{
    // basic test
    const int QuantityPolicyCount = 9;
    const int Quantities[QuantityPolicyCount] = {0, 1, 2, 4, 8, 16, 32, 64, 128};
    const int PositionPolicyCount = 3;
    const std::string PositionPolicy[PositionPolicyCount] = {"All inside CCV", "Half inside CCV", "All outside CCV"};
    const int VisibilityPolicyCount = 3;
    const std::string VisibilityPolicy[VisibilityPolicyCount] = {"All Visible", "Half Visible", "All Invisible"};
    const int NumberOfParticlesPolicyCount = 5;
    const int NumberOfParticlesPolicy[NumberOfParticlesPolicyCount] = {200, 400, 800, 1600, 3200};
}

class ProfileParticleBasicTest : public ProfileParticleTest
{
public:
    CREATE_FUNC(ProfileParticleBasicTest);
    void onEnter() override;
    
protected:
    // not override
    void onQuantityPrev(Ref*);
    void onQuantityNext(Ref*);
    void onPositionPrev(Ref*);
    void onPositionNext(Ref*);
    void onVisibilityPrev(Ref*);
    void onVisibilityNext(Ref*);
    void onNofParticlesPrev(Ref*);
    void onNofParticlesNext(Ref*);
    void toggleNofParticles();
    
    void recreate() override;
    std::string hint() const override;
    
protected:
    int _quantityIndex;
    int _positionIndex;
    int _visibilityIndex;
    int _nParticleIndex;
    
    Label* _nParticleLabel;
};

#endif

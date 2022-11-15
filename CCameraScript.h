#pragma once
#include "CScript.h"
class CCameraScript :
    public CScript
{
private:
    float           m_fSpeed;
    float           m_fAcctime;

    Vec3            m_vDir; // 플레이어 방향 벡터

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void Move();


    CLONE(CCameraScript);
public:
    CCameraScript();
    ~CCameraScript();
};


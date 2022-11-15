#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    UINT                        m_iMaxCount;        // 파티클의 매틱 마다 갯수

    Vec2                        m_vStartScale;      // 시작 크기 
    Vec2                        m_vEndScale;        // 끝날때 크기

    Vec4                        m_vStartColor;      // 시작 색
    Vec4                        m_vEndColor;        // 끝날때 색

    float                       m_fMinLifeTime;     // 최소 살아있는 시간
    float                       m_fMaxLifeTime;     // 최대 살아있는 시간

    float                       m_fFrequency;       // 진동수

    CStructuredBuffer*          m_ParticleBuffer;   // 구조화버퍼 기반 파티클 버퍼
    Ptr<CParticleUpdateShader>  m_UpdateCS;         // 컴퓨트쉐이더 기반 파이클 쉐이더

public:
    virtual void finaltick() override;
    virtual void render() override;

    CLONE(CParticleSystem);

public:
    CParticleSystem();
    ~CParticleSystem();
};


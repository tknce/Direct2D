#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleUpdateShader :
    public CComputeShader
{
    CStructuredBuffer* m_ParticleBuffer;

public:
    void SetParticleBuffer(CStructuredBuffer* _Buffer);

public:
    virtual void UpdateData() override;
    virtual void Clear() override;


public:
    CParticleUpdateShader();
    ~CParticleUpdateShader();
};


#pragma once
#include "CRes.h"

class CShader :
    public CRes
{
protected:
    ComPtr<ID3DBlob>    m_ErrBlob;

public:
    virtual int Load(const wstring& _strFilePath) override { return S_OK; }

    CLONE_ASSERT(CShader);
public:
    CShader(RES_TYPE _eResType);
    ~CShader();
};


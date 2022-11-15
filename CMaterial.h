#pragma once
#include "CRes.h"
#include "CGraphicsShader.h"

#include "CTexture.h"

class CMaterial :
    public CRes
{
private:
    tMtrlConst              m_tConst;                   // 상스바파
    Ptr<CTexture>           m_arrTex[TEX_END];          // 바인딩된 텍스쳐 찾기
    Ptr<CGraphicsShader>    m_pShader;                  // 바인딩된 쉐이더

    Ptr<CMaterial>          m_pMasterMtrl;              // 바인딩된 재질     다들 이 재질을 쓰다가 잠깐 상태값이 변하면 빠져나갔다가 돌아온다.

public:
    void SetScalarParam(SCALAR_PARAM _eScalarType, void* _pData);   // 스칼라값을 바인딩할 곳 세팅
    void SetTexParam(TEX_PARAM _eTex, Ptr<CTexture> _pTex);         // 텍스쳐를 바인딩할 곳 세팅

    void SetShader(Ptr<CGraphicsShader> _pShader) { m_pShader = _pShader;}  // 쉐이더 설정
    Ptr<CGraphicsShader> GetShader() { return m_pShader; }                  // 설정된 곳의 쉐이더 얻어오기

    void UpdateData();
    static void Clear();                                                    // 바인딩된 모든 리소스 클리어

    virtual int Load(const wstring& _strFilePath) { return S_OK; }


    CLONE(CMaterial);
public:
    CMaterial();
    CMaterial(const CMaterial& _other);
    ~CMaterial();
};


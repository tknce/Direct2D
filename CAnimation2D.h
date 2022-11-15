#pragma once
#include "CEntity.h"

#include "CTexture.h"
class CAnimator2D;

class CAnimation2D :
    public CEntity
{
private:
    vector<tAnim2DFrm>  m_vecFrm;
    int                 m_iCurIdx;

    CAnimator2D*        m_pOwner;
    Ptr<CTexture>       m_AtlasTex;

    float               m_fAccTime;
    bool                m_bFinish;

public:
    void finaltick();

public:
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    void Reset()
    {
        m_fAccTime = 0.f;
        m_iCurIdx = 0;
        m_bFinish = false;
    }


    void UpdateData();
    void Clear();

    CLONE(CAnimation2D);

public:
    CAnimation2D();
    ~CAnimation2D();

    friend class CAnimator2D;
        
};


#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnimation2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnimation2D*> m_mapAnim;
    CAnimation2D*               m_pCurAnim;

    bool                        m_bRepeat;

public:
    virtual void finaltick() override;

public:
    void CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 Lefttop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS);
    CAnimation2D* FindAnimation(const wstring& _strKey);

    void Play(const wstring& _strKey, bool _bRepeat);

    void UpdateDate();
    void Clear();

public:
    CLONE(CAnimator2D);

public:

    CAnimator2D();
    ~CAnimator2D();
};


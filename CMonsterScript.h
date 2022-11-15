#pragma once
#include "CScript.h"

class CGameObject;

class CMonsterScript :
    public CScript
{
private:
    CGameObject*        m_pPlayer;
    Vec3                m_vDir;
    Vec3                m_vPlayerDir;

    bool                m_bDead;
    bool                m_bColliderState;

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    void SetDir(Vec3 _Dir) { m_vPlayerDir = _Dir; }
    void Defend(CCollider2D* _pOther);


public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;


    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();
};


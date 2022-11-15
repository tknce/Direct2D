#pragma once
#include "CScript.h"

struct PlayerInfo
{
    int HP;

    float Speed;
};


class CPlayerScript :
    public CScript
{
    PlayerInfo  playerinfo;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    PlayerInfo Getinfo() { return playerinfo; }
    void Setinfo(PlayerInfo _info) { playerinfo = _info; }

public:
    virtual void BeginOverlap(CCollider2D* _pOther) override;
    virtual void Overlap(CCollider2D* _pOther) override;
    virtual void EndOverlap(CCollider2D* _pOther) override;


    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};


#pragma once

#include "CEntity.h"
#include "CLayer.h"

class CGameObject;

class CLevel :
    public CEntity 
{
private:
    CLayer          m_arrLayer[MAX_LAYER];

public:
    // 시점 함수
    void begin();
    void tick();
    void finaltick();


public:
    CLayer* GetLayer(UINT _iIdx) { return &m_arrLayer[_iIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);

    void AddGameObject(CGameObject* _pObject, UINT _iLayerIdx);
    void AddGameObject(CGameObject* _pObject, const wstring& _strLayerName);

    CLONE_DUMMY(CLevel);
public:
    CLevel();
    virtual ~CLevel();
};


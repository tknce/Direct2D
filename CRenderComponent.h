#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;

    Ptr<CMaterial>          m_pSharedMtrl;  // 공유 재질            똑같은 재질을 쓸 경우 같은 재질을 쓰도록 설계 -> 최적화를 위해
    Ptr<CMaterial>          m_pDynamicMtrl; // 동적 재질            똑같은 재질을 쓰다가 피격판정을 입거나 그 몬스터만 다르게 적용되어야된다면 바꿔줌
    Ptr<CMaterial>          m_pCurMtrl;     // 현재 사용 중인 재질


public:
    virtual void render() = 0;

public:
    void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; }       // 네모난걸 쓸지 동그란걸 쓸지 설정
    Ptr<CMesh> GetMesh() { return m_pMesh; }

    void SetSharedMaterial(Ptr<CMaterial> _pMtrl) { m_pSharedMtrl = _pMtrl; m_pCurMtrl = _pMtrl; }; // 공유재질 설정
    Ptr<CMaterial> GetSharedMaterial();


    Ptr<CMaterial> GetCurMaterial() { return m_pCurMtrl; }      // 현재 재질 가져오기
    Ptr<CMaterial> GetDynamicMaterial();                        // 공유 재질 가져오기

public:
    virtual CRenderComponent* Clone() = 0;

public:
    CRenderComponent(COMPONENT_TYPE _eComponentType);
    CRenderComponent(const CRenderComponent& _origin);
    ~CRenderComponent();
};


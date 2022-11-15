#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _eType)
	: CComponent(_eType)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _origin)
	: CComponent(_origin)
	, m_pMesh(_origin.m_pMesh)
	, m_pSharedMtrl(_origin.m_pSharedMtrl)
	, m_pDynamicMtrl(nullptr)
	, m_pCurMtrl(nullptr)
{
	if (_origin.m_pCurMtrl == _origin.m_pSharedMtrl)
	{
		m_pCurMtrl = m_pSharedMtrl;
	}
	else if (_origin.m_pCurMtrl == _origin.m_pDynamicMtrl)
	{
		GetDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{	
}


Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	// 현재 재질을 공유재질로 바꾼다.
	m_pCurMtrl = m_pSharedMtrl;

	// 동적재질이 비어있지 않다면 비어준다.
	if (nullptr != m_pDynamicMtrl)
	{
		m_pDynamicMtrl = nullptr;
	}

	// 공유재질을 리턴시킨다.
	return m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// 동적재질이 안비어있으면 동적재질을 보낸다.
	if (nullptr != m_pDynamicMtrl)
		return m_pDynamicMtrl;

	// 비어있지 않다면 새로운 재질 생성
	m_pDynamicMtrl = new CMaterial(*m_pSharedMtrl.Get());
	// 복사된 재질을 클론으로 이름을 짓는다.
	m_pDynamicMtrl->SetName(m_pSharedMtrl->GetName() + L"_Clone");
	// 현재 재질을 동적대질로 바꾼다.
	m_pCurMtrl = m_pDynamicMtrl;

	return m_pCurMtrl;
}

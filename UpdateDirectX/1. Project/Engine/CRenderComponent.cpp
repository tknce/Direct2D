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
	//m_pDynamicMtrl = nullptr;
}


Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_pCurMtrl = m_pSharedMtrl;

	//if (nullptr != m_pDynamicMtrl)
	//{
	//	m_pDynamicMtrl = nullptr;
	//}

	return m_pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (nullptr != m_pDynamicMtrl)
		return m_pDynamicMtrl;

	m_pDynamicMtrl = m_pSharedMtrl->Clone();
	m_pDynamicMtrl->SetName(m_pSharedMtrl->GetName() + L"_Clone");
	m_pCurMtrl = m_pDynamicMtrl;

	return m_pCurMtrl;
}

void CRenderComponent::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	SaveResourceRef<CMesh>(m_pMesh, _File);
	SaveResourceRef<CMaterial>(m_pSharedMtrl, _File);
}

void CRenderComponent::LoadFromFile(FILE* _File)
{
	LoadResourceRef<CMesh>(m_pMesh, _File);
	LoadResourceRef<CMaterial>(m_pSharedMtrl, _File);
	m_pCurMtrl = m_pSharedMtrl;
}

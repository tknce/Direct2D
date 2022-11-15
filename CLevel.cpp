#include "pch.h"
#include "CLevel.h"


CLevel::CLevel()
{
	m_arrLayer[0].SetName(L"Default");

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].m_iLayerIdx = i;
	}
}

CLevel::~CLevel()
{

}


void CLevel::begin()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].begin();
	}
}

void CLevel::tick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].clear();
	}

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].tick();
	}
}

void CLevel::finaltick()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_arrLayer[i].finaltick();
	}
}



CLayer* CLevel::GetLayer(const wstring& _strLayerName)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (_strLayerName == m_arrLayer[i].GetName())
			return &m_arrLayer[i];
	}
	
	return nullptr;
}

void CLevel::AddGameObject(CGameObject* _pObject, UINT _iLayerIdx)
{
	assert(0 <= _iLayerIdx && _iLayerIdx < MAX_LAYER);

	m_arrLayer[_iLayerIdx].AddGameObject(_pObject);
}

void CLevel::AddGameObject(CGameObject* _pObject, const wstring& _strLayerName)
{
	CLayer* pLayer = GetLayer(_strLayerName);

	assert(pLayer);

	pLayer->AddGameObject(_pObject);
}

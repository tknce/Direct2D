#include "pch.h"
#include "CRenderMgr.h"

#include "GlobalComponent.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"



CRenderMgr::CRenderMgr()
	: m_pLight2DBuffer(nullptr)
{
	m_pLight2DBuffer = new CStructuredBuffer();
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr);
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pLight2DBuffer)
		delete m_pLight2DBuffer;
}

void CRenderMgr::init()
{
}

void CRenderMgr::tick()
{
	m_vecCam.clear();		// 캠클리어 위치 조정 랜더되자마자 없어지면 얻는 쪽에서 항상 빈곳을 찾기 떄문에
	m_vecLight2D.clear();	// 빛도 마찬가지
}

void CRenderMgr::render()
{
	// 2D 광원 업데이트
	UpdateLight2D();

	// GlobalDate 업데이트
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	pGlobalCB->UpdateData_CS();

	// 렌더링의 기준을 카메라로 설정
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();
	}


}

void CRenderMgr::UpdateLight2D()
{
	// 버퍼 공간이 모자랄 경우 재할당
	if (m_pLight2DBuffer->GetElementCount() < (UINT)m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementCount(), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, nullptr);
	}

	// 광원정보를 구조화버퍼로 옮김
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), (UINT)m_vecLight2D.size());

	// 구조화 버퍼를 t13 레지스터에 바인딩
	m_pLight2DBuffer->UpdateData(13, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	// 광원 개수 글로벌 데이터에 업데이트
	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}



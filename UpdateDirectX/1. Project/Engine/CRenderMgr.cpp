#include "pch.h"
#include "CRenderMgr.h"

#include "GlobalComponent.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CLevel.h"

#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"



CRenderMgr::CRenderMgr()
	: m_pLight2DBuffer(nullptr)
	, m_EditorCam(nullptr)
{
	m_pLight2DBuffer = new CStructuredBuffer();
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr, true);
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pLight2DBuffer)
		delete m_pLight2DBuffer;
}

void CRenderMgr::init()
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();

	// 후처리 용도, 렌더타겟 복사용
	m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
		, (UINT)vRenderResol.x, (UINT)vRenderResol.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}

void CRenderMgr::tick()
{
	m_vecCam.clear();
	m_vecLight2D.clear();
}

void CRenderMgr::render()
{
	// 렌더타겟 지정
	static Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	static Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// NoiseTexture Update
	UpdateNoiseTexture();

	// 2D 광원 업데이트
	UpdateLight2D();

	// 3D 광원 업데이트
	//UpdateLight3D();

	// GlobalData 업데이트
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	pGlobalCB->UpdateData_CS();

	// 렌더링의 기준을 카메라로 설정
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		render_game();
	}
	else
	{
		render_editor();
	}

	// Post Process

}

void CRenderMgr::render_game()
{
	// 렌더링의 기준을 카메라로 설정
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();

	}
}

void CRenderMgr::render_editor()
{
	assert(m_EditorCam);

	m_EditorCam->render();
}

void CRenderMgr::UpdateNoiseTexture()
{
	Ptr<CTexture> NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_03.jpg");
	NoiseTex->UpdateData(15, PIPELINE_STAGE::ALL_STAGE);
	NoiseTex->UpdateData_CS(15, true);

	g_global.vNoiseResolution = Vec2(NoiseTex->GetWidth(), NoiseTex->GetHeight());
}

void CRenderMgr::UpdateLight2D()
{
	// 버퍼 공간이 모자랄 경우 재할당
	if (m_pLight2DBuffer->GetElementCount() < (UINT)m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementSize(), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, nullptr, true);
	}

	// 광원정보를 구조화버퍼로 옮김
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), (UINT)m_vecLight2D.size()); 	

	// 구조화 버퍼를 t13 레지스터에 바인딩
	m_pLight2DBuffer->UpdateData(13, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS); 	

	// 광원 개수 글로벌 데이터에 업데이트
	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}


void CRenderMgr::CopyRenderTarget()
{
	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

	// 바인딩 해제
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->PSSetShaderResources(60, 1, &SRV);

	// 렌더타겟 복사받기
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), RTTex->GetTex2D().Get());

	// 60 레지스터 바인딩
	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}

CCamera* CRenderMgr::GetMainCam()
{
	if (LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetLevelState())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_EditorCam;
	}
}
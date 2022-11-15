#include "pch.h"
#include "CParticleSystem.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(100)
	, m_fMinLifeTime(0.f)
	, m_fMaxLifeTime(0.f)
	, m_ParticleBuffer(nullptr)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"SmokeParticle"));



	// 구조화버퍼 생성
	tParticle arrParticle[1000] = {};
	Vec4 vStartPos = Vec4(-800, -450, 0.f, 0.f);
	for (int i = 0; i < m_iMaxCount; ++i)
	{
		arrParticle[i].vRelativePos = Vec3(0.f, 0.f, 100.f);
		arrParticle[i].vDir = Vec3(cosf((float)i * (XM_2PI / (float)m_iMaxCount)), sinf((float)i * (XM_2PI / 100.f)), 0.f);
		arrParticle[i].iActive = 1;
		arrParticle[i].fSpeed = 200.f;
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, arrParticle);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}

void CParticleSystem::finaltick()
{
	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->Excute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();
	m_ParticleBuffer->UpdateData(15, PIPELINE_STAGE::VS |PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);

	GetCurMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);

	m_ParticleBuffer->Clear();
}




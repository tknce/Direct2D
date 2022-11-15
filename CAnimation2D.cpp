#include "pch.h"
#include "CAnimation2D.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CTimeMgr.h"
#include "CTexture.h"

CAnimation2D::CAnimation2D()
	: m_iCurIdx(-1)
	, m_pOwner(nullptr)
	, m_fAccTime(0.f)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::finaltick()
{
	// 애니메이션이 끝났을때 반복재생이 아니면 리턴
	if (m_bFinish)
		return;

	// 시간 체크
	m_fAccTime += DT;

	// 누적 시간이 해당 프레임 유지시간을 넘어서면 다음프레임으로 넘어감
	if (m_vecFrm[m_iCurIdx].fDuration < m_fAccTime)
	{
		m_fAccTime = 0.f;
		++m_iCurIdx;

		// 최대 프레임에 도달하면 Finish 상태로 전환
		if (m_vecFrm.size() <= m_iCurIdx)
		{
			m_iCurIdx = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnimation2D::Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
	// Animation Name
	SetName(_strKey);

	// Atlas Texture
	m_AtlasTex = _AtlasTex;

	float fWidth = (float)m_AtlasTex->GetWidth();
	float fHeight = (float)m_AtlasTex->GetHeight();

	// Frame Info
	for (int i = 0; i < _iMaxFrm; ++i)
	{
		tAnim2DFrm frm = {};
		frm.vLeftTop = Vec2(_vLeftTop.x + _fStep * (float)i / fWidth, _vLeftTop.y / fHeight);
		frm.vSlice = Vec2(_vSlice.x / fWidth, _vSlice.y / fHeight);
		frm.fDuration = 1.f / _FPS;

		frm.vFullSize = Vec2(200.f / fWidth, 200.f / fHeight);

		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::UpdateData()
{
	// 상수버퍼 세팅
	m_AtlasTex->UpdateData(12, PIPELINE_STAGE::PS);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	tAnim2DInfo info = {};
	info.iAnim2DUse = 1;
	info.vLeftTop = m_vecFrm[m_iCurIdx].vLeftTop;
	info.vSlice = m_vecFrm[m_iCurIdx].vSlice;
	info.vFullSize = m_vecFrm[m_iCurIdx].vFullSize;
	info.vOffset = m_vecFrm[m_iCurIdx].vOffset;

	pCB->SetData(&info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}

void CAnimation2D::Clear()
{
	CTexture::Clear(12);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	tAnim2DInfo info = {};
	info.iAnim2DUse = 0;

	pCB->SetData(&info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}



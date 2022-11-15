#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

#include "CTexture.h"


CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_bRepeat(false)
{
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (!IsVaild(m_pCurAnim))
		return;
	this;
	// Animation 이 Finish 상태이고, 반복재생을 하기로 한 경우
	if (m_pCurAnim->m_bFinish && m_bRepeat)
	{	
		// 다시 0 프레임으로 리셋시켜서 동작하게 한다.
		m_pCurAnim->Reset();
	}
	
	// Animation 업데이트
	m_pCurAnim->finaltick();
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 Lefttop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
	// 텍스쳐 없으면 경고
	assert(_AtlasTex.Get());

	// 이미 만들어진게 있을 경우 경고
	CAnimation2D* pAnim = FindAnimation(_strKey);
	assert(!pAnim);

	// 텍스쳐가 있고 이미 만들어진게 없을 경우 제작
	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, Lefttop, _vSlice, _fStep, _iMaxFrm, _FPS);

	// 애니메이션의 주인을 설정
	pAnim->m_pOwner = this;
	// 만들어진 애니메이션 등록
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

CAnimation2D* CAnimator2D::FindAnimation(const wstring& _strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if(iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::Play(const wstring& _strKey, bool _bRepeat)
{
	CAnimation2D* pAnim = FindAnimation(_strKey);

	if (IsVaild(pAnim))
	{
		m_pCurAnim = pAnim;
	}

	//반복재생 여부 판단
	m_bRepeat = _bRepeat;

	// 지정된 Animation을 초기화
	m_pCurAnim->Reset();
}

void CAnimator2D::UpdateDate()
{
	if (!IsVaild(m_pCurAnim))
		return;

	m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	if (!IsVaild(m_pCurAnim))
		return;

	m_pCurAnim->Clear();
}


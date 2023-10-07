#include "pch.h"
#include "CTargetScript.h"

#include <Engine/CFontMgr.h>

CTargetScript::CTargetScript()
	:CScript(TARGETSCRIPT)
	, m_b(false)
{
}

CTargetScript::~CTargetScript()
{
}


void CTargetScript::begin()
{
	if (Animator2D() != nullptr)
	{
		Animator2D()->Play(L"target", true);
	}
}

void CTargetScript::tick()
{
	if(m_b)
	CFontMgr::GetInst()->SetDrawFont(L"  감사합니다", 700.f, 400.f, 10, FONT_RGBA(255, 255, 255, 255));
}

void CTargetScript::BeginOverlap(CCollider2D* _pOther)
{
	int a = 1;
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\smokeparticle.png"));
	MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_3, &a);
	m_b = true;
}

void CTargetScript::Overlap(CCollider2D* _pOther)
{
}

void CTargetScript::EndOverlap(CCollider2D* _pOther)
{
}




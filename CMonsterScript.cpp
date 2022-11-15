#include "pch.h"
#include "CMonsterScript.h"
#include "CGameObject.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"



CMonsterScript::CMonsterScript()
	: m_pPlayer(nullptr)
	, m_vDir()
	, m_vPlayerDir()
	, m_bDead(false)
	, m_bColliderState(true)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer = pLevel->GetLayer(0);
	vector<CGameObject*> i_pObject = pLayer->GetParentObjects();
	for (int i = 0; i < i_pObject.size(); ++i)
	{
		if (L"Player" == i_pObject[i]->GetName())
		{
			m_pPlayer = i_pObject[i];
		}
	}
	m_vDir = m_pPlayer->Transform()->GetRelativePos() - Transform()->GetRelativePos();
	m_vDir.Normalize();
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Smoke"));
}

void CMonsterScript::tick()
{
	
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += m_vPlayerDir.x * 800 * DT;
	vPos.y += m_vPlayerDir.y * 800 * DT;
	//vPos.z += 30.f * DT;
	Transform()->SetRelativePos(vPos);



	//if(m_pPlayer->Transform()->GetRelativePos().x + 3000 < Transform()->GetRelativePos().x)
	
	Vec3 a = Transform()->GetRelativePos();
	if (Transform()->GetRelativePos().x > 4000.f || Transform()->GetRelativePos().x < -4000.f
		|| Transform()->GetRelativePos().y > 2500.f || Transform()->GetRelativePos().y < -2500.f)		
	{
		if (!m_bDead)
		{
			GetOwner()->Destroy();
			m_bDead = true;
		}
		
		Vec3 a = Transform()->GetRelativePos();
	}
}




void CMonsterScript::BeginOverlap(CCollider2D* _pOther)
{/*
	if (L"Player" == _pOther->GetOwner()->GetName())
	{
		Vec3 Dir = _pOther->Transform()->GetRelativePos() - Transform()->GetRelativePos();

		Dir = Dir.Normalize();
		Vec3 PlayerPos = _pOther->Transform()->GetRelativePos();

		PlayerPos -= Dir;
		_pOther->Transform()->SetRelativePos(PlayerPos);
	}*/
	Defend(_pOther);

}

void CMonsterScript::Overlap(CCollider2D* _pOther)
{
	
	Defend(_pOther);
}

void CMonsterScript::EndOverlap(CCollider2D* _pOther)
{
	m_bColliderState = true;
}



void CMonsterScript::Defend(CCollider2D* _pOther)
{

	if (L"Player" == _pOther->GetOwner()->GetName())
	{
		// 뭐지이건
		float fLen = (float)fabs(_pOther->Transform()->GetRelativePos().y - Transform()->GetRelativePos().y);
		float fScale = (_pOther->Transform()->GetRelativeScale().y / 2.f) + (Transform()->GetRelativeScale().y / 2.f);
		// 플레이어위치
		Vec3 PlayerPos = _pOther->Transform()->GetRelativePos();


		// 타일의 y값을 구하기
		float if1 = (Transform()->GetRelativePos().y - (Transform()->GetWorldScale().y / 2)) ;
		float if2 = (Transform()->GetRelativePos().y - (Transform()->GetWorldScale().y / 2)) + Transform()->GetRelativeScale().y;

		// 플레이어 
		float PlayerPosA = PlayerPos.y + _pOther->Transform()->GetWorldScale().y / 2 - 5.f;
		float PlayerPosB = PlayerPos.y - _pOther->Transform()->GetWorldScale().y / 2 + 5.f;

		// 밀리는 값
		float bouns = 1.f;
		if (m_bColliderState)
		{
			// 타일의 y와 플레이어의 y을 비교해 안에 들어가 있으면 y값 코드 진행
			if (PlayerPosA >= (if1) && PlayerPosB <= (if2))
			{
				fLen = (float)fabs(_pOther->Transform()->GetRelativePos().x - Transform()->GetRelativePos().x);
				fScale = (_pOther->Transform()->GetWorldScale().x / 2.f + (Transform()->GetWorldScale().x / 2.f));

				// y축에서 접근한다면 왼쪽인지 오른쪽인지 판단
				if ((_pOther->Transform()->GetRelativePos().x < Transform()->GetRelativePos().x))
				{
					if (fLen < fScale)
					{
						Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
						vPlayerPos.x -= (fScale - fLen) + bouns;

						_pOther->Transform()->SetRelativePos(vPlayerPos);
						return;
					}
				}
				else
				{
					if (fLen < fScale)
					{
						Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
						vPlayerPos.x += (fScale - fLen) + bouns;
						_pOther->Transform()->SetRelativePos(vPlayerPos);
						return;
					}
				}
			}
		}

		// x축에서 접근한다면 위쪽인지 아닌지 판단
		if ((_pOther->Transform()->GetRelativePos().y < Transform()->GetRelativePos().y))
		{
			m_bColliderState = false;
			if (fLen < fScale)
			{
				Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
				vPlayerPos.y -= (fScale - fLen) + bouns;
				_pOther->Transform()->SetRelativePos(vPlayerPos);
				return;
			}
		}
		else
		{
			if (fLen < fScale)
			{
				Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
				vPlayerPos.y += (fScale - fLen) + bouns;
				_pOther->Transform()->SetRelativePos(vPlayerPos);
				return;
			}
		}
	}
}

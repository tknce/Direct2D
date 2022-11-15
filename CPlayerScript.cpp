#include "pch.h"
#include "CPlayerScript.h"



CPlayerScript::CPlayerScript()
	:playerinfo()
{
	playerinfo.HP = 10;
	playerinfo.Speed = 100.f;
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	//MeshRender()->GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Smoke"));
	int a = 0;
}

void CPlayerScript::tick()													  
{																			  
	Vec3 vPos = Transform()->GetRelativePos();								  
																			  
	if (KEY_PRESSED(KEY::LEFT))												  
	{																		  
		vPos.x -= playerinfo.Speed * DT;									  
	}																		  
																			  
	if (KEY_PRESSED(KEY::RIGHT))											  
	{																		  
		vPos.x += playerinfo.Speed * DT;									  
	}																		  
																			  
	if (KEY_PRESSED(KEY::UP))												  
	{																		  
		vPos.y += playerinfo.Speed * DT;									  
	}																		  

	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= playerinfo.Speed * DT;
	}

	if (KEY_PRESSED(KEY::J))												  
	{																		  
		vPos.y -= 1 * DT;									  
	}																		  
	if (KEY_PRESSED(KEY::H))
	{
		vPos.x -= 1 * DT;
	}

	if (KEY_PRESSED(KEY::K))
	{
		vPos.x +=  1 *DT;
	}

	if (KEY_PRESSED(KEY::U))
	{
		vPos.y +=  1 *DT;
	}


																			  
	if (KEY_PRESSED(KEY::Y))												  
	{																		  
		Vec3 vRot = Transform()->GetRelativeRotation();						  
		vRot.z += DT * XM_PI;												  
		Transform()->SetRelativeRotation(vRot);								  
																			  
		float fZRot = Collider2D()->GetRotationZ();							  
		fZRot += DT * XM_PI;												  
		Collider2D()->SetRotationZ(fZRot);									  
	}

	if (vPos != Transform()->GetRelativePos())
		Transform()->SetRelativePos(vPos);


	int a = 0;
	if (KEY_TAP(KEY::Z))
	{
		a = 0;
		if (nullptr != MeshRender() && nullptr != MeshRender()->GetCurMaterial())
		{
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
		}
	}

	if (KEY_TAP(KEY::X))
	{
		a = 1;
		if (nullptr != MeshRender() && nullptr != MeshRender()->GetCurMaterial())
		{
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_0, &a);
		}
	}
	/*if (KEY_TAP(KEY::U))
	{*/
		
	
	

	if (KEY_TAP(KEY::SPACE))
	{
		// 플레이어 위치에 미사일 생성
		Vec3 vMissilePos = Transform()->GetRelativePos();
		vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;

		Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
		Instantiate(pMissilePrefab->Instantiate(), vMissilePos);
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{
	if (playerinfo.HP == 0)
		//Destroy();

	--playerinfo.HP;
}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}

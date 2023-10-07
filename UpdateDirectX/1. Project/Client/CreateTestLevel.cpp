#include "pch.h"
#include "CreateTestLevel.h"

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CGameObject.h>
#include <Engine\GlobalComponent.h>

#include <Engine\CPaintShader.h>

#include <Script\CPlayerScript.h>
#include "CSaveLoadMgr.h"
#include "CCameraScript.h"

//#include "CSaveLoadMgr.h"

void CreateTestLevel()
{/*
	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 1);
	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 2);*/



	CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\Menu.lv");

	CLevelMgr::GetInst()->ChangeLevel(Level);
	return;

	// Level 하나 제작하기
	CLevel* pLevel = new CLevel;

	// Layer 이름 설정
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(3)->SetName(L"Monster");
	pLevel->GetLayer(4)->SetName(L"MonsterProjectile");





	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	

	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	pLevel->AddGameObject(pCamObj, 0);


	// Directional Light 추가
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pDirLight->Light2D()->SetLightEmbient(Vec3(0.2f, 0.2f, 0.2f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	pLevel->AddGameObject(pDirLight, 0);


	// PointLight 추가
	//CGameObject* pPointLight = new CGameObject;
	//pPointLight->SetName(L"PointLight");

	//pPointLight->AddComponent(new CTransform);
	//pPointLight->AddComponent(new CLight2D);

	//pPointLight->Transform()->SetRelativePos(0.f, 0.f, 0.f);

	//pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pPointLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	//pPointLight->Light2D()->SetRadius(500.f);
	//pPointLight->Light2D()->SetAngle(XM_PI / 3);


	//pLevel->AddGameObject(pPointLight, 0);


	// GameObject 초기화
	/*CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CPlayerScript);
	pObject->AddComponent(new CRigidBody2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(1024.f, 1024.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Walk", CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png"), Vec2(0.f, 350.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	
	pObject->Animator2D()->Play(L"LeftWalk", true);

	int a = 0;
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\smokeparticle.png"));

	pLevel->AddGameObject(pObject, 0);*/


	//CGameObject* pChild = new CGameObject;
	//pChild->SetName(L"Child");

	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CMeshRender);
	//pChild->AddComponent(new CCollider2D);

	//pChild->Transform()->SetIgnoreParentScale(true);
	//pChild->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pChild->Transform()->SetRelativeScale(Vec3(2000.f, 2000.f, 1.f));

	//pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pChild->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Plane"));




	//pLevel->AddGameObject(pChild, 1);



	// Particle Object
	//CGameObject* pParticle = new CGameObject;
	//pParticle->SetName(L"particle");
	//pParticle->AddComponent(new CTransform);
	//pParticle->AddComponent(new CParticleSystem);

	//pParticle->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pParticle->ParticleSystem()->SetWorldSpawn(true);

	//pLevel->AddGameObject(pParticle, 0);


	// PostProcess Object
	//CGameObject* pPostProcess = new CGameObject;
	//pPostProcess->SetName(L"postprocess");
	//pPostProcess->AddComponent(new CTransform);
	//pPostProcess->AddComponent(new CMeshRender);
	//

	//pPostProcess->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	//pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	//pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	//pLevel->AddGameObject(pPostProcess, 0);




	//pObject = new CGameObject;
	//pObject->SetName(L"Monster");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CCollider2D);


	//pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	//pObject->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 0.f));

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	//pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);

	//a = 0;
	//vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	//pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	//pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	//pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Character"));

	//pLevel->AddGameObject(pObject, 0);


	// TileMap Object
	//CGameObject* pTileMapObj = new CGameObject;
	//pTileMapObj->SetName(L"TileMap");
	//pTileMapObj->AddComponent(new CTransform);
	//pTileMapObj->AddComponent(new CTileMap);

	//pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1000.f));
	//pTileMapObj->Transform()->SetRelativeScale(Vec3(1000.f, 800.f, 1.f));

	//pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"texture\\TILE.bmp"));
	//pTileMapObj->TileMap()->SetTileCount(10, 10);

	//pLevel->AddGameObject(pTileMapObj, 0);

	// ComputeShader 테스트
	//Ptr<CPaintShader> pComputeShader = (CPaintShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PaintShader").Get();
	//pComputeShader->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));
	//pComputeShader->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	//pComputeShader->Execute();

	// 충돌 레이어 설정
	// 충돌 레이어 설정
	


	// Level 저장
	//CSaveLoadMgr::GetInst()->SaveLevel(pLevel, L"level\\Test.lv");



	// 레벨 설정
	CLevelMgr::GetInst()->ChangeLevel(pLevel);
}

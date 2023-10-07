#include "pch.h"
#include "CCamera.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CRenderComponent.h"
#include "CMaterial.h"

#include "CGameObject.h"


CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType(PROJ_TYPE::PERSPECTIVE)
	, m_fAspectRatio(1.f)
	, m_fFar(100000.f)
	, m_fScale(1.f)
	, m_iLayerMask(0)
	, m_iCamIdx(0)
{
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResolution.x / vRenderResolution.y;
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
	CalcViewMat();

	CalcProjMat();

	if (m_pTarget != nullptr)
	{
		Vec3 Pos = m_pTarget->Transform()->GetRelativePos();
		Vec3 MousePos = Vec3(CKeyMgr::GetInst()->GetCameraMousePos().x, CKeyMgr::GetInst()->GetCameraMousePos().y, 0.f);
		Vec3 CameraPos = Transform()->GetRelativePos();

		Vec3 Dir = Pos - MousePos;
		Dir.Normalize();
		
		CameraPos = Pos;
		CameraPos -= Dir * 80;
		if ((Pos + 200) < CameraPos)
		{
			Dir = Vec3(0, 0, 0);
			CameraPos = Pos;
		}		
		if ((Dir + -200) > CameraPos)
		{
			Dir = Vec3(0, 0, 0);
			CameraPos = Pos;
		}			
		Transform()->SetRelativePos(CameraPos);
	}

	// 카메라 등록
	CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::render()
{
	// 행렬 세팅
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	// Shader Domain 에 따른 물체 분류
	SortObject();

	// Domain 분류에 따른 렌더링
	render_opaque();
	render_mask();
	render_transparent();
	render_postprocess();
}


void CCamera::SetLayerMask(const wstring& _strLayerName)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);
	assert(pLayer);

	SetLayerMask(pLayer->GetLayerIdx());
}

void CCamera::SetLayerMask(int _iLayerIdx)
{
	if (m_iLayerMask & (1 << _iLayerIdx))
	{
		m_iLayerMask &= ~(1 << _iLayerIdx);
	}
	else
	{
		m_iLayerMask |= (1 << _iLayerIdx);
	}
}

void CCamera::CalcViewMat()
{

		// =============
		// View 행렬 계산
		// =============
		// View 이동행렬 ( 카메라를 원점으로 이동하는 만큼 )
		Vec3 vPos = Transform()->GetRelativePos();
		Matrix matViewTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

		// View 회전 행렬 ( 카메라가 보는 전방 방향을 z 축을 보도록 돌리는 만큼 )
		Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);
		Vec3 vUp = Transform()->GetRelativeDir(DIR::UP);
		Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);

		Matrix matViewRot = XMMatrixIdentity();
		matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
		matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
		matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

		m_matView = matViewTrans * matViewRot;
	
}

void CCamera::CalcProjMat()
{
	// ==============
	// 투영 행렬 계산
	// ==============
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if (PERSPECTIVE == m_eProjType)
	{
		// 원근투영
		m_matProj = XMMatrixPerspectiveFovLH(XM_2PI / 6.f, m_fAspectRatio, 1.f, m_fFar);
	}
	else
	{
		// 직교투영
		m_matProj = XMMatrixOrthographicLH(vRenderResolution.x * m_fScale, vRenderResolution.y * m_fScale, 1.f, m_fFar);
	}
}

void CCamera::SortObject()
{
	m_vecQpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();
	m_vecPostProcess.clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer 확인
		if (m_iLayerMask & (1 << i))
		{
			// 해당 레이어에 속한 게임오브젝트를 가져온다.
			CLayer* pLayer = pCurLevel->GetLayer(i);


			// 오브젝트들을 쉐이더 도메인에 따라 분류한다.
			const vector<CGameObject*>& vecObj = pLayer->GetObjects();
			for (size_t j = 0; j < vecObj.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();

				if (nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMesh()
					|| nullptr == pRenderCom->GetCurMaterial()
					|| nullptr == pRenderCom->GetCurMaterial()->GetShader())
				{
					continue;
				}

				Ptr<CGraphicsShader> pShader = pRenderCom->GetCurMaterial()->GetShader();

				SHADER_DOMAIN eDomain = pShader->GetDomain();

				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecQpaque.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POST_PROCESS:
					m_vecPostProcess.push_back(vecObj[j]);
					break;
				}
			}
		}
	}
}

void CCamera::render_opaque()
{
	for (size_t i = 0; i < m_vecQpaque.size(); ++i)
	{
		m_vecQpaque[i]->render();
	}
}

void CCamera::render_mask()
{
	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->render();
	}
}

void CCamera::render_transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPostProcess[i]->render();
	}
}

void CCamera::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_eProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	fwrite(&m_fFar, sizeof(float), 1, _File);
	fwrite(&m_fScale, sizeof(float), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_eProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fFar, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
}

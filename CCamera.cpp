#include "pch.h"
#include "CCamera.h"

#include "CLevel.h"
#include "CLevelMgr.h"


#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CRenderComponent.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType(PROJ_TYPE::PERSPECTIVE)
	, m_fAspectRatio(1.f)
	, m_fFar(100000.f)
	, m_fScale(1.f)
	, m_iLayerMask(0)
{
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResolution.x / vRenderResolution.y;
}

CCamera::~CCamera()
{
}


void CCamera::finaltick()
{
	// =============
	// View 행렬 계산
	// =============
	// View 이동행렬
	Vec3 vPos = Transform()->GetRelativePos();
	// 포즈행렬 계산
	Matrix m_matViewTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	// View 회전 행렬
	Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);
	Vec3 vUp	= Transform()->GetRelativeDir(DIR::UP);
	Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
	// 회전값들을 넣을 때 결국 단위행렬화 해야되는데 그것들이 1행 2행 3행들의 값들이 결국
	// 1열 2열 3열로 들어가서 밑의 수식처럼 넣으면 된다.
	// 단위행렬화 하고 회전값 넣음
	Matrix matViewRot = XMMatrixIdentity();
	matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

	// 행렬 누적
	m_matView = m_matViewTrans * matViewRot;


	// =============
	// 투영 행렬 계산
	// =============

	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if (PERSPECTIVE == m_eProjType)
	{
		// 원근투영
		m_matProj = XMMatrixPerspectiveFovLH(	XM_2PI / 6.f		// 60도
											,	m_fAspectRatio		// 종횡비율 - 해상도가로세로 나눈 값
											,	1.f					// 제일 얇은 깊이 1
											,	m_fFar);			// 제일 깊은 깊이 1000
	}
	else
	{
		// 직교투영
		m_matProj = XMMatrixOrthographicLH(vRenderResolution.x * m_fScale	// 월드 상의 카메라가 담을 수 있는 x크기
										, vRenderResolution.y * m_fScale	// 월드 상의 카메라가 담을 수 있는 y크기
										, 1.f								// 제일 얇은 깊이 1
										, m_fFar);							// 제일 깊은 깊이 1000
	}

	//// 행렬 세팅
	//g_transform.matView = m_matView;
	//g_transform.matProj = m_matProj;

	// 카메라 등록
	// 카메라가 항상 지워지고 다시 등록
	CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::render()
{
	// 행렬 세팅
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;		// 설정한 벡터를 다른 행렬로 투영시켜주는 계산

	// Shader Domain 에 따른 물체 분류
	SortObject();

	// Domain 분류에 따른 렌더링
	render_opaque();
	render_mask();
	render_transparent();

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

void CCamera::SortObject()
{
	m_vecQpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer 확인
		if (m_iLayerMask & (1 << i))
		{
			// 해당 레이어를 속한 게임 오브젝트를 가져온다.
			CLayer* pLayer = pCurLevel->GetLayer(i);

			// 오브젝트들을 쉐이더 도메인에 따라 분류한다.
			const vector<CGameObject*>& vecObj = pLayer->GetObjects();
			for (size_t j = 0; j < vecObj.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();
				
				// 이것들이 없으면 그려질 필요가 없기에 제외한다.
				if (nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMesh()
					|| nullptr == pRenderCom->GetCurMaterial()
					|| nullptr == pRenderCom->GetCurMaterial()->GetShader())
				{
					continue;
				}
				// 랜더컴포넌트에서 재질 속 쉐이더를 얻는다.
				Ptr<CGraphicsShader> pShader = pRenderCom->GetCurMaterial()->GetShader();
				
				// 얻은 쉐이더에서 도메인 설정값을 얻는다.
				SHADER_DOMAIN eDomain = pShader->GetDomain();

				switch (eDomain)
				{
					// 불투명단계
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecQpaque.push_back(vecObj[j]);
					break;
					// 출력, 비출력
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObj[j]);
					break;
					// 투명단계
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObj[j]);
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

#include "pch.h"
#include "CMeshRender.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CAnimator2D.h"

#include "CMesh.h"
#include "CMaterial.h"


CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)	
{
}

CMeshRender::~CMeshRender()
{
}


void CMeshRender::tick()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{
	// Mesh가 죽어있다면 리턴
	if (!IsActive())
		return;

	// Mesh나 현재재질이 없다면 그려질 이유가 없다.
	if (nullptr == GetMesh() || nullptr == GetCurMaterial())
		return;
		
	// 위치정보 업데이트
	// 행렬정보 계산한것을 매 틱마다 계산
	Transform()->UpdateData();

	// 사용할 재질 업데이트
	// 현재 재질을 업데이트 동적재질로 바뀔가능성이 있기때문에
	GetCurMaterial()->UpdateData();

	// 애니메이션 컴포넌트가 있다면 애니메이션으로 랜더
	if (Animator2D())
	{
		Animator2D()->UpdateDate();
	}


	// 사용할 메쉬 업데이트 및 렌더링
	GetMesh()->render();

	// 사용한 텍스쳐 레지스터 비우기
	CMaterial::Clear();

	// Animator2D 컴포넌트가 있었으면
	if (Animator2D())
		Animator2D()->Clear();
}
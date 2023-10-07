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
	if (!IsActive())
		return;

	if (nullptr == GetMesh() || nullptr == GetCurMaterial())
		return;
		
	// ��ġ���� ������Ʈ
	Transform()->UpdateData();
		
	// ����� ���� ������Ʈ
	GetCurMaterial()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// ����� �޽� ������Ʈ �� ������
	GetMesh()->render();

	// ����� �ؽ��� �������� ����
	CMaterial::Clear();

	// Animator2D ������Ʈ�� �־�����, ���� �������� Clear
	if (Animator2D())
		Animator2D()->Clear();
}
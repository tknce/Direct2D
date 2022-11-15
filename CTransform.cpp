#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bIgnParentScale(false)
{
	m_vRelativeDir[(UINT)DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_vRelativeDir[(UINT)DIR::UP]	 = Vec3(0.f, 1.f, 0.f);
	m_vRelativeDir[(UINT)DIR::FRONT] = Vec3(0.f, 0.f, 1.f);
}

CTransform::~CTransform()
{
}

void CTransform::tick()
{

}

void CTransform::finaltick()
{
	// m_vRelativePos, m_vRelativeScale, m_vRelativeRotation
	// ㄴ=> 월드행렬을 만들어 냄
	m_matWorld = XMMatrixIdentity(); // 단위행렬 - 단위행렬이란? Vector가 행렬에 곱했을때 Vector값이 그대로 나오는 것이다
									// 1 0 0 0
									// 0 1 0 0
									// 0 0 1 0
									// 0 0 0 1

	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z); 
	// 행렬에 크기값을 곱한다 크기값은
									// x 0 0 0 이것중 xyz의 영향받는 부분이다.
									// 0 y 0 0
									// 0 0 z 0
									// 0 0 0 0

	Matrix	matRot =  XMMatrixRotationX(m_vRelativeRotation.x); // x축 회전 값을 곱한다
			matRot *= XMMatrixRotationY(m_vRelativeRotation.y); // y축 회전 값을 곱한다
			matRot *= XMMatrixRotationZ(m_vRelativeRotation.z); // z축 회전 값을 곱한다
			// 회전행렬 값은
									// x y z 0 이것중 x,y,z이다.
									// x y z 0
									// x y z 0
									// 0 0 0 0

	Matrix matTrans = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z); // 이동값을 행렬에 곱한다.
			// 이동행렬 값은
									// 0 0 0 0 이것중 x,y,z 이다.
									// 0 0 0 0
									// 0 0 0 0
									// x y z 1

			// transform
			// 회전개념
			// 곱 순서 크기 -> 회전 -> 이동
			// 
			// 회전행렬
			// cos알파 *x - sin알파*x sin 알파 y + cos 알파 y
			// z축 회전 행렬
			// x y z		cos알파		sin알파		0			0
			//				-sin알파	cos알파		0			0
			//				0			0			1			0
			//				0			0			0			1
			// 
			// x축 회전 행렬
			// x y z		1			0			0			0
			//				0			cos알파		sin알파		0		
			//				0			-sin알파	cos알파		0	
			//				0			0			0			1
			// 
			// y축 회전 행렬
			// xyz			cos알파		0			-sin알파	0
			//				0			1			0			0
			//				sin알파		0			cos알파		0
			//				0			0			0			1
			// 왼손좌표계를 이용하기 때문에 - sin알파를 한다. 
			// 왜냐하면 전부 반시계방향으로 도는데 sin 알파를 해버리면 y축만 시계방향으로 돈다.
			// 즉 회전 시킬경우 정점들이 이동하기 때문에 결국엔 반시계방향이 결과값으로 나오기 때문에 back이 되어 픽셀쉐이더로 가지 못하고 리턴된다.
			// 
			// 외적을 해서 수직한 방향을 구하고 쳐다본 방향과 보는 방향을 내적해서 판단한다.
	



	// 회전행렬을 이용해서 현재 물체의 우, 상, 전 방향을 구해놓는다.
	// 회전행렬의 영향을 받은 scale값이다.
	m_vRelativeDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), matRot);	
	m_vRelativeDir[(UINT)DIR::UP] =	   XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), matRot);
	m_vRelativeDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), matRot);

	// 꼭 이 순서대로 곱해야된다.
	m_matWorld = matScale * matRot * matTrans;

	// 부모 오브젝트가 있다면
	if (GetOwner()->GetParent())
	{
		// 부모 오브젝트의 크기를 무시하기로 한 경우
		if (m_bIgnParentScale)
		{
			// 위쪽으로 모든 부모의 크기값을 누적해서 역행렬을 만들어 둔다.
			CGameObject* pParent = GetOwner()->GetParent();
			Vec3 vParentScale = Vec3(1.f,1.f,1.f);

			while (pParent)
			{
				vParentScale *= pParent->Transform()->GetRelativeScale();

				// 부모 오브젝트도 그 위로 크기를 무시하기로 한 경우 크기를 더이상 누적할 필요가 없다.
				if (pParent->Transform()->m_bIgnParentScale)
					pParent = nullptr;
				else
					pParent = pParent->GetParent();
			}

			// 부모 행렬의 크기부분을 역행렬을 구해서 상쇄시킨다.
			// 역행렬 예외처리
			if (0.f == vParentScale.x)
				vParentScale.x = 1.f;
			if (0.f == vParentScale.y)
				vParentScale.y = 1.f;
			if (0.f == vParentScale.z)
				vParentScale.z = 1.f;

			Matrix maxParentScaleInv = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			maxParentScaleInv = XMMatrixInverse(nullptr, maxParentScaleInv);

			// 구한 역행렬을 미리 곱해두고 부모의 월드행렬을 곱한다.
			m_matWorld = m_matWorld * maxParentScaleInv * GetOwner()->GetParent()->Transform()->GetWorldMat();
		}
		else
		{
			m_matWorld *= GetOwner()->GetParent()->Transform()->GetWorldMat();
		}

		// WorldDir 구하기
		// XMVector3TransformNormal - 동차좌표를 0으로 해서 리턴한다. (주로 계산값만 받아올때)
		// XMVector3TransformCoord  - 동차좌표를 1으로 해서 리턴한다. (주로 값을 적용시킬때) 
		m_vWorldDir[(UINT)DIR::RIGHT]	= XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::UP]		= XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::FRONT]	= XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), m_matWorld);

		// 회전, 크기 변환이 이루어졌기 때문에 변경된 크기를 초기화 하기 위해 Normalize를 해준다.
		m_vWorldDir[(UINT)DIR::RIGHT].Normalize();
		m_vWorldDir[(UINT)DIR::UP].Normalize();
		m_vWorldDir[(UINT)DIR::FRONT].Normalize();
	}
}

void CTransform::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
		
	// 투영행렬까지 미리 계산해서 파이프라인에 넘긴다.
	g_transform.matWorld = m_matWorld;	
	g_transform.matWV = g_transform.matWorld * g_transform.matView;	// 월드포지션과 카메라뷰행렬(view는 카메라가 좌표0과 z축을 바라보는 형태)를 미리 곱한다.
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;	// WorldView와 Proj투영행렬을 곱한다   Proj은 직교투영인지 원근투영인지에 대한 방식

	pCB->SetData(&g_transform);
	pCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
}

Vec3 CTransform::GetWorldScale()
{
	// 위쪽으로 모든 부모의 크기값을 누적해서 역행렬을 만들어 둔다.
	Vec3 vWorldScale = m_vRelativeScale;

	if (m_bIgnParentScale)
		return vWorldScale;

	CGameObject* pParent = GetOwner()->GetParent();
	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		// 부모 오브젝트도 그 위로 크기를 무시하기로 한 경우 크기를 더 이상 누적할 필요가 없다.
		// 부모의 크기에 영향을 받음 효과 부모가 크기가 줄어들거나 늘어날때 자식도 줄어들거나 늘어남
		// 그것을 끄는 기능도 있으니 자식 고유의 스케일로 지정할 수도 있음
		if (pParent->Transform()->m_bIgnParentScale)
			pParent = nullptr;
		else
		{
			pParent = pParent->GetParent();
		}
	}

	return vWorldScale;
}

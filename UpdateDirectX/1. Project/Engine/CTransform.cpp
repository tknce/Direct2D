#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bIgnParentScale(false)
{
	m_vRelativeDir[(UINT)DIR::RIGHT]	= Vec3(1.f, 0.f, 0.f);
	m_vRelativeDir[(UINT)DIR::UP]		= Vec3(0.f, 1.f, 0.f);
	m_vRelativeDir[(UINT)DIR::FRONT]	= Vec3(0.f, 0.f, 1.f);
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
	// ��=> ��������� ����� ��
	m_matWorld = XMMatrixIdentity();

	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matTrans = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	Matrix matRot = XMMatrixRotationX(m_vRelativeRotation.x);
	matRot *= XMMatrixRotationY(m_vRelativeRotation.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRotation.z);

	// ȸ������� �̿��ؼ� ���� ��ü�� ��, ��, �� ������ ���س��´�.
	m_vRelativeDir[(UINT)DIR::RIGHT] = m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), matRot);
	m_vRelativeDir[(UINT)DIR::UP]	 = m_vWorldDir[(UINT)DIR::UP]	 = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), matRot);
	m_vRelativeDir[(UINT)DIR::FRONT] = m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), matRot);

	m_matWorld = matScale * matRot * matTrans;

	// �θ� ������Ʈ�� �ִٸ�
	if (GetOwner()->GetParent())
	{
		// �θ� ������Ʈ�� ũ�⸦ �����ϱ�� �� ���
		if (m_bIgnParentScale)
		{
			// �������� ��� �θ��� ũ�Ⱚ�� �����ؼ� ������� ����� �д�.
			CGameObject* pParent = GetOwner()->GetParent();
			Vec3 vParentScale = Vec3(1.f, 1.f, 1.f);

			while (pParent)
			{
				vParentScale *= pParent->Transform()->GetRelativeScale();

				// �θ� ������Ʈ�� �� ���� ũ�⸦ �����ϱ�� �� ��� ũ�⸦ ���̻� ������ �ʿ䰡 ����.
				if (pParent->Transform()->m_bIgnParentScale)
					pParent = nullptr;
				else			
					pParent = pParent->GetParent();
			}

			// �θ� ����� ũ��κ��� ������� ���ؼ� ����Ų��.
			// ����� ����ó��
			if (0.f == vParentScale.x)
				vParentScale.x = 1.f;
			if (0.f == vParentScale.y)
				vParentScale.y = 1.f;
			if (0.f == vParentScale.z)
				vParentScale.z = 1.f;

			Matrix matParentScaleInv = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			matParentScaleInv = XMMatrixInverse(nullptr, matParentScaleInv);

			// ���� ������� �̸� ���صΰ� �θ��� ��������� ���Ѵ�.
			m_matWorld = m_matWorld * matParentScaleInv * GetOwner()->GetParent()->Transform()->GetWorldMat();			
		}
		else
		{
			m_matWorld *= GetOwner()->GetParent()->Transform()->GetWorldMat();
		}	

		// WorldDir ���ϱ�
		m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::UP]	  = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), m_matWorld);

		// ȸ��, ũ�� ��ȯ�� �̷������ ������ ����� ũ�⸦ �ʱ�ȭ �ϱ� ���� Normalize ���ش�.����
		m_vWorldDir[(UINT)DIR::RIGHT].Normalize();
		m_vWorldDir[(UINT)DIR::UP].Normalize();
		m_vWorldDir[(UINT)DIR::FRONT].Normalize();
	}
}


void CTransform::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
		
	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;
	
	pCB->SetData(&g_transform);
	pCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
}

Vec3 CTransform::GetWorldScale()
{
	// �������� ��� �θ��� ũ�Ⱚ�� �����ؼ� ������� ����� �д�.	
	Vec3 vWorldScale = m_vRelativeScale;

	if (m_bIgnParentScale)
		return vWorldScale;

	CGameObject* pParent = GetOwner()->GetParent();
	while (pParent)
	{		
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		// �θ� ������Ʈ�� �� ���� ũ�⸦ �����ϱ�� �� ��� ũ�⸦ ���̻� ������ �ʿ䰡 ����.
		if (pParent->Transform()->m_bIgnParentScale)
			pParent = nullptr;
		else
			pParent = pParent->GetParent();
	}

	return vWorldScale;
}

void CTransform::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE type = GetType();
	fwrite(&type, sizeof(UINT), 1, _File);

	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_bIgnParentScale, sizeof(bool), 1, _File);

}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_vRelativePos,sizeof(Vec3),1,_File);
	fread(&m_vRelativeScale,sizeof(Vec3),1,_File);
	fread(&m_vRelativeRotation,sizeof(Vec3),1,_File);
	fread(&m_bIgnParentScale, sizeof(bool), 1, _File);
}

void CTransform::SaveTurnBackTime(FILE* _File)
{
	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
}

void CTransform::LoadTurnBackTime(FILE* _File)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeRotation, sizeof(Vec3), 1, _File);

}

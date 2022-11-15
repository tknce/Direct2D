#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"


CCollisionMgr::CCollisionMgr()
	: m_matrix{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (int iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (int iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (m_matrix[iRow] & (1 << iCol))
			{
				CollisionBtwLayer(pCurLevel, iRow, iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionBtwLayer(CLevel* _pCurLevel, int _iLeft, int _iRight)
{
	const vector<CGameObject*>& vecLeft = _pCurLevel->GetLayer(_iLeft)->GetObjects();
	const vector<CGameObject*>& vecRight = _pCurLevel->GetLayer(_iRight)->GetObjects();

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (nullptr == vecLeft[i]->Collider2D())
			continue;

		size_t j = 0;
		if (_iLeft == _iRight)
			j = i + 1;
		
		for (; j < vecRight.size(); ++j)
		{
			if (nullptr == vecRight[j]->Collider2D())
				continue;

			CollisionBtwCollider(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D());
			
		}
	}
}

void CCollisionMgr::CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// 두 충돌체의 ID 를 확인
	COLLIDER_ID id;
	id.LEFT_ID = _pLeft->GetID();
	id.RIGHT_ID = _pRight->GetID();

	// 이전 충돌 정보를 검색한다.
	map<ULONGLONG, bool>::iterator iter = m_mapColInfo.find(id.ID);

	// 충돌정보가 아예 없으면 만들어준다.
	if (iter == m_mapColInfo.end())
	{
		m_mapColInfo.insert(make_pair(id.ID, false));
		iter = m_mapColInfo.find(id.ID);
	}

	// 두 충돌체중 하나 이상의 Dead 상태인지
	bool IsDead = _pLeft->GetOwner()->IsDead() || _pRight->GetOwner()->IsDead();


	// 현재 충돌중인지 검사 한다.			
	if (IsCollision(_pLeft, _pRight))
	{
		// 충돌 중이다.
		if (false == iter->second)
		{
			// 이전 프레임에는 충돌하지 않았다.
			// ㄴ--> 이번 프레임에 충돌 진입
			if (!IsDead)
			{
				_pLeft->BeginOverlap(_pRight);
				_pRight->BeginOverlap(_pLeft);
				iter->second = true;
			}
		}

		// 충돌 중
		else
		{
			// 삭제 예정인 경우, 충돌을 해제하는 방향으로 진행
				if (IsDead)
			{
				_pLeft->EndOverlap(_pRight);
				_pRight->EndOverlap(_pLeft);
				iter->second = false;
			}

			// 이전에도 충돌, 지금도 충돌 중
			else
			{
			_pLeft->Overlap(_pRight);
			_pRight->Overlap(_pLeft);
			}
		}
	}

	else
	{
		// 충돌 X
		if (iter->second)
		{
			// 이전에는 충돌하고 있었다
			// ㄴ--> 충돌을 막 벗어난 시점
			_pLeft->EndOverlap(_pRight);
			_pRight->EndOverlap(_pLeft);
			iter->second = false;
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// 0 --- 1
	// |  \  |
	// 3 --- 2
	static const Vec3 arrLocalPos[4]
		=
	{
		// NDC좌표계에서 충돌체의 꼭지점 부분 투영하기
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f)
	};


	// 분리축 구하기
	Vec3 vAxis[4] = {};

	// 두 충돌체의 월드 행렬을 가져온다.
	const Matrix& matLeft = _pLeft->GetWorldMat();
	const Matrix& matRight = _pRight->GetWorldMat();

	// 분리축 벡터 == 투영벡터
	vAxis[0] = XMVector3TransformCoord(arrLocalPos[1], matLeft) - XMVector3TransformCoord(arrLocalPos[0], matLeft);
	vAxis[1] = XMVector3TransformCoord(arrLocalPos[3], matLeft) - XMVector3TransformCoord(arrLocalPos[0], matLeft);
	vAxis[2] = XMVector3TransformCoord(arrLocalPos[1], matRight) - XMVector3TransformCoord(arrLocalPos[0], matRight);
	vAxis[3] = XMVector3TransformCoord(arrLocalPos[3], matRight) - XMVector3TransformCoord(arrLocalPos[0], matRight);
	// XMVector3TransformCoord		3D벡터를 주어진 메트릭스로 변형하는 함수라고 한다. 4행 4열 을 1로 설정한 채 리턴
	// XMVector3TransformNormal		회전 및 크기 조정을 위해 입력 행력 4행 1열 2열 3열 4열에서 4열의 행을 0으로 설정하여 무시함


	for (int i = 0; i < 4; ++i)
	{
		vAxis[i].z = 0.f;
	}

	// 두 충돌체의 거리 계산
	Vec2 vC = _pLeft->GetFinalPos() - _pRight->GetFinalPos();
	Vec3 vCenterDir = Vec3(vC.x, vC.y, 0.f);

	for (int i = 0; i < 4; ++i)
	{
		// 방향 벡터 구하기
		Vec3 vA = vAxis[i];
		vA.Normalize();

		float fProjDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			// 분리축에서 투영한 벡터에서 사선일 경우 내적하여 밑변을 구하고 분리축으로 투영한 것을 더하여 두 충돌체간의 거리계산
			// 내적할댸 cos세타로 구하기 때문에 음수값이 나올 수 있다. 그래서 절대값으로 변환하여 계산한다.
			// 절대값이란? 함수에 이동한 거리 -4에서 0으로 이동할때 4가 나오는것
			fProjDist += fabsf(vAxis[j].Dot(vA)) / 2.f;
		}

		if (fProjDist < fabsf(vCenterDir.Dot(vA)))
			return false;
	}

	return true;
}

void CCollisionMgr::CollisionLayerCheck(int _iLeft, int _iRight)
{
	// 더 작은 숫자를 행(배열의 인덱스) 으로, 더 큰 숫자를 열(비트 위치) 로
	UINT iRow = 0, iCol = 0;

	if (_iLeft <= _iRight)
	{
		iRow = _iLeft;
		iCol = _iRight;
	}
	else
	{
		iRow = _iRight;
		iCol = _iLeft;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::CollisionLayerRelease(int _iLeft, int _iRight)
{
	// 더 작은 숫자를 행(배열의 인덱스) 으로, 더 큰 숫자를 열(비트 위치) 로
	UINT iRow = 0, iCol = 0;

	if (_iLeft <= _iRight)
	{
		iRow = _iLeft;
		iCol = _iRight;
	}
	else
	{
		iRow = _iRight;
		iCol = _iLeft;
	}

	m_matrix[iRow] &= ~(1 << iCol);
}


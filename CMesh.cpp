#include "pch.h"
#include "CMesh.h"

#include "CDevice.h"

CMesh::CMesh()
	: CRes(RES_TYPE::MESH)
	, m_tVBDesc{}
	, m_tIBDesc{}
	, m_iIdxCount(0)
{
}

CMesh::~CMesh()
{
}

void CMesh::render()
{
	UpdateData();								// 정점정보를 항상 업데이트 후
	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);	// 매틱마다 인덱스카운트 보내어 그리기
}

void CMesh::render_particle(UINT _iCount)
{
	UpdateData();
	CONTEXT->DrawIndexedInstanced(m_iIdxCount, _iCount, 0, 0, 0);
}

int CMesh::Create(void* _pVtxSysmem, size_t _iVtxCount, void* _pIdxSysmem, size_t _iIdxCount)
{
	HRESULT hr = S_OK;
	
	// System -> GPU	
	m_tVBDesc.ByteWidth = UINT(sizeof(Vtx) * _iVtxCount);				// 설정한 정점데이터와 그수 설정
	m_tVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;	// 사용 용도 설정
	m_tVBDesc.Usage = D3D11_USAGE_DEFAULT;								// 읽기 쓰기 설정
	m_tVBDesc.CPUAccessFlags = 0;										

	// D3D11_SUBRESOURCE_DATA 하위 리소스들을 초기화 시키는 구조체
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVtxSysmem;

	hr = DEVICE->CreateBuffer(&m_tVBDesc, &tSubData, &m_VB);

	// IndexBuffer 만들기	정점정보들에 대한 순서와 크기 몇개인지 설정하는 버퍼
	m_iIdxCount = (UINT)_iIdxCount;										// 인덱스 수 알려주기
	m_tIBDesc.ByteWidth = sizeof(UINT) * m_iIdxCount;					// 버퍼의 총 크기 알려주기
	m_tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;		// 사용 용도 설정
	m_tIBDesc.Usage = D3D11_USAGE_DEFAULT;								// 읽기 쓰기 설정
	m_tIBDesc.CPUAccessFlags = 0;

	tSubData = {};
	tSubData.pSysMem = _pIdxSysmem;

	hr = DEVICE->CreateBuffer(&m_tIBDesc, &tSubData, &m_IB);

	return hr;
}

void CMesh::UpdateData()
{
	UINT iStride = sizeof(Vtx); // 정점의 크기
	UINT iOffset = 0;			// 정점들의 오프셋 -> 우린 행렬계산후 월드행렬에서 옮기는 구조
	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset); // 매틱마다 정점정보 보내기
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);				// 매틱마다 인덱스정보 보내기
}
#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_tDesc{}
	, m_eType(SB_TYPE::SRV_ONLY)
	, m_iElementSize()
	, m_iElementCount()
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& _origin)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, void* _pInitial)
{
	m_SB = nullptr;
	m_SRV = nullptr;

	m_eType = _eType;											// 타입
	m_iElementSize = _iElementSize;								// 버퍼 사이즈
	m_iElementCount = _iElementCount;							// 몇개인지

	m_tDesc.ByteWidth = _iElementCount * _iElementSize;			// 버퍼 전체 크기
	m_tDesc.StructureByteStride = _iElementSize;				// 버퍼 요소 크기

	m_tDesc.Usage = D3D11_USAGE_DYNAMIC;						// 동적사용
	m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// 쓰기
	m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;				// Texture Register Binding
	m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;	// 구조화 버퍼 추가 플래그 설정

	HRESULT hr = S_OK;

	if (nullptr == _pInitial)
	{
		hr = DEVICE->CreateBuffer(&m_tDesc, nullptr, m_SB.GetAddressOf()); // 함수 없는 버편
	}
	else
	{
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _pInitial;

		hr = DEVICE->CreateBuffer(&m_tDesc, &tSub, m_SB.GetAddressOf()); // 함수 있는 버젼
	}

	// 버퍼 생성실패
	if (FAILED(hr))
		return E_FAIL;

	// Shader Resource 생성하기 위한 구조체 변수 설정
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	
	tSRVDesc.BufferEx.NumElements = _iElementCount;
	tSRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;


	// ShaderResourceView 설정
	if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &tSRVDesc, m_SRV.GetAddressOf())))
	{
		return E_FAIL;
	}
	return hr;
}

void CStructuredBuffer::SetData(void* _pSrc, UINT _iElementCount)
{
	// 공간이 모자라면 추가할당하면서 초기화한다.
	if (m_iElementCount < _iElementCount)
	{
		Create(m_iElementCount, _iElementCount, SB_TYPE::SRV_ONLY, _pSrc);
	}

	// 공간이 충분하다면, 데이터 전송
	else
	{
		// 파이프라인도는 도중 밖으로 나오는 데이터를 잡아주고 넣을때 쓰는 구조체
		D3D11_MAPPED_SUBRESOURCE tMapSub = {};
		
		// 나오는 데이터 받아온다
		CONTEXT->Map(m_SB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
		// 나오는 데이터를 채운다
		memcpy(tMapSub.pData, _pSrc, m_iElementSize * _iElementCount);
		// 나온 데이터를 돌려보낸다.
		CONTEXT->Unmap(m_SB.Get(), 0);
	}
}

void CStructuredBuffer::UpdateData(UINT _iRegisterNum, UINT _PipelineStage)
{
	// 파이프라인 스테이지에 구조화버퍼 t레지스터를 모든 shader상황에 전달
	if ((UINT)PIPELINE_STAGE::VS & _PipelineStage)
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::HS & _PipelineStage)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::DS & _PipelineStage)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::GS & _PipelineStage)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::PS & _PipelineStage)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::UpdateData_CS(UINT _iRegisterNum, bool _bShaderRes)
{
	m_iRecentRegisterNumRW = _iRegisterNum;

	// Computeshader에서 구조화버퍼를 만들 때 읽고 쓰기가 가능할지 읽기만 가능할지에 대한 여부를 물어본다.
	if (_bShaderRes)
	{
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}
	else
	{
		UINT i = -1;
		CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(),&i);
	}
}

void CStructuredBuffer::Clear()
{
	// 모든 쉐이더리소스 클리어
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSGetShaderResources(m_iRecentRegisterNum, 1, &pSRV);	// Vertex
	CONTEXT->HSGetShaderResources(m_iRecentRegisterNum, 1, &pSRV);	// Hull
	CONTEXT->DSGetShaderResources(m_iRecentRegisterNum, 1, &pSRV);	// Domain
	CONTEXT->GSGetShaderResources(m_iRecentRegisterNum, 1, &pSRV);	// Geometry
	CONTEXT->PSGetShaderResources(m_iRecentRegisterNum, 1, &pSRV);	// pixel

	
	CONTEXT->CSGetShaderResources(m_iRecentRegisterNumRW, 1, &pSRV);// Compute
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = -1;
	// Computeshader는 UnorderedAccessview도 초기화
	CONTEXT->CSSetUnorderedAccessViews(m_iRecentRegisterNumRW, 1, &pUAV, &i);

}


#include "pch.h"
#include "CConstBuffer.h"

#include "CDevice.h"

CConstBuffer::CConstBuffer(CB_TYPE _CBType)
	: m_tDesc{}
	, m_eCBType(_CBType)
{
}

CConstBuffer::~CConstBuffer()
{

}


int CConstBuffer::Create(size_t _iBufferSize)
{
	// 상수버퍼 생성
	// D3D11_BUFFER_DESC
	m_tDesc.ByteWidth = (UINT)_iBufferSize; // 16 의 배수로
	m_tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 버퍼가 파이프라인에 바인딩되는 방법을 식별
	// 버퍼 종류
	// Vertex
	// Index		Vertex 버퍼랑 비슷한 의미 Vertex buffer에 있는 정점 정보들을 다시 읽는 방식을 전해줌
	// Constant
	// Shader
	// Render
	// Depth

	m_tDesc.Usage = D3D11_USAGE_DYNAMIC; // 버퍼에서 읽고 쓸 것으로 예상되는 방법을 식별
	// Default		읽기 및 쓰기
	// Immutable	읽기
	// Dynamic		읽기 및 쓰기 (프레임당 한 번 이상 업데이트되는 리소스에 적합)

	m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = DEVICE->CreateBuffer(&m_tDesc, nullptr, m_CB.GetAddressOf());

	return hr;
}

void CConstBuffer::SetData(void* _pSrc, size_t _iSize) // 포인터, 사이즈
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};   // 데이터당 크기 정하는 구조체
	// 다이렉트 버젼에 따라 달라짐

	CONTEXT->Map(m_CB.Get()							// ID3D11Resource에 대한 포인터
			, 0										// 리소스의 인덱스 번호
			, D3D11_MAP::D3D11_MAP_WRITE_DISCARD	// 읽기 및 쓰기 와 const할지 말지 정보 표시
			, 0										// GPU가 사용 중일 때 CPU가 수행하는 작업을 지정하는 플래그(선택)
			, &tSub);								// D3D11_MAPPED_SUBRESOURCE 이 구조체를 넣어줌
	// 메모리 복사
	memcpy(tSub.pData, _pSrc, _iSize); // _pSrc(두번째 인자)만큼 _Size길이 만큼 복사해서 tSub.pData만큼 복사해서 붙여넣는 함수

	CONTEXT->Unmap(m_CB.Get(), 0);
	// 리소스에 대한 포인터를 무효화하고 해당 리소스에 대한 GPU의 액세스를 다시 활성화합니다.
}

void CConstBuffer::SetData(void* _pSrc)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSub);

	memcpy(tSub.pData, _pSrc, m_tDesc.ByteWidth);

	CONTEXT->Unmap(m_CB.Get(), 0);
}

void CConstBuffer::UpdateData(UINT _iPipelineStage)
{
	if ((UINT)PIPELINE_STAGE::VS & _iPipelineStage)
		CONTEXT->VSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::HS & _iPipelineStage)
		CONTEXT->HSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::DS & _iPipelineStage)
		CONTEXT->DSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::GS & _iPipelineStage)
		CONTEXT->GSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());

	if ((UINT)PIPELINE_STAGE::PS & _iPipelineStage)
		CONTEXT->PSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
}

void CConstBuffer::UpdateData_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_eCBType, 1, m_CB.GetAddressOf());
}


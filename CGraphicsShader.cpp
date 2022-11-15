#include "pch.h"
#include "CGraphicsShader.h"

#include "CPathMgr.h"
#include "CDevice.h"
#include "CResMgr.h"


CGraphicsShader::CGraphicsShader()
	: CShader(RES_TYPE::GRAPHICS_SHADER)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_eRSType(RS_TYPE::CULL_BACK)
	, m_eDomain(SHADER_DOMAIN::NONE)
{
}

CGraphicsShader::~CGraphicsShader()
{
}

void CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)    // 정점 정보를 fx 쉐이더랑 연동하는 함수
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	HRESULT hr = D3DCompileFromFile(	strFilePath.c_str()					// fx 최종 파일 경로
								, nullptr									// 쉐이더 메크로 정하는 인수 
								, D3D_COMPILE_STANDARD_FILE_INCLUDE			// #include 처리메크로 표준메크로를 전달해주면 된다.
								, _strFuncName.c_str()						// fx 함수 이름
								, "vs_5_0"									// 버젼 5
								, 0											// fx 파일 컴파일 방식 설정
								, 0											// 비트 연산을 통한 컴파일 방식 설정
								, m_VSBlob.GetAddressOf()					// fx 코드를 넣어주는 메모리블록
								, m_ErrBlob.GetAddressOf());				// 오류 메세지를 넣어줄 메모리 블록

	if (FAILED(hr))
	{
		const char* pErr = (const char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Compile Failed!!", MB_OK);
	}

	hr = DEVICE->CreateVertexShader(	m_VSBlob->GetBufferPointer()		// 쉐이더에 대한 포인터
									,	m_VSBlob->GetBufferSize()			// 쉐이더의 크기
									,	nullptr								// ClassLinkage fx에 대한것 c++로 작성한걸 넘기는듯?
									,	m_VS.GetAddressOf());				// 쉐이더의 포인터의 주소
	if (FAILED(hr))
	{
		const char* pErr = (const char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}

	const vector<D3D11_INPUT_ELEMENT_DESC>& vecLayout = CResMgr::GetInst()->GetInputLayoutInfo();

	// vector.data() 벡터의 첫주소값 리턴
	// IA에서 입력-버퍼 데이터를 설명하는 입력-레이아웃 개체를 만듭니다.
	hr = DEVICE->CreateInputLayout(vecLayout.data()				// 입력 어셈블러 단계 입력 데이터 유형의 배열
							, (UINT)vecLayout.size()			// 들어간 대열의 사이즈
							, m_VSBlob->GetBufferPointer()		// 만들어진 쉐이더의 포인터 
							, m_VSBlob->GetBufferSize()			// 만들어진 쉐이더의 사이즈
							, m_Layout.GetAddressOf());			// 만들어진 레이아웃의 포인터 주소

	if (FAILED(hr))
	{
		const char* pErr = (const char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}

	assert(!FAILED(hr));
}


void CGraphicsShader::CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "gs_5_0", 0, 0, m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		const char* pErr = (const char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}

	hr = DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf());

	assert(!FAILED(hr));
}

void CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", 0, 0, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		const char* pErr = (const char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}

	hr = DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	assert(!FAILED(hr));
}

// 설정된 RS, DS, BS, 정점 정보, 탑폴로지유형 정보, 계속 업데이트
void CGraphicsShader::UpdateData()
{
	// 버텍스 쉐이더와 픽셀쉐이더 매틱마다 업데이트
	CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	CONTEXT->HSSetShader(m_HS.Get(), 0, 0);
	CONTEXT->DSSetShader(m_DS.Get(), 0, 0);
	CONTEXT->GSSetShader(m_GS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);

	// 레스터라이즈 스테이트 설정
	ComPtr<ID3D11RasterizerState> RSState = CDevice::GetInst()->GetRasterizerState(m_eRSType);
	// 레스터라이즈에 RS 정보를 전달
	CONTEXT->RSSetState(RSState.Get());

	// 뎊스 스텐실 스테이트 설정
	ComPtr<ID3D11DepthStencilState> DSState = CDevice::GetInst()->GetDepthStencilState(m_eDSType);
	// OM에서 뎁스스탠실 설정을 하기 떄문에 OM스테이지에 전달
	CONTEXT->OMSetDepthStencilState(DSState.Get(), 0);

	// 블렌드 스테이트 설정
	ComPtr<ID3D11BlendState> BSState = CDevice::GetInst()->GetBlendState(m_eBSType);
	// 알파값의 혼합정도를 어케해야될지 값을 전달
	CONTEXT->OMSetBlendState(BSState.Get(), nullptr, 0xffffff);

	// Vertex정보 전달
	CONTEXT->IASetInputLayout(m_Layout.Get());
	// 어떤 방식으로 정점정보를 연결할지 전달
	// 기본 유형 및 데이터 순서에 대한 바인딩 정보
	CONTEXT->IASetPrimitiveTopology(m_eTopology);
}

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

void CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", 0, 0, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		const char* pErr = (const char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Compile Failed!!", MB_OK);
	}

	hr = DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, m_VS.GetAddressOf());


	const vector<D3D11_INPUT_ELEMENT_DESC>& vecLayout = CResMgr::GetInst()->GetInputLayoutInfo();

	hr = DEVICE->CreateInputLayout(vecLayout.data(), (UINT)vecLayout.size()
								 , m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
								 , m_Layout.GetAddressOf());

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

void CGraphicsShader::UpdateData()
{
	CONTEXT->VSSetShader(m_VS.Get(), 0, 0);
	CONTEXT->HSSetShader(m_HS.Get(), 0, 0);
	CONTEXT->DSSetShader(m_DS.Get(), 0, 0);
	CONTEXT->GSSetShader(m_GS.Get(), 0, 0);
	CONTEXT->PSSetShader(m_PS.Get(), 0, 0);

	// �����Ͷ����� ������Ʈ ����
	ComPtr<ID3D11RasterizerState> RSState = CDevice::GetInst()->GetRasterizerState(m_eRSType);
	CONTEXT->RSSetState(RSState.Get());

	// �X�� ���ٽ� ������Ʈ ����
	ComPtr<ID3D11DepthStencilState> DSState = CDevice::GetInst()->GetDepthStencilState(m_eDSType);
	CONTEXT->OMSetDepthStencilState(DSState.Get(), 0);

	// ���� ������Ʈ ����
	ComPtr<ID3D11BlendState> BSState = CDevice::GetInst()->GetBlendState(m_eBSType);
	CONTEXT->OMSetBlendState(BSState.Get(), nullptr, 0xffffff);

	
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_eTopology);
}

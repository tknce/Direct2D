#include "pch.h"
#include "CComputeShader.h"

#include "CConstBuffer.h"

#include "CPathMgr.h"
#include "CDevice.h"

CComputeShader::CComputeShader(UINT _iGroupPerX, UINT _iGroupPerY, UINT _iGroupPerZ)
	: CShader(RES_TYPE::COMPUTE_SHADER)
	, m_iGroupX(0)
	, m_iGroupY(0)
	, m_iGroupZ(0)
	, m_iGroupPerThreadX(_iGroupPerX)
	, m_iGroupPerThreadY(_iGroupPerY)
	, m_iGroupPerThreadZ(_iGroupPerZ)
{
}

CComputeShader::~CComputeShader()
{
}

void CComputeShader::CreateComputeShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "cs_5_0", 0, 0, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		const char* pErr = (const char*)m_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Compile Failed!!", MB_OK);
		assert(nullptr);
	}

	hr = DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize()
		, nullptr, m_CS.GetAddressOf());

	assert(!FAILED(hr));
}

void CComputeShader::Excute()
{
	UpdateData();

	// 상수 업데이트
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Param);
	pCB->UpdateData_CS();

	// Compute shader setting
	CONTEXT->CSSetShader(m_CS.Get(), nullptr, 0);
	// Compute shader 안에 있는 그룹 설정 몇개를 돌릴 수 있는지
	CONTEXT->Dispatch(m_iGroupX, m_iGroupY, m_iGroupZ);

	Clear();
}



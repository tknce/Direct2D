#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
	: m_iLayoutOffset(0)
	, m_bChanged(false)
{

}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[i].begin();

		for (; iter != m_arrRes[i].end(); ++iter)
		{
			iter->second = nullptr;
		}
	}

	CSound::g_pFMOD->release();
}


Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	m_bChanged = true;
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);

	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	m_bChanged = true;
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	pTex->Create(_Tex2D);

	AddRes<CTexture>(_strKey, pTex.Get());

	return pTex;
}

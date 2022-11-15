#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
	: m_iLayoutOffset(0)
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
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strkey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strkey);
	assert(!pTex.Get());

	pTex = new CTexture;
	pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);

	AddRes<CTexture>(_strkey, pTex.Get());

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strkey, ComPtr<ID3D11Texture2D> _Tex2D)
{

	Ptr<CTexture> pTex = FindRes<CTexture>(_strkey);
	assert(!pTex.Get());

	pTex = new CTexture;
	pTex->Create(_Tex2D);

	AddRes<CTexture>(_strkey, pTex.Get());

	return pTex;

}



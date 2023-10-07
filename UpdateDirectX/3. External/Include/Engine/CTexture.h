#pragma once
#include "CRes.h"

#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex/DirectXTex")
#endif


class CTexture :
    public CRes
{
private:
    ScratchImage                        m_Image;    // System Mem �� �ε��� �̹��� �ȼ������� ����
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // GPU Mem �� �ű� ������ �����ϴ� ��ü


    ComPtr<ID3D11RenderTargetView>      m_RTV;  // Tex2D �� OM(OutputMerge) RenderTarget ���� ������ �� ���
    ComPtr<ID3D11DepthStencilView>      m_DSV;  // Tex2D �� OM(OutputMerge) DepthStencil �� ������ �� ���
    ComPtr<ID3D11ShaderResourceView>    m_SRV;  // Tex2D �� t �������Ϳ� ���ε��� �� ���
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;  // Tex2D �� ��ǻƮ ���̴� u �������Ϳ� ������ �� ���
    
    


    D3D11_TEXTURE2D_DESC                m_Desc;     // Tex2D Description ����ü


public:
    void GetPixelVector(vector<vector<tBGRA>>& _inVec);
    Vec4 WIdthSearch(vector<vector<tBGRA>> _inVec, Vec2 _inPos);


public:
    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }

    ComPtr<ID3D11Texture2D>           GetTex2D() { return m_Tex2D; }
	ComPtr<ID3D11RenderTargetView>    GetRTV() { return  m_RTV; }
	ComPtr<ID3D11DepthStencilView>    GetDSV() { return  m_DSV; }
	ComPtr<ID3D11ShaderResourceView>  GetSRV() { return  m_SRV; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return  m_UAV; }


    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipelineStage);
    void UpdateData_CS(UINT _iRegisterNum, bool _bShaderRes);

    static void Clear(UINT _iRegisterNum);
    static void Clear_CS(UINT _iRegisterNum);

private:
    virtual void Save(const wstring& _strRelativePath);
    virtual int Load(const wstring& _strFilePath);

    // ����
    void Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _Format, UINT _iBindFlag);
    void Create(ComPtr<ID3D11Texture2D> _Tex2D);

    CLONE_ASSERT(CTexture);
public:
    CTexture(bool bEngineRes = false);
    ~CTexture();

    friend class CResMgr;
};


#pragma once
#include "CShader.h"

class CGraphicsShader :
    public CShader
{
private:
    // ID3DBlob                     컴파일된 fx를 저장시키는 데이터 객체 컴파일러함수 -> 경로 널 컨파일러메크로 버택스쉐이더fx 함수 Blob, Blob(컴파일을 실패할 경우에 넣어줄 에러메서지를 담아올 Blob)
    ComPtr<ID3DBlob>                m_VSBlob;
    ComPtr<ID3DBlob>                m_HSBlob;
    ComPtr<ID3DBlob>                m_DSBlob;
    ComPtr<ID3DBlob>                m_GSBlob;
    ComPtr<ID3DBlob>                m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    // ID3D11InputLayout            메모리에 등록된 Vertex정보를 IA에 전달하는 객체
    ComPtr<ID3D11InputLayout>       m_Layout;

    // 레스터라이즈단계에서 Vertex정보 그리는 방식에 따른 랜더방식 선택
    RS_TYPE                         m_eRSType;
    // 알파값 계산 방식
    BS_TYPE                         m_eBSType;
    // 깊이 방식
    DS_TYPE                         m_eDSType;

    // IA(Input Assembler)단계에 바인딩된 Vertex data를 해석하는 방식을 전달하는 객체
    D3D11_PRIMITIVE_TOPOLOGY        m_eTopology;

    // 출력 방식  출력, 비출력, 반투명
    SHADER_DOMAIN                   m_eDomain;

public:
    void CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName);
    
    //void CreateHullShader(const wstring& _strRelativePath, const string& _strFuncName);
    // Hullshader는 폴리곤을 어떻게 분할할 것인가, 얼마나 분할할 것인가를 결정해서 
    // tessellatior 단계로 넘긴다.  
    //void CreateDomainShader(const wstring& _strRelativePath, const string& _strFuncName);
    // 
    void CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName);

    void CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName);


    void UpdateData();

public:
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopology) { m_eTopology = _eTopology; }
    void SetRSType(RS_TYPE _eType) { m_eRSType = _eType; }
    void SetBSType(BS_TYPE _eType) { m_eBSType = _eType; }
    void SetDSType(DS_TYPE _eType) { m_eDSType = _eType; }

    void SetDomain(SHADER_DOMAIN _eDomain) { m_eDomain = _eDomain; }
    SHADER_DOMAIN GetDomain() { return m_eDomain; }

public:
    CGraphicsShader();
    ~CGraphicsShader();
};


#pragma once
#include "CShader.h"
class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>                m_CSBlob;   // shader만들 때 쓰는 정보값
    ComPtr<ID3D11ComputeShader>     m_CS;       // computeshader 다이렉트에 있는 클래스

protected:
    tMtrlConst                      m_Param;    // CS 에 전달할 상수 값 상수버퍼

    UINT                            m_iGroupX;  // Thread 속 그룹 X축
    UINT                            m_iGroupY;  // Thread 속 그룹 Y축
    UINT                            m_iGroupZ;  // Thread 속 그룹 Z축

    UINT                            m_iGroupPerThreadX; // 그룹을 가지고 있는 Thread X축
    UINT                            m_iGroupPerThreadY; // 그룹을 가지고 있는 Thread Y축
    UINT                            m_iGroupPerThreadZ; // 그룹을 가지고 있는 Thread Z축

public:
    void CreateComputeShader(const wstring& _strRelativePath, const string& _strFuncName); // computeshader Resmgr에 등록
    void Excute(); 

    virtual void UpdateData() = 0;
    virtual void Clear() = 0;


public:
    CComputeShader(UINT _iGroupPerX,UINT _iGroupPerY,UINT _iGroupPerZ);
    ~CComputeShader();
};


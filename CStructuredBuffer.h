#pragma once
#include "CEntity.h"

// 메쉬Mesh,인덱스버퍼Indexbuffer, 상수버퍼 Constantbuffer
// 구조화버퍼란 HLSL에 바인딩할 목적인 버퍼이지만, 단위가 크고 유동적인 데이터를
// 넘길때 쓰이는 버퍼이다.
// 특징은 
class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                            m_SB;
    ComPtr<ID3D11ShaderResourceView>                m_SRV;
    ComPtr<ID3D11UnorderedAccessView>               m_UAV;

    D3D11_BUFFER_DESC                               m_tDesc;
    SB_TYPE                                         m_eType;

    UINT                                            m_iElementSize;
    UINT                                            m_iElementCount;

    UINT                                            m_iRecentRegisterNum;   // 최근 사용한 t레지스터 갯수
    UINT                                            m_iRecentRegisterNumRW; // 최근 사용한 U레지스터 갯수


public:
    int Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _eType, void* _pInitial);
    UINT GetElementSize() { return m_iElementSize; }
    UINT GetElementCount() { return m_iElementCount; }

    void SetData(void* _pSrc, UINT _iElementCount);

    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _PipelineStage);
    void UpdateData_CS(UINT _iRegisterNum, bool _bShaderRes);

    void Clear();

public:
    CLONE(CStructuredBuffer);

public:
    CStructuredBuffer();
    CStructuredBuffer(const CStructuredBuffer& _origin);
    ~CStructuredBuffer();

};


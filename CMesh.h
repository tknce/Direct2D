#pragma once
#include "CRes.h"

class CMesh :
    public CRes
{
private:
    // ID3D11Buffer 구조화되지 않는 버퍼  보통 Vertex 와 Index 정보를 저장함
    ComPtr<ID3D11Buffer>    m_VB;           // 정점정보 버퍼 - 결국 필수데이터라도 정보기 때문에 버퍼로 보내야된다.
    D3D11_BUFFER_DESC       m_tVBDesc;      // 정점세부구조 설정 구조체 버퍼구조체라 안에서 플래그를 다르게 줘야한다.

    ComPtr<ID3D11Buffer>    m_IB;           // 정점의 인덱스 버퍼
    D3D11_BUFFER_DESC       m_tIBDesc;      // 정점의 인덱스 설정
    UINT                    m_iIdxCount;    // 정점의 수

public:
    // 정점정보와 인덱스정보를 취합해 만드는 함수
    int Create(void* _pVtxSysmem, size_t _iVtxCount, void* _pIdxSysmem, size_t _iIdxCount);
    // 정점의 정보 업데이트
    void UpdateData();
    // 정점정보를 토대로 그리기
    void render();
    // 파티클 전용 랜더함수
    void render_particle(UINT _iCount);

    virtual int Load(const wstring& _strFilePath) override
    {
        return S_OK;
    }

    CLONE_ASSERT(CMesh);
public:
    CMesh();
    ~CMesh();
};


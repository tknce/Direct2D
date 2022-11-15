#pragma once
#include "CComponent.h"
class CLight2D :
    public CComponent
{
private:

    tLightInfo      m_Info; // 광원 정보

public:
    // 광원정보 주기
    const tLightInfo& GetLightInfo() { return m_Info;    }

    // 빛의 색 필터
    void SetLightColor(Vec3 _vColor) { m_Info.vDiff = _vColor; }
    // 아무리 어두워도 어느정도 밝기를 유지할지 설정
    void SetLightEvbient(Vec3 _vColor) { m_Info.vEmb = _vColor; }

    // 타입 설정
    // 1. 방향성(태양) 2. 점광원(원래빛) 3. 스포트라이트
    void SetLightType(LIGHT_TYPE _eType) { m_Info.iLightType = _eType; }

    // 점광원 범위 설정
    void SetRadius(float _fRadius) { m_Info.fRadius = _fRadius; }
    // 방향성,스포트라이트 방향 설정
    void SetAngle(float _fAngle) { m_Info.fAngle = _fAngle; }

    // 컬러 얻기
    Vec3 GetLightColor() { return m_Info.vDiff; }
    // 타입 얻기
    LIGHT_TYPE GetLightType(LIGHT_TYPE _eType) { return m_Info.iLightType; }
    // 범위 얻기
    float GetRadius(float _fRadius) { return m_Info.fRadius; }
    // 방향 얻기(방향성, 스포트라이트)
    float GetAngle(float _fAngle) { return m_Info.fAngle; }

public:
    virtual void finaltick() override;

    CLONE(CLight2D);

public:
    CLight2D();
    ~CLight2D();
};


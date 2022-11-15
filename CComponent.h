#pragma once
#include "CEntity.h"

#include "CGameObject.h"

#define GET_OTHER_COMPONENT(ComponentName) C##ComponentName* ComponentName() { return m_pOwnerObject->ComponentName(); }

class CComponent :
    public CEntity
{
private:
    // 주인
    CGameObject*            m_pOwnerObject;
    // 내타입
    const COMPONENT_TYPE    m_eType;
    // 파티클의 생존여부 - flase로 판단되면 GS단계에서 리턴시킴
    bool                    m_bActive;

public:
    // 초기화 함수
    virtual void begin() {}
    // 매틱마다 실행
    virtual void tick() {}
    // 매틱마다 실행 주로 포지션 정보
    virtual void finaltick() = 0;

    
public:
    // 컴포넌트 타입 리턴
    COMPONENT_TYPE GetType() { return m_eType; }
    // 주인 리턴
    CGameObject* GetOwner() { return m_pOwnerObject; }
    // 주인 파괴
    void Destroy() { GetOwner()->Destroy(); }

    void Activate() { m_bActive = true; }
    void Deactiveate() { m_bActive = false; }
    bool IsActive() { return m_bActive; }


    // 등록된 컴포넌트 옴겨주기
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Animator2D);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(ParticleSystem);


    // 가상함수를 클론으로 설정
    virtual CComponent* Clone() = 0;




public:
    // 기본생성자
    CComponent(COMPONENT_TYPE _eType);
    // 복사생성자
    CComponent(const CComponent& _origin);
    // 하위 자식들 소멸자 호출
    virtual ~CComponent();

    // 게임오브젝트 클래스 친구해줌 효과, 비밀정보도 얻을 수 있음
    friend class CGameObject;
};


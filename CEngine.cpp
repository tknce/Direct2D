#include "pch.h"
#include "CEngine.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CCollisionMgr.h"

#include "CDevice.h"





CEngine::CEngine()
	: m_hMainWnd(nullptr)
{

}

CEngine::~CEngine()
{

}

// 게임이 실행될 때 한번 실행 초기값 세팅
int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hMainWnd = _hWnd;
	m_vResolution.x = (float)_iWidth;
	m_vResolution.y = (float)_iHeight;

	// 비트맵 해상도를 설정하기 위한 실제 윈도우 크기 계산
	RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// 윈도우 크기 변경 및 Show 설정
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);

	// Manager 초기화
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution))) // 설정한 해상도 얻어오기 실패할경우 실패,
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();	// content 폴더 경로 설정하기
	CKeyMgr::GetInst()->init();		// 우리가 알보기 쉬운 키값과 윈도우 키값 일치시키기
	CTimeMgr::GetInst()->init();	// 초당 1000만 진동수를 얻어오는 함수와 현재 실행시키고 난 뒤의 진동수를 멤버변수에 일치시키기

	CResMgr::GetInst()->init();		// mesh정보 만들기 Rect, circle, debug용
	CRenderMgr::GetInst()->init();	// 아직 하는 것이 없음
	CLevelMgr::GetInst()->init();	// 생성한 레벨 및 게임 오브젝트 생성   init실행 순서 Level -> Layer -> 최상위부모Gameobject -> component -> 각각의 컴포넌트



	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();
}

void CEngine::tick()
{
	// Manager 업데이트
	// 델타타임 업데이트 
	CTimeMgr::GetInst()->tick();
	// 키 상태 업데이트
	CKeyMgr::GetInst()->tick();
	// 캠 벡터 비워주기
	CRenderMgr::GetInst()->tick();

	// 현재 레벨 -> 레이어 -> 레이어안에 들어있는 최상위 부모오브젝트 -> 오브젝트에 달려있는 컴포넌트
	CLevelMgr::GetInst()->tick();

	// 현재 레벨 -> 레이어 -> 레이어 안에 들어있는 최상위 부모오브젝트 
	// -> 부모 컴포넌트 , 자식오브젝트(자식이 없을떄까지 재귀적으로 진행), 자기의 레이어 인덱스를 레이어에 등록 -> 컴포넌트 진행 
	CLevelMgr::GetInst()->finaltick();


	CCollisionMgr::GetInst()->tick();
}

void CEngine::render()
{
	CDevice::GetInst()->TargetClear();

	CTimeMgr::GetInst()->render();

	CRenderMgr::GetInst()->render();

	CEventMgr::GetInst()->tick();
}

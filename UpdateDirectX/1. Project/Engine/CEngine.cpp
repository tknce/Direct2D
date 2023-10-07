#include "pch.h"
#include "CEngine.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CDevice.h"
#include "CCollisionMgr.h"
#include "CFontMgr.h"

#include "CSound.h"




CEngine::CEngine()
	: m_hMainWnd(nullptr)	
{

}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hMainWnd = _hWnd;
	m_vResolution.x = (float)_iWidth;
	m_vResolution.y = (float)_iHeight;

	// ��Ʈ�� �ػ󵵸� �����ϱ� ���� ���� ������ ũ�� ���
	RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// ������ ũ�� ���� �� Show ����
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);

	// Manager �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
			
	CResMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	//CLevelMgr::GetInst()->init();
	CFontMgr::GetInst()->init();
	

	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();
}

void CEngine::begin()
{
	CLevelMgr::GetInst()->init();
}

void CEngine::tick()
{
	// Manager ������Ʈ
	//CResMgr::GetInst()->tick();
	CSound::g_pFMOD->update();
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();
	CRenderMgr::GetInst()->tick();
	
	CResMgr::GetInst()->tick();
	
	
	CLevelMgr::GetInst()->Progress();
	
	


	CCollisionMgr::GetInst()->tick();

	if (KEY_TAP(KEY::H))
	{
		tEvent evn = {};
		evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
		evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
		CEventMgr::GetInst()->AddEvent(evn);
	}
}

void CEngine::render()
{	
	CDevice::GetInst()->TargetClear();

	CRenderMgr::GetInst()->render();

	CFontMgr::GetInst()->render();

	CTimeMgr::GetInst()->render();
	// �̺�Ʈ�Ŵ���
	//CEventMgr::GetInst()->tick();
}

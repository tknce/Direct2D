#include "pch.h"
#include "CCamereScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CCamereScript::CCamereScript()
	: CScript(CAMERESCRIPT)
{
}

CCamereScript::~CCamereScript()
{
}


void CCamereScript::begin()
{
	CGameObject* Player = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");

	Camera()->SetObject(Player);
}

void CCamereScript::tick()
{
}
#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	AISCRIPT,
	BLOODSCRIPT,
	BROKENOBJECT,
	CAMERESCRIPT,
	CLEARSCRIPT,
	CLEAR_1SCRIPT,
	DEADSCRIPT,
	DOORSCRIPT,
	FACTORY_1SCRIPT,
	FLOORDOWNSCRIPT,
	FLOORSCRIPT,
	GASTERSCRIPT,
	GROUNTSCRIPT,
	GUNPROJSCRIPT,
	GUNSPARKSCRIPT,
	LASTSCRIPT,
	LIGHTERSCRIPT,
	MAINSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	NONGRABWALLSCRIPT,
	PLAYERSCRIPT,
	POMPSCRIPT,
	RAZERSCRIPT,
	STAIRSCRIPT,
	TARGETSCRIPT,
	TILEMAPSTATE,
	TIMERSCRIPT,
	TITLESCRIPT,
	TURNBACKSCRIPT,
	UICURSORSCRIPT,
	UIICONSCRIPT,
	UISCRIPT,
	UISKILLSCRIPT,
	VENTSCRIPT,
	WALLSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};

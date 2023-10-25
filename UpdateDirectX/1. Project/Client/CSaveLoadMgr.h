#pragma once

class CLevel;
class CLayer;
class CGameObject;

struct TIMESTATE {
    bool recording;
    bool play;
    bool Rerecording;
};

struct SAVEINSIDE {
    CGameObject*    Object;
    Vec3            ObjectPos;
    Vec3            ObjectDir;
    wstring         ObjectAniName;
    int             ObjectIdx;
};

class CSaveLoadMgr :
    public CSingleton<CSaveLoadMgr>
{
private:
    int                         m_iSaveIdx;
    int                         m_iCurIdx;
    wstring                     m_vSave;

 
    vector<vector<SAVEINSIDE>>  m_vSavesave;


    FILE*                       m_pFile;

    float                       m_fAcctime;
    
    TIMESTATE                   m_Timestate;

    bool                        m_bChangeLevel;
public:
    void init();
    void tick();
public:
    void SaveLevel(CLevel* _Level, wstring _strRelativePath);
    CLevel* LoadLevel(wstring _strRelativePath);

    void SaveTurnBackTime();
    void SaveTime();
    void LoadTurnBackTime();

    void SetTimeState(TIMESTATE _state) { m_Timestate = _state; }
    TIMESTATE GetTimestate() { return m_Timestate; }
    void ChangeLevel() { m_bChangeLevel = true; }
    void ClearTimeobj() { m_vSavesave.clear(); m_iSaveIdx = 0; m_iCurIdx = 0; }

    int GetSaveIDX() { return m_iSaveIdx; }
public:
    static void SaveGameObject(CGameObject* _Object, FILE* _File);
    static CGameObject* LoadGameObject(FILE* _File);

private:
    CSaveLoadMgr();
    ~CSaveLoadMgr();
    friend class CSingleton<CSaveLoadMgr>;
};


#pragma once


class CLevel;

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
private:
	CLevel*			m_pCurLevel;

public:
	CLevel* GetCurLevel() { return m_pCurLevel; }



public:
	void init();
	void tick();
	void finaltick();


public:
	CLevelMgr();
	~CLevelMgr();
};


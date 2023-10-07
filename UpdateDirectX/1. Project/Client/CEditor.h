#pragma once

class CGameObjectEx;

class CEditor
	: public CSingleton<CEditor>
{
private:
	vector<CGameObjectEx*>	m_vecEditorObj;

	list<tDebugShapeInfo>	m_DebugDrawList;
	CGameObjectEx* m_DebugDrawObject[(UINT)DEBUG_SHAPE::END];



public:
	void init();
	void progress();

private:
	void tick();
	void render();
	void render_debug();

private:
	void CreateDebugDrawObject();
	void DebugDraw(tDebugShapeInfo& _info);

public:
	CEditor();
	~CEditor();
};


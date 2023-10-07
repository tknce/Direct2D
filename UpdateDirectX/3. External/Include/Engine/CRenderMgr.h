#pragma once

class CCamera;
class CLight2D;
class CStructuredBuffer;

#include "CLight2D.h"
#include "CTexture.h"

class CRenderMgr
	: public CSingleton<CRenderMgr>
{
private:
	vector<CCamera*>		m_vecCam;		// ���� ������ �ִ� ��� ī�޶�
	CCamera*				m_EditorCam;	// Editor �� ī�޶�

	vector<tLightInfo>		m_vecLight2D;	// ���� ������ �ִ� ��� 2D ����
	CStructuredBuffer*		m_pLight2DBuffer;	// 2D ���� ������ ������ ����ȭ ����

	vector<tDebugShapeInfo>	m_DebugDrawInfo;

	Ptr<CTexture>			m_RTCopyTex;

public:
	void init();
	void tick();
	void render();

private:
	void render_game();
	void render_editor();

	void UpdateNoiseTexture();
	void UpdateLight2D();

public:
	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void RegisterEditorCamera(CCamera* _pCam) { m_EditorCam = _pCam; }

	void RegisterLight2D(CLight2D* _pLight2D) { m_vecLight2D.push_back(_pLight2D->GetLightInfo()); }

	CCamera* GetMainCam();


	// ����Ÿ���� ī���ؽ��ķ� ����
	void CopyRenderTarget();

	void DebugDraw(DEBUG_SHAPE _eShape, Vec4 _vColor, Vec3 _vPosition, Vec3 _vScale, Vec3 _vRotation, float _fRadius, float _fDuration) 
	{
#ifdef _DEBUG
		m_DebugDrawInfo.push_back(tDebugShapeInfo{ _eShape , _vColor, _vPosition , _vScale , _vRotation , _fRadius , _fDuration , 0.f });
#endif
	}

	vector<tDebugShapeInfo>& GetDebutgDrawInfo() { return m_DebugDrawInfo;}


public:
	CRenderMgr();
	~CRenderMgr();
};


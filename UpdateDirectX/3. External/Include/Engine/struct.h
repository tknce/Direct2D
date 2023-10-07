#pragma once


struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};


struct tEvent
{
	EVENT_TYPE	eType;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

struct tLightInfo
{
	Vec4		vDiff;		// ���ݻ� ��
	Vec4		vSpec;		// �ݻ籤 ���
	Vec4		vEmb;		// ȯ�汤
	
	Vec4		vWorldPos;	// ���� ���� ��ġ
	Vec4		vWorldDir;	// ������ ����
	LIGHT_TYPE	iLightType;	// ���� Ÿ��
	float		fRadius;	// ���� �ݰ� (POINT, SPOT)
	float		fAngle;		// ���� ���� (SPOT)
	int			pad;
};


// ================
// Animation ����ü
// ================
struct tAnim2DFrm
{
	Vec2 vLeftTop; 
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
};


// ===============
// Particle ����ü
// ===============
struct tParticle
{
	Vec4 vRelativePos;
	Vec4 vDir;
		
	float fMaxTime;
	float fCurTime;
	float fSpeed;
	UINT  iActive;
};

struct tParticleShare
{
	UINT	iAliveCount;
	float	fSeta;
	Vec2	vVelopcity;
	Vec2	vForce;
	UINT	Padding[2];
};

// ==============
// TileMap ����ü
// ==============

struct tTile
{
	Vec2 vLeftTop;	// ��Ʋ���� �̹��� ���� �� ������ �������� �ڸ��� uv���� ����ؼ� �ڸ��� ���� ����     tex�̹��� ���μ��� ���̸� ����
	Vec2 vSlice;	// �ڸ� ũ��
};




// ================
// Debug Shape Info
// ================
struct tDebugShapeInfo
{
	DEBUG_SHAPE eShape;
	Vec4		vColor;
	Vec3		vPosition;
	Vec3		vScale;
	Vec3		vRot;
	float		fRadius;
	float		fDuration;
	float		fCurTime;
};


// ================
// Shader Parameter
// ================
struct tScalarParam
{
	SCALAR_PARAM eParam;
	string		 strName;
};

struct tTextureParam
{
	TEX_PARAM	eParam;
	string		strName;
};


// ================
// ������ۿ� ����ü
// ================
struct tTransform
{	
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int	HasTex[(UINT)TEX_PARAM::TEX_END];
};

struct tGlobalData
{
	Vec2	vRenderResolution;
	Vec2	vNoiseResolution;
	int		iLight2DCount;
	int		iLight3DCount;

	float	fAccTime;
	float	fDT;	
};

extern tGlobalData g_global;

struct tAnim2DInfo
{	
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;

	int iAnim2DUse;
	int iPadding[3];
};

struct tBGRA
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;

	bool        check;
};

struct Statevalue
{
	bool enter;
	bool tick;
	bool exit;
};

struct STATE_VAL_IDLE
{
	bool enter;
	bool tick;
	bool exit;
};

struct STATE_VLA_TRACE
{
	bool enter;
	bool tick;
	bool exit;
};
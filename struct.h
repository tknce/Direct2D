#pragma once


struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

struct tEvent
{
	EVENT_TYPE		eType;
	DWORD_PTR		wParam; // DWORD_PTR이란 다용도 포인터
	DWORD_PTR		IParam;
};

struct tLightInfo // 빛 관련 바인딩할 용도
{
	Vec4			vDiff;		// 난반사 광 - 빛이 표면에 물어올떄 부딪히는 계수
	Vec4			vSpec;		// 반사광 계수
	Vec4			vEmb;		// 환경광 - 아무리 어두워도 보이는 계수

	Vec4			vWorldPos;	// 광원 월드 위치
	Vec4			vWorldDir;	// 광원의 방향
	LIGHT_TYPE		iLightType;	// 광원 타입
	float			fRadius;	// 광원 반경 (POINT, SPOT)
	float			fAngle;		// 광원 각도 (SPOT)
	int				pad;
};


// ================
// Animation 구조체
// ================
struct tAnim2DFrm
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
};


// ==============
// Particle 구조체
// ==============
struct tParticle
{
	Vec4	vRelativePos;
	Vec4	vDir;

	float	fMaxTime;
	float	fCurTime;
	float	fSpeed;
	UINT	iActive;
};


// ================
// Debug Shape Info
// ================
struct  tDebugShapeInfo
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
// 상수버퍼용 구조체
// ================
struct tTransform
{	
	Matrix matWorld;				// 월드좌표 계산 행렬 크기 * 회전 * 위치(순서가 중요하다.) 
	Matrix matView;					// 카메라가 보는 뷰
	Matrix matProj;					// 카메라 투영행렬 직교인지 원근인지

	Matrix matWV;					// ndc좌표계에서 월드상에 좌표로 미리 계산 
	Matrix matWVP;					// ndc좌표계에서 월드상에 좌표로 미리 계산
};

extern tTransform g_transform;

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];
};

struct tGlobalDate 
{
	Vec2	vRenderResolution;
	int		iLight2DCount;
	int		iLight3DCount;

	float	fAccTime;
	float	fDT;
	int		ipad[2];
};

extern tGlobalDate g_global;

struct tAnim2DInfo
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;

	int iAnim2DUse;
	int iPadding[3];
};
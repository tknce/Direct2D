#pragma once

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

// 쭉눌름
#define KEY_PRESSED(key)	CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::PRESS
// 한번만 누름
#define KEY_TAP(key)		CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::TAP
// 뗌
#define KEY_RELEASE(key)	CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::RELEASE

// 복사가능
#define CLONE(type) public: type* Clone() {return new type(*this);}
// 복사호출은 되지만 안됌
#define CLONE_DUMMY(type) public: type* Clone() {return nullptr;}
// 복사되면 큰일남
#define CLONE_ASSERT(type) public: type* Clone() {assert(nullptr); return nullptr;}

#define MAX_LAYER 32

enum class SAMPLER_TYPE
{
	ANISOTROPIC, // 이방성  뿌옇게 일어나는 효과
	POINT,		 // 픽셀처럼 보이게 출력하는 효과

	END,
};

// 바인딩되는 상수버퍼값
enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	ANIMATION2D, // b3

	END,
};

// Resterizer State
enum class RS_TYPE
{
	CULL_BACK,		// 앞면만 출력   반시계방향 정점
	CULL_FRONT,		// 뒷면만 출력	 시계방향 정점
	CULL_NONE,		// 앞뒷면 출력
	WIRE_FRAME,		// 정점 선만 출력
	END,
};

enum class BS_TYPE
{
	DEFAULT,		// 그냥 무조건 위로
	ALPHABLEND,		// SrcBlend * SrcBlendAlpha + DestBlend * ( 1- SrcBlendAlpha )	transparent 일반적인 알파블랜딩 no write인 이유 앞에 그려져야지 투명과 배경과의 색섞임을 할 수 있어서
	ONE_ONE,		// ScrBlend * 1 + DestBlend	 ex) 초록색 필터

	END,
};

enum class DS_TYPE
{
	LESS,				// Less, Depth Write			앞뒤로 판단하고, 깊이를 남긴다
	GREATER,			// Greater, Depth Write			뒤앞으로 판단하고, 깊이를 남긴다
	NO_WRITE,			// Less, Depth no Write			앞뒤로 판단하고, 깊이를 남긴다 -> 설정된 것간의 우열 판단.
	NO_TEST_NO_WRITE,	// NoTest, Depth No Write		판단도 안하고 깊이도 남기지 않는다. 무조건 화면 위에 생성

	END,
};

enum PIPELINE_STAGE
{
	VS = 0x01,			// 버텍스쉐이더			정점정보를 보내주면 정점을 생성
	HS = 0x02,			// 헐쉐이더				효율적으로 정점삼각형을 더 나눔, 각 입력 패치(사각형, 삼각형 또는 선)에 해당하는 기하 도형 패치(및 패치 상수)를 생성합니다. 
	DS = 0x04,			// 도메인 쉐이더		테셀레이터 단계에서 설정된 세분화 삼각형들을 위치 계산하는 단계 
	GS = 0x08,			// 지오메트리 쉐이더	(기하 쉐이더) 기하학할때 기하 : 공간에 있는 도형의 성질, 즉 대상들의 치수, 모양, 상대적 위치 등을 연구하는 수학의 한 분야
	PS = 0x10,			// 픽셀 쉐이더			보간작업을 통안 이미지 출력
	ALL_STAGE = VS | HS | DS | GS | PS,
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		// 불투명 단계
	DOMAIN_MASK,		// 출력, 비출력
	DOMAIN_TRANSPARENT,	// 반투명

	NONE,
};

enum class RES_TYPE
{
	PREFAB,					// 프리팹
	MESHDATA,				// 정점 데이터
	COMPUTE_SHADER,			// 정점의 자세한 데이터

	MATERIAL,				// 재질
	MESH,					// 정점
	TEXTURE,				// 이미지정보들
	SOUND,					// 사운드
	GRAPHICS_SHADER,		// 각종 쉐이터 RS, DS, BS 값 설정하고 전달
	
	END,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,				// 위치값 NDC좌표계의 값을 행렬로 누적계산함 크기, 회전, 이동순
	CAMERA,					// 카메라 z축을 기준으로 모든 오브젝트 값 재 정렬
	COLLIDER2D,				// 충돌체
	COLLIDER3D,
	ANIMATOR2D,				// 애니메이션
	ANIMATOR3D,
	LIGHT2D,				// 빛
	LIGHT3D,

	// render
	MESHRENDER,				// 메쉬그리기
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,


	END,

	SCRIPT,					// 스크립트는 따로 돌아야되기떄문에 END다음으로 넣는다
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,		// wParam : Object Adress, IParam : Layer Index
	DELETE_OBJECT,		// wParam : Object Adress,
	CHANGE_LEVEL,

	END,
};

enum class DEBUG_SHAPE
{
	// 2D
	RECT,				// 그려질 정점 4개
	CIRCLE,				// 그려질 정점 정보 무수히 많이

	// 3D
	CUBE,
	SPHERE,

	END,
};

// 충돌체 모양
enum class COLLIDER2D_TYPE
{
	COLLIDER2D_RECT,	// 네모
	COLLIDER2D_CIRCLE,	// 동그라미
};

// 바인딩할 수 있는 인자값
enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

// 텍스쳐 바인딩 할 곳
enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,


	TEX_END,
};

enum class DIR
{					// 좌표상 위치값
	RIGHT,			// +x
	UP,				// +y
	FRONT,			// +z
	END,
};

// 구조화 버퍼 생성시 무슨 타입인지 알려주기 위한 이넘클래스
enum class SB_TYPE
{
	SRV_ONLY,
	UAV_INC,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성 빛이 한방향으로 내려온다. -> 주로 태양같은 멀리서 온 빛을 표현하기위해 쓰임
	POINT,			// 점광원 사방에서 빛이 날라감 -> 이게 원래 빛
	SPOT,			// 스포트 라이트
};
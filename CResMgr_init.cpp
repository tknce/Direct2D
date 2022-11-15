#include "pch.h"
#include "CResMgr.h"


void CResMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();

	CreateDefaultPrefab();
}

void CResMgr::CreateDefaultMesh() // 레스터라이져에 있는 ndc좌표계에서 표시될 정점들의 정보
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	CMesh* pMesh = nullptr;

	// Point Mesh
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;

	pMesh = new CMesh;
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh);

	// 사각형 메쉬 만들기
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);




	// Index 정점들의 읽는 순서 조정
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	// 삼각형 2개를 이어서 사각형으로 만든 메쉬를 "RectMesh"라 부르기로 했다.
	AddRes<CMesh>(L"RectMesh", pMesh);
	vecIdx.clear();




	// Index Debug용 mesh 생성
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	// 삼각형 2개를 이어서 사각형으로 만들고 다시 라인선을 딴 전용 메쉬를
	//"RectMesh_Debug"라 부르기로 했다.
	AddRes<CMesh>(L"RectMesh_Debug", pMesh);
	//vecVtx.clear();
	vecVtx.clear();
	vecIdx.clear();



	// 원형메쉬 만들기
	// 중심점	
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	vecVtx.push_back(v);

	int iSlice = 40; // 정밀한 사각형 만들기 위한 변수
	float fRadius = 0.5f;	
	float fTheta = XM_2PI / (float)iSlice;

	for (int i = 0; i < iSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);
	}


	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		vecIdx.push_back(0);
		if (i == iSlice - 1)
		{
			vecIdx.push_back(1);
		}
		else
		{
			vecIdx.push_back(i + 2);
		}
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	// 무수히 만은 삼각형을 이어 만든 원을 이제부터 "CircleMesh"라 부르기로 했다.
	AddRes<CMesh>(L"CircleMesh", pMesh);
	vecIdx.clear();


	// Circle_debug
	for (int i = 0; i < vecVtx.size() - 2; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	// 무수히 만은 삼각형을 이어 만든 원을 라인만 딴 정점 집합체를
	// 이제부터 "CircleMesh_Debug"라 부르기로 했다.
	AddRes<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CResMgr::CreateDefaultTexture() // 업로드해논 텍스쳐들의 정보
{

	// 텍스쳐 로딩
	//CResMgr::GetInst()->Load<CTexture>(L"MagicCircle", L"texture\\Player.bmp");
	Load<CTexture>(L"Plane", L"texture\\Player.bmp");
	Load<CTexture>(L"Smoke", L"texture\\smokeparticle.png");
	Load<CTexture>(L"Character", L"texture\\MagicCircle.png");
	Load<CTexture>(L"Space", L"texture\\Space.jpg");
	Load<CTexture>(L"Link", L"texture\\link.png");
	Load<CTexture>(L"Mel", L"texture\\Character.png");
	Load<CTexture>(L"maple", L"texture\\maple.jfif");

	// 텍스쳐 생성
	CreateTexture(L"UAVTex", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
}

void CResMgr::CreateDefaultGraphicsShader()	// fx에 표시할 포지션 컬러 텍스정보들을 레이아웃에 생성 및 정보들을 조합해서 쉐이더 만들기
{
	// 정점들 속 데이터 단위 설정값
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR");
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");

	CGraphicsShader* pShader = nullptr;

	// Std2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes<CGraphicsShader>(L"Std2DShader", pShader);


	// Std2dAlphaBlend Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader);


	// Grid Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\editor.fx", "VS_Grid");
	pShader->CreatePixelShader(L"shader\\editor.fx", "PS_Grid");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"GridShader", pShader);

	// DebugDraw Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\debugdraw.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\debugdraw.fx", "PS_DebugDraw");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DebugDrawShader", pShader);


	// DeadCell Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_DeadCell");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_DeadCell");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes<CGraphicsShader>(L"DeadCellShader", pShader);

	CGraphicsShader* pShader1 = nullptr;
	// ParticleRenderShader
	pShader1 = new CGraphicsShader;
	pShader1->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader1->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader1->CreatePixelShader(L"shader\\particlerender.fx", "PS_ParticleRender");
		   
	pShader1->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader1->SetRSType(RS_TYPE::CULL_NONE);
	pShader1->SetBSType(BS_TYPE::ALPHABLEND);
	pShader1->SetDSType(DS_TYPE::NO_WRITE);
	pShader1->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	
	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader);

}
#include "CPaintShader.h"
#include "CParticleUpdateShader.h"
void CResMgr::CreateDefaultComputeShader()
{
	CComputeShader* pShader = nullptr;

	// Paint Shader
	pShader = new CPaintShader;
	pShader->CreateComputeShader(L"shader\\compute.fx", "CS_Paint");
	AddRes<CComputeShader>(L"PaintShader", pShader);

	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);

}

void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;


	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"Std2DMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes<CMaterial>(L"Std2DAlphaBlendMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"GridShader"));
	AddRes<CMaterial>(L"GridMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugDrawShader"));
	AddRes<CMaterial>(L"DebugDrawMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"MonsterMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes<CMaterial>(L"ParticleRenderMtrl", pMtrl);
}

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CMissileScript.h"

void CResMgr::CreateDefaultPrefab()
{
	CGameObject* pObject = new CGameObject;
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CMissileScript);

	pObject->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Plane"));

	AddRes<CPrefab>(L"MissilePrefab", new CPrefab(pObject));
}

// DXGI_FORMAT :  Position, Color, Texcord
void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};
	LayoutDesc.AlignedByteOffset = m_iLayoutOffset;				// 꼭짓점의 시작부분 오프셋
	LayoutDesc.Format = _eFormat;								// 색상 정보 및 깊이 정보에 대한 해석방식
	LayoutDesc.InputSlot = 0;									// IA가 식별하는 정수 값 0~15
	LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;	// 입력된 데이터 유형 
	LayoutDesc.SemanticName = _strSemanticName;					// fx에 대한 이름
	LayoutDesc.SemanticIndex = 0;								// 동일한 의미를 가진 인덱스가 2개이상 있을 시 설정
	m_vecLayoutInfo.push_back(LayoutDesc);

	m_iLayoutOffset += GetSizeofFormat(_eFormat);
}

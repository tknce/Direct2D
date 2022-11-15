#include "pch.h"
#include "CDevice.h"

#include "CResMgr.h"
#include "CConstBuffer.h"


CDevice::CDevice()
	: m_hWnd(nullptr)
	, m_tViewPort{}
	, m_arrCB{}
	, m_arrRS{}
	, m_arrDS{}
	, m_arrBS{}
{
}

CDevice::~CDevice()
{
	for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
	{
		if (nullptr != m_arrCB[i])
			delete m_arrCB[i];
	}
}
// 버퍼종류
// Vertex Buffer	정점버퍼
// Index Buffer		Vertex Buffer랑 비슷하다
// Constant Buffer	상수버퍼

int CDevice::init(HWND _hWnd, Vec2 _vResolution)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = _vResolution;

	UINT iDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;
	// Device를 만들기 전에 어떤 모드를 선택하고 어떤 레벨을 설정할지 정한다.
	// 레벨 즉 버젼을 선택할 필요가 없다. 예전에는 드라이버 업데이트를 통해 버젼을 선택했어야했는데
	// 레벨에 따라 버젼을 다르게 적용하게 바뀌어서 자동으로 바뀐다.

	ID3D11Device* pDevice = nullptr;
	HRESULT hr = D3D11CreateDevice(nullptr					// IDXGIAdapter인자 nullptr를 넣으면 기본 Adapter가 전달된다.
		, D3D_DRIVER_TYPE_HARDWARE							// HARDWARE는 컴퓨터 부품중 어느 곳을 쓸지 정하는 인자이다.이것은 그래픽카드를 쓰겠다는 의미
		, nullptr													// software 일시 어떤 것을 쓰는가
		, iDeviceFlag, nullptr, 0		// D3D11_CREATE_DEVICE_DEBUG 플래그 중 어떤 모드를 쓸지
		, D3D11_SDK_VERSION									// 어떤 버젼을 사용될지 판단하는 요소 지금은 자동으로 판단되기떄문에 널포인트로 잡아둔다
		, m_pDevice.GetAddressOf()							// 레벨수준 위와 마찬가지이다
		, &FeatureLevel									// 그냥 이거 쓰라고 나와있음
		, m_pDeviceContext.GetAddressOf());			// 더블 포인터

	UINT iQuality = 0;
	DEVICE->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM	// 텍스쳐가 나타날 수 있는 알쥐비알파값
										, 4								// RGBA라서 4개인듯?
										, &iQuality);					// 품질 수준이지만 0이면 이것도 알아서 정해지는듯

	// device에 스왚체인 생성
	// 스왚체인 만들기
	if (FAILED(CreateSwapchain())) 
	{
		MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// RenderTarget, DepthStencilTarget 만들기
	if (FAILED(CreateTarget()))
	{
		MessageBox(nullptr, L"타겟 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}
	
	// OM에 rendertarget과 depthstensil을 생성함으로써 그거와 관련된 인자들이 여기에 들어간다.
	// Output-Merger
	// 출력용 렌더 타겟 및 깊이 타겟 지정
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetTex->GetRTV().GetAddressOf(), m_pDepthStencilTex->GetDSV().Get());


	// ViewPort
	m_tViewPort.TopLeftX = 0;					// 왼쪽위	X
	m_tViewPort.TopLeftY = 0;					// 왼쪽위	Y
	m_tViewPort.Width = _vResolution.x;			// 범위		X
	m_tViewPort.Height = _vResolution.y;		// 범위		Y
	m_tViewPort.MinDepth = 0;					// 최소 깊이
	m_tViewPort.MaxDepth = 1;					// 최대 깊이

	m_pDeviceContext->RSSetViewports(1, &m_tViewPort);	//레스터라이져에 설정


	// 샘플러 생성 및 바인딩
	// 샘플러는 보간작업할때 필요한 정보주소들을 들고 각각의 쉐이더들에게 정보전달할 객체주소 생성
	if (FAILED(CreateSampler()))
	{
		MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}


	// 상수버퍼 생성
	// Trensform, Material
	if (FAILED(CreateConstBuffer()))
	{
		MessageBox(nullptr, L"상수버퍼 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// 레스터라이저 스테이트 생성
	// 정점들의 정보를 어떤 방식으로 읽을지 설정 
	// RS_TYPE
	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"레스터라이저 스테이트 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	// DepthStencil 스테이트 생성
	// 깊이들의 정보를 어떤 방식으로 읽을지 설정
	// DS_TYPE
	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"뎊스 스텐실 스테이트 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}


	// 블렌드 스테이트 생성
	// 픽셀간의 앞뒤인지, 깊이를 남길지에 대한 깊이 판정을 어떻게 할것인가 설정   
	// BS_TYPE
	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"블렌드 스테이트 생성 실패", L"Device 초기화 실패", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


int CDevice::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc = {};

	desc.OutputWindow = m_hWnd;	// Front Buffer 를 출력시킬 윈도우 핸들
	desc.Windowed = true;		// 윈도우, 전체화면 모드
	desc.BufferCount = 1;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.


	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;							// 사용 목적을 랜더 아웃풋으로 설정  쉐이더, 플래그, 렌더링, 백버퍼용 등 사용목적이 있다.
	desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;						// 해상도 x 가로길이 
	desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;						// 해상도 y 세로길이
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// 포멧 RGBA 8비트씩
	desc.BufferDesc.RefreshRate.Numerator = 60;									// 프레임 설정 60분의1
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;					// 배율 설정 배율이 필요가 없어서 배율 설정을 안하겠다는 표시
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// 스캔라인 설정하는 부분 순서를 신경안써도 된다는 메세지 전달

	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr;


	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))			// interface를 생성해서 디바이스를 넣기
	{
		return E_FAIL;
	}

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))				// 디바이스 부모에 다시 어뎁터 넣기
		{
			return E_FAIL;
		}

			if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))			// 다시 어뎁터 부모에 팩토리 넣기
			{
				return E_FAIL;
			}

				if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &desc, m_pSwapChain.GetAddressOf())))	// 그것을 스왑체인버퍼에 
	{																														// 위의 모든것이 설정되어야 생성이 된다.
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateTarget()
{
	HRESULT hr = S_OK;
	// RenderTargetTexture 를 스왚체인으로부터 참조하기
	ComPtr<ID3D11Texture2D> tex2D;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

	// RenderTargetTexture 등록
	m_pRenderTargetTex = CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);
	
	// DepthStencilTexture 생성
	m_pDepthStencilTex = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex"
		, (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y
		, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	return hr;
	// DepthStencilTexture
	//D3D11_TEXTURE2D_DESC texdesc = {};
	//
	//texdesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;  // 파이프라인 바인딩 위치를 정해주는 플래그
	//
	//texdesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;				// 사용목적 읽기만 쓸수있다거나 복사가능하다거나
	//																// 동적이거나 -> 프레임당 1번이상 쓰면 좋음
	//texdesc.CPUAccessFlags = 0;										// cpu 액세스가 필요하지 않는 경우 0을 사용함
	//
	//texdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;					// 깊이 24비트 , 스탠실은 8비트 -> 스탠실이란 픽셀마다 마스크값을 주는것(무늬를 찍어낼수있음)
	//texdesc.Width = (UINT)m_vRenderResolution.x;					// 해상도 
	//texdesc.Height = (UINT)m_vRenderResolution.y;					// 해상도 
	//texdesc.ArraySize = 1;											// 바인딩되는 텍스쳐의 수
	//
	//texdesc.SampleDesc.Count = 1;									// 픽셀당 멀티샘플 수						
	//texdesc.SampleDesc.Quality = 0;									// ID3D10Device::CheckMultisampleQualityLevels -> 11에선 안씀
	//
	//texdesc.MipLevels = 0;											// Mipmap은 랜더링 속도를 향상시키기 위한 목적으로 기본 텍스쳐와 이를 
	//																// 연속적으로 미리 축소시킨 텍스쳐들로 이루어진 비트맵 이미지의 집합
	//																// 최대 Mipmap의 레벨 수 -1로 설정하면 가장 낮은 세부 정보까지 모든 밉맵을 나타냄
	//texdesc.MiscFlags = 0;											// 리소스 옵션을 식별하는 플래그 주로 일반적이지 않는 리소스 0이면 플래그가 적용되지않는다 -> 버퍼화, 비트화

	//hr = m_pDevice->CreateTexture2D(&texdesc, nullptr, m_pDepthStencilTex.GetAddressOf());
	//
	//// DepthStencilView 제작
	//hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilTex.Get(), nullptr, m_pDepthStencilView.GetAddressOf());

	
}

int CDevice::CreateSampler()
{
	HRESULT hr = S_OK;

	// 샘플러 스테이트 생성
	D3D11_SAMPLER_DESC desc = {};
	// 픽셀들의 주소 설정 UVW 보간작업에 필요한 픽셀 스페이트 생성
	desc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;		// U
	desc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;		// V
	desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;		// W는 3차원 텍스쳐옵션인데 게임에선 쓰이지 않음
	// 보간작업 방식에 대한 필터링 방식\
	// 흐릿하게 보이는 보간작업
	desc.Filter = D3D11_FILTER_ANISOTROPIC; // 축소 확대 및 Map Level 샘플링에 이방성 보간을 사용합니다. (-1,1) ~ (1,-1) 사이에 있는 보간작업 

	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());

	// 확실한 픽셀을 보이게 하는 보간작업
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = DEVICE->CreateSamplerState(&desc, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());


	// 샘플러 바인딩
	CONTEXT->VSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->HSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->DSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->GSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->PSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());
	CONTEXT->CSSetSamplers((UINT)SAMPLER_TYPE::ANISOTROPIC, 1, m_arrSampler[(UINT)SAMPLER_TYPE::ANISOTROPIC].GetAddressOf());

	CONTEXT->VSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->HSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->DSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->GSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->PSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());
	CONTEXT->CSSetSamplers((UINT)SAMPLER_TYPE::POINT, 1, m_arrSampler[(UINT)SAMPLER_TYPE::POINT].GetAddressOf());

	return hr;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform));

	m_arrCB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer(CB_TYPE::MATERIAL);
	m_arrCB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst));

	m_arrCB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer(CB_TYPE::GLOBAL);
	m_arrCB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(tGlobalDate));

	m_arrCB[(UINT)CB_TYPE::ANIMATION2D] = new CConstBuffer(CB_TYPE::ANIMATION2D);
	m_arrCB[(UINT)CB_TYPE::ANIMATION2D]->Create(sizeof(tAnim2DInfo));

	// 상수버퍼 생성
	// 
	// D3D11_BUFFER_DESC       m_tDesc;
	// m_tDesc.ByteWidth = (UINT)_iBufferSize;											// 16 의 배수로 GPU 인식단위가 16비트 sizeof(tTransform)와 sizeof(tMtrlConst)의 크기 		
	// m_tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;									// 상수 버퍼용 플래그    
	// m_tDesc.Usage = D3D11_USAGE_DYNAMIC;												// GPU(읽기 전용)와 CPU(쓰기 전용) 모두에서 액세스할 수 있는 리소스입니다.
	//																					// GPU와 CPU의 사용목적을 설정하는 단계
	// m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;									// CPU에서 값을 변경할 수 있도록 설정 write는 입력은 할 수 없다 read는 그저 읽기만 할 수 있다.
	// 
	// HRESULT hr = DEVICE->CreateBuffer(&m_tDesc, nullptr, m_CB.GetAddressOf());

	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	HRESULT hr = S_OK;

	m_arrRS[(UINT)RS_TYPE::CULL_BACK] = nullptr;


	D3D11_RASTERIZER_DESC desc = {};

	// Cull Front Mode
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());


	// Cull None Mode
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());


	// WireFrame Mode
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

	hr = DEVICE->CreateRasterizerState(&desc, m_arrRS[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());


	return hr;
}

int CDevice::CreateDepthStencilState()
{
	HRESULT hr = S_OK;

	// Less
	m_arrDS[(UINT)DS_TYPE::LESS] = nullptr;


	D3D11_DEPTH_STENCIL_DESC desc = {};

	// Greater
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.StencilEnable = false;

	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::GREATER].GetAddressOf());

	// NO_WRITE
	desc.DepthEnable = true;
	desc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	desc.StencilEnable = false;

	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

	// NO_TEST_NO_WRITE
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.StencilEnable = false;

	DEVICE->CreateDepthStencilState(&desc, m_arrDS[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

	return hr;
}

int CDevice::CreateBlendState()
{
	HRESULT hr = S_OK;

	// Default 블렌딩
	m_arrBS[(UINT)BS_TYPE::DEFAULT] = nullptr;



	D3D11_BLEND_DESC desc = {};

	// Alpha Blend
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;


	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;




	hr = DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf());


	// One One Blend
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	DEVICE->CreateBlendState(&desc, m_arrBS[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());


	return hr;
}

void CDevice::TargetClear()
{
	float arrColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetTex->GetRTV().Get(), arrColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}


void CDevice::Present()
{
	m_pSwapChain->Present(0, 0);
}
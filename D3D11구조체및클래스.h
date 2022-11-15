#pragma once

#include "pch.h"
// ====================================================================================================
ComPtr<ID3D11Device>			m_pDevice;				// GPU 메모리 관리
UINT iDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)0;
// Device를 만들기 전에 어떤 모드를 선택하고 어떤 레벨을 설정할지 정한다.
// 레벨 즉 버젼을 선택할 필요가 없다. 예전에는 드라이버 업데이트를 통해 버젼을 선택했어야했는데
// 레벨에 따라 버젼을 다르게 적용하게 바뀌어서 자동으로 바뀐다.
//D3D11CreateDevice(nullptr			IDXGIAdapter인자 nullptr를 넣으면 기본 Adapter가 전달된다.
//  , D3D_DRIVER_TYPE_HARDWARE		HARDWARE는 컴퓨터 부품중 어느 곳을 쓸지 정하는 인자이다. 이것은 그래픽카드를 쓰겠다는 의미
//  , nullptr						software 일시 어떤 것을 쓰는가
//	, iDeviceFlag					D3D11_CREATE_DEVICE_DEBUG 플래그 중 어떤 모드를 쓸지
//  , nullptr						어떤 버젼을 사용될지 판단하는 요소 지금은 자동으로 판단되기떄문에 널포인트로 잡아둔다
//  , 0								레벨수준 위와 마찬가지이다
//	, D3D11_SDK_VERSION				그냥 이거 쓰라고 나와있음
//	, m_pDevice.GetAddressOf()		더블 포인터
//	, &FeatureLevel					지원되는 기능을 표현하는 곳
//	, m_pDeviceContext.GetAddressOf());더블포인터
//														
// UINT iQuality = 0;
// DEVICE->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM	텍스쳐가 나타날 수 있는 알쥐비알파값
//  , 4								RGBA라서 4개인듯?
//  , &iQuality);					품질 수준이지만 0이면 이것도 알아서 정해지는듯
// ===================================================================================================
ComPtr<ID3D11DeviceContext>		m_pDeviceContext;		// GPU Rendering



ComPtr<ID3D11Texture2D>			m_pRenderTargetTex;		// Render Target
ComPtr<ID3D11RenderTargetView>	m_pRenderTargetView;	// RenderTarget 을 설명

ComPtr<ID3D11Texture2D>			m_pDepthStencilTex;		// 깊이 저장 타겟
ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;	// DSTex 를 설명	
//==============================================================================================================
ComPtr<IDXGISwapChain>			m_pSwapChain;			// RenderTarget(FrontBuffer, BackBuffer) 를 관리 및 역할 교체 지시
//DXGI_SWAP_CHAIN_DESC desc = {};
//
//desc.OutputWindow = m_hWnd;	// Front Buffer 를 출력시킬 윈도우 핸들
//desc.Windowed = true;		// 윈도우, 전체화면 모드
//desc.BufferCount = 1;
//desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 이전 프레임 장면을 유지하지 않는다.
//
//desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//desc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
//desc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
//desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//desc.BufferDesc.RefreshRate.Numerator = 60;
//desc.BufferDesc.RefreshRate.Denominator = 1;
//desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//
//desc.SampleDesc.Count = 1;
//desc.SampleDesc.Quality = 0;
//
//ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
//ComPtr<IDXGIAdapter> pAdapter = nullptr;
//ComPtr<IDXGIFactory> pFactory = nullptr;
//
//
//if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))    interface를 생성해서 디바이스를 넣기
//{
//	return E_FAIL;
//}
//
//if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))	디바이스 부모에 다시 어뎁터 넣기	
//{
//	return E_FAIL;
//}
//
//if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))		다시 어뎁터 부모에 팩토리 넣기
//{
//	return E_FAIL;
//}
//
//if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &desc, m_pSwapChain.GetAddressOf())))	그것을 스왑체인버퍼에 넣는다
//{
//	return E_FAIL;
//}
//														
//==============================================================================================================
D3D11_VIEWPORT					m_tViewPort;			// 백버퍼를 윈도우에 그릴 영역(위치, 크기) 지정

Vec2							m_vRenderResolution;	// 렌더링 버퍼 해상도

ComPtr<ID3D11SamplerState>		m_arrSampler[(UINT)SAMPLER_TYPE::END];

ComPtr<ID3D11Buffer>    m_CB;
D3D11_BUFFER_DESC       m_tDesc;

ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
ComPtr<ID3D11DepthStencilState>	m_arrDS[(UINT)DS_TYPE::END];
ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];
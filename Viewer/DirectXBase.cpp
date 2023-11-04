#include "DirectXBase.h"

#include <string>
#include <vector>
//#include<dxgidebug.h>
//
//DirectXBase* DirectXBase::GetIns()
//{
	//static DirectXBase instance;

//	return &instance;
//}
//
DirectXBase::~DirectXBase()
{
	//pddebug->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);
}
//
void DirectXBase::Initialize(WinApp* winapp)
{
	//でバッグレイヤーのやつ
//#ifdef _DEBUG
//		//ComPtr<ID3D12DebugDevice> pddebug;
//
//	ID3D12Debug1* debugController;
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
//		debugController->EnableDebugLayer();
//		debugController->SetEnableGPUBasedValidation(TRUE);
//	}
//#endif
//
//	//assert(winapp);
	this->winapp = winapp;
	//デバイスの生成
	InitializeDevice();
	//コマンド関連の初期化
	InitializeCommand();
	//スワップチェーン周りの初期化
	InitializeSwapchain();

	//レンダーターゲットビュー当たりの初期化
	InitializeRenderTargetView();
	//深度バッファの初期化
	InitializeDepthBuffer();
	//フェンス生成
	InitializeFence();
}
//デバイスの生成
void DirectXBase::InitializeDevice()
{
	HRESULT result;
	//dxgiファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory));

	//アダプタの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;

	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (size_t i = 0; m_DxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}
	//グラフィックボードのアダプタを列挙2
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];
			break;
		}
	}

	//デバイスの生成
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;
	for (size_t i = 0; i < _countof(levels); i++)
	{
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&m_Device));
		if (result == S_OK)
		{
			featureLevel = levels[i];
			break;
		}
	}
	result = m_Device->QueryInterface(pddebug.GetAddressOf());
}
//
// コマンド関連の初期化
void DirectXBase::InitializeCommand()
{
	HRESULT result;
	result = m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_CmdAllocator));

	//コマンドリストの生成
	result = m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_CmdList));

	//コマンドキューの生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc
	{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	m_Device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_CmdQueue));
}

//スワップチェーンの初期化
void DirectXBase::InitializeSwapchain()
{
	RECT rect;
	GetClientRect(winapp->GetHwnd(), &rect);

	int width = rect.right - rect.left;
	int height =  rect.bottom- rect.top;
	
	//スワップチェーンの生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = width;
	swapchainDesc.Height = height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	m_DxgiFactory->CreateSwapChainForHwnd(m_CmdQueue.Get(), winapp->GetHwnd(), &swapchainDesc, nullptr, nullptr,
		&m_Swapchain1);
	m_Swapchain1.As(&swapchain);
}

// レンダーターゲット周りの初期化
void DirectXBase::InitializeRenderTargetView()
{
	HRESULT result;
	//デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	//レンダーターゲットビュー
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//ダブルバッファ
	heapDesc.NumDescriptors = 2;
	m_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_RtvHeaps));

	//レンダーターゲットビューの生成
	for (int i = 0; i < 2; i++)
	{
		//スワップチェインのバッファー
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&m_Backbuffers[i]));
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			i, m_Device->GetDescriptorHandleIncrementSize(heapDesc.Type));

		m_Device->CreateRenderTargetView(m_Backbuffers[i].Get(), nullptr, handle);
	}
}

// 深度バッファ関連設定
void DirectXBase::InitializeDepthBuffer()
{
	HRESULT result;

	m_DepthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, 780, 500, 1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	//リソース生成
	result = m_Device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
		&m_DepthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_DepthBuffer));

	//深度view用でスクリプタヒープ作成
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap));

	//深度ビュー作成
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_Device->CreateDepthStencilView(m_DepthBuffer.Get(), &dsvDesc, m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
}
//#pragma endregion
//
//#pragma region フェンス生成
void DirectXBase::InitializeFence()
{
	HRESULT result;
	result = m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
}
//#pragma endregion
//
//#pragma region 描画前処理
void DirectXBase::BeginDraw()
{
	//バックバッファの番号取得
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	//リソースバリアを書き込み用に
	m_CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_Backbuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	//クリアコマンド
	//描画先指定
	auto rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeaps->GetCPUDescriptorHandleForHeapStart(),
		bbIndex, m_Device->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	//深度ステンシルビュー用でスクリプタヒープ
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_CmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	///////
	///
	//画面クリア　描画色の指定
	float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	//レンダーターゲット　クリア
	m_CmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファ　クリア
	m_CmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	//viewポート領域の設定

	RECT rect;
	GetClientRect(winapp->GetHwnd(), &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	m_CmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, 780,500));
	//シザー短形の設定
	m_CmdList->RSSetScissorRects(1, &CD3DX12_RECT(static_cast<LONG>(0.0f), static_cast<LONG>(0.0f), static_cast<LONG>(700),
		static_cast<LONG>(500)));
}
#pragma endregion

#pragma region 描画後処理
void DirectXBase::EndDraw()
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	m_CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_Backbuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 命令のクローズ
	m_CmdList->Close();
	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { m_CmdList.Get() }; // コマンドリストの配列
	m_CmdQueue->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ（裏表の入替え）
	swapchain->Present(1, 0);

	// コマンドリストの実行完了を待つ
	m_CmdQueue->Signal(m_Fence.Get(), ++fenceVal);
	if (m_Fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_Fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	
	m_CmdAllocator->Reset(); // キューをクリア
	m_CmdList->Reset(m_CmdAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備
}
#pragma endregion
//
//
//void DirectXBase::ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList)
//{
//	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
//	auto dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
//	// 深度バッファのクリア
//	this->cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
//}
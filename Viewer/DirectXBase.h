#include <wrl/client.h>

#include "WinApp.h"
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;
class DirectXBase
{
public:
	DirectXBase(){};
	~DirectXBase(){};
private:
	WinApp* winapp = nullptr;
	// デバイス関連
	ComPtr<IDXGIFactory6> m_DxgiFactory=nullptr;
	ComPtr<ID3D12Device> m_Device = nullptr;

	// スワップチェイン
	ComPtr<IDXGISwapChain4> m_Swapchain = nullptr;
	ComPtr<IDXGISwapChain1> m_Swapchain2 = nullptr;

	// コマンド関連
	ComPtr<ID3D12CommandAllocator> m_CmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> m_CmdList = nullptr;
	ComPtr<ID3D12CommandQueue> m_CmdQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_RtvHeaps = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_DsvHeap = nullptr;
	// フェンス
	ComPtr<ID3D12Fence> m_Fence = nullptr;
	UINT64 fenceVal = 0;
	//リソース設定
	ComPtr<ID3D12Resource> m_Backbuffers[2]={};
	ComPtr<ID3D12Resource> m_DepthBuffer = nullptr;
	CD3DX12_RESOURCE_DESC m_DepthResDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	//深度view用でスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

private:
	//　各種初期化処理
	//Device
	void InitializeDevice();
	//CommandQueue.CommandAllocator
	void InitializeCommand();
	//Swapchain
	void InitializeSwapchain();
	//RenderTarget
	void InitializeRenderTargetView();
	//
	void InitializeDepthBuffer();
	//Fence
	void InitializeFence();

public:
	//上の初期化まとめたもの
	void Initialize(WinApp* winapp);
	//描画前・後処理
	void BeginDraw();
	void EndDraw();

public:
	//ゲッター
	ID3D12Device* GetDev() { return m_Device.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return m_CmdList.Get(); }
	ID3D12CommandQueue* GetCmdQue() { return m_CmdQueue.Get(); };
};

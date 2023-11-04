#include <wrl/client.h>

#include "WinApp.h"
//#pragma once
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
//
using namespace Microsoft::WRL;
class DirectXBase
{
public:
	~DirectXBase();
//private:
	WinApp* winapp = nullptr;
	ComPtr<ID3D12DebugDevice> pddebug;
	ComPtr<IDXGIFactory6> m_DxgiFactory;
	ID3D12Device* m_Device;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<IDXGISwapChain1> m_Swapchain1;
	ComPtr<ID3D12CommandAllocator> m_CmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_CmdList;
	ComPtr<ID3D12CommandQueue> m_CmdQueue;
	ComPtr<ID3D12DescriptorHeap> m_RtvHeaps;
	ComPtr<ID3D12DescriptorHeap> m_DsvHeap;
	ComPtr<ID3D12Fence> m_Fence;
	ComPtr<ID3D12Resource> m_Backbuffers[2];
	//リソース設定
	ComPtr<ID3D12Resource> m_DepthBuffer;
	CD3DX12_RESOURCE_DESC m_DepthResDesc;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
//	//深度view用でスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
//	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	UINT64 fenceVal = 0;
//	//ウィンドウサイズの指定
//	int window_width = 1900;
//	int window_height = 1020;
//	float wx, wy;
//	//深度テスト用
//public:
//	//各種初期化処理
	void Initialize(WinApp* winapp);
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();
//	//描画前・後処理
	void BeginDraw();
	void EndDraw();
//	//ゲッター
	ID3D12Device* GetDev() { return m_Device; }
	ID3D12GraphicsCommandList* GetCmdList() { return m_CmdList.Get(); }
	ID3D12CommandQueue* GetCmdQue() { return m_CmdQueue.Get(); };
//	//
//public:
//	static ID3D12DescriptorHeap* GetDsvheap();
//	static DirectXBase* GetIns();
//
//public:
//	void ClearDepthBuffer(ID3D12GraphicsCommandList* cmdList);
};

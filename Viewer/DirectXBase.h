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
	// �f�o�C�X�֘A
	ComPtr<IDXGIFactory6> m_DxgiFactory=nullptr;
	ComPtr<ID3D12Device> m_Device = nullptr;

	// �X���b�v�`�F�C��
	ComPtr<IDXGISwapChain4> m_Swapchain = nullptr;
	ComPtr<IDXGISwapChain1> m_Swapchain2 = nullptr;

	// �R�}���h�֘A
	ComPtr<ID3D12CommandAllocator> m_CmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> m_CmdList = nullptr;
	ComPtr<ID3D12CommandQueue> m_CmdQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_RtvHeaps = nullptr;
	ComPtr<ID3D12DescriptorHeap> m_DsvHeap = nullptr;
	// �t�F���X
	ComPtr<ID3D12Fence> m_Fence = nullptr;
	UINT64 fenceVal = 0;
	
	//���\�[�X�ݒ�
	ComPtr<ID3D12Resource> m_Backbuffers[2]={};
	ComPtr<ID3D12Resource> m_DepthBuffer = nullptr;
	CD3DX12_RESOURCE_DESC m_DepthResDesc{};
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	//�[�xview�p�ŃX�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

public:
	//�@�e�평��������
	void Initialize(WinApp* winapp);
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();
	//�`��O�E�㏈��
	void BeginDraw();
	void EndDraw();


public:
	//�Q�b�^�[
	ID3D12Device* GetDev() { return m_Device.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return m_CmdList.Get(); }
	ID3D12CommandQueue* GetCmdQue() { return m_CmdQueue.Get(); };
};

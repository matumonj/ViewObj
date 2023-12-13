#include "DirectXBase.h"

#include <cassert>
#include <string>
#include <vector>


void DirectXBase::Initialize(WinApp* winapp)
{
	this->winapp = winapp;
	if (FAILED(winapp)) {
		assert(0);
	}

	//�f�o�C�X�̐���
	InitializeDevice();
	//�R�}���h�֘A�̏�����
	InitializeCommand();
	//�X���b�v�`�F�[������̏�����
	InitializeSwapchain();
	//�����_�[�^�[�Q�b�g�r���[������̏�����
	InitializeRenderTargetView();
	//�[�x�o�b�t�@�̏�����

	//�t�F���X����
	InitializeFence();
}
//�f�o�C�X�̐���
void DirectXBase::InitializeDevice()
{
	HRESULT result=S_FALSE;
	//dxgi�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory));

	//�A�_�v�^�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter1>> adapters;

	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (size_t i = 0; m_DxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}
	//�O���t�B�b�N�{�[�h�̃A�_�v�^���2
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

	//�f�o�C�X�̐���
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
}

// �R�}���h�֘A�̏�����
void DirectXBase::InitializeCommand()
{
	HRESULT result=S_FALSE;

	//�A���P�[�^����
	result = m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_CmdAllocator));

	//�R�}���h���X�g�̐���
	result = m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_CmdList));

	//�R�}���h�L���[�̐���
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc
	{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};

	m_Device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_CmdQueue));
}

//�X���b�v�`�F�[���̏�����
void DirectXBase::InitializeSwapchain()
{
	RECT rect;
	GetClientRect(winapp->GetHwnd(), &rect);

	int width =WinApp::window_width;
	int height = WinApp::window_height;
	
	//�X���b�v�`�F�[���̐���
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
		&m_Swapchain2);
	m_Swapchain2.As(&m_Swapchain);
}

// �����_�[�^�[�Q�b�g����̏�����
void DirectXBase::InitializeRenderTargetView()
{
	HRESULT result;
	//�f�X�N���v�^�q�[�v�̐���
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	//�����_�[�^�[�Q�b�g�r���[
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	//�_�u���o�b�t�@
	heapDesc.NumDescriptors = 2;
	m_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_RtvHeaps));

	//�����_�[�^�[�Q�b�g�r���[�̐���
	for (int i = 0; i < 2; i++)
	{
		//�X���b�v�`�F�C���̃o�b�t�@�[
		result = m_Swapchain->GetBuffer(i, IID_PPV_ARGS(&m_Backbuffers[i]));
		auto handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RtvHeaps->GetCPUDescriptorHandleForHeapStart(),
			i, m_Device->GetDescriptorHandleIncrementSize(heapDesc.Type));

		m_Device->CreateRenderTargetView(m_Backbuffers[i].Get(), nullptr, handle);
	}
}

// �[�x�o�b�t�@�֘A�ݒ�
void DirectXBase::InitializeDepthBuffer()
{
	HRESULT result;

	m_DepthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, WinApp::window_width, WinApp::window_height, 1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	//���\�[�X����
	result = m_Device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
		&m_DepthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_DepthBuffer));

	//�[�xview�p�ŃX�N���v�^�q�[�v�쐬
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap));

	//�[�x�r���[�쐬
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_Device->CreateDepthStencilView(m_DepthBuffer.Get(), &dsvDesc, m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
}

// �t�F���X����
void DirectXBase::InitializeFence()
{
	HRESULT result=S_FALSE;

	result = m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
}

// �`��O����
void DirectXBase::BeginDraw()
{
	//�o�b�N�o�b�t�@�̔ԍ��擾
	auto bbIdx = m_Swapchain->GetCurrentBackBufferIndex();

	//���\�[�X�o���A�ݒ�
	D3D12_RESOURCE_BARRIER BarrierDesc = {};
	BarrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	BarrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	BarrierDesc.Transition.pResource = m_Backbuffers[bbIdx].Get();
	BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	BarrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	BarrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	m_CmdList->ResourceBarrier(1, &BarrierDesc);

	//�����_�[�^�[�Q�b�g���w��
	auto rtvH = m_RtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIdx * m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_CmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);

	//��ʃN���A�@�`��F�̎w��
	float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	//�����_�[�^�[�Q�b�g�@�N���A
	m_CmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

// �`��㏈��
void DirectXBase::EndDraw()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = m_Swapchain->GetCurrentBackBufferIndex();
	m_CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_Backbuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// ���߂̃N���[�Y
	m_CmdList->Close();
	// �R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { m_CmdList.Get() }; // �R�}���h���X�g�̔z��
	m_CmdQueue->ExecuteCommandLists(1, cmdLists);

	

	// �R�}���h���X�g�̎��s������҂�
	m_CmdQueue->Signal(m_Fence.Get(), ++fenceVal);
	if (m_Fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_Fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	m_CmdAllocator->Reset(); // �L���[���N���A
	m_CmdList->Reset(m_CmdAllocator.Get(), nullptr);
	// �o�b�t�@���t���b�v�i���\�̓��ւ��j
	m_Swapchain->Present(1, 0);
}
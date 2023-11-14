#include "Object3d.h"

#include <cassert>

#include "WinApp.h"

ID3D12Device*Object3d::m_Device = nullptr;
//�ЂƂ܂������
//XMFLOAT3 Object3d::vertices[3] = {};
ComPtr<ID3D12GraphicsCommandList>Object3d::m_CmdList = nullptr;
ComPtr<ID3DBlob>Object3d::m_VsBlob = nullptr;
ComPtr<ID3DBlob>Object3d::m_PsBlob = nullptr;
ComPtr<ID3D12RootSignature>Object3d::m_RootSignature = nullptr;
ComPtr<ID3D12PipelineState>Object3d::pipeline_state = nullptr;
D3D12_VIEWPORT Object3d::viewport = {};
D3D12_RECT Object3d::scissorRect = {};
D3D12_INDEX_BUFFER_VIEW Object3d::ibView={};
D3D12_VERTEX_BUFFER_VIEW Object3d::vertex_buffer_view={};
bool Object3d::SetDevice(ID3D12Device* device)
{
	m_Device = device;
	//�f�o�C�X�̃Z�b�g
	if(m_Device==nullptr)
	{
		return false;
	}

	return true;
}

bool Object3d::SetCommandList(ID3D12GraphicsCommandList* cmdlist)
{
	m_CmdList = cmdlist;
	//�R�}���h���X�g�̃Z�b�g
	if(m_CmdList==nullptr)
	{
		return false;
	}
	return true;
}

//
// ���ʕ����̏�����
//
void Object3d::CommonInit()
{
	
	CreateVBView();
	ShaderSetting();
	CreatePipelineState();

}

//
// ���_�o�b�t�@�r���[�쐬
//
void Object3d::CreateVBView()
{
	HRESULT result = S_FALSE;
	XMFLOAT3 vertices[] = {
	{-0.5f,-0.5f,0.f},
	{-0.5f,+0.5f,0.f},
	{+0.5f,-0.5f,0.f},
	{+0.5f,0.5f,0.f}
	};

	unsigned short indices[] = {
	0,1,2,
	2,1,3
	};
	//�v���p�e�B�ݒ�
	D3D12_HEAP_PROPERTIES heap_properties = {};
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//���\�[�X�f�X�N�ݒ�
	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//���_���
	resource_desc.Width = sizeof(vertices);
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	ID3D12Resource* vertexBuffer = nullptr;

	result = m_Device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	//�߂�l�`�F�b�N
	if (FAILED(result)) {
		assert(0);
	}

	//�o�b�t�@�[�ɒ��_���R�s�[
	XMFLOAT3* vertMap = nullptr;
	result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
	//if (SUCCEEDED(result)) {
		std::copy(std::begin(vertices), std::end(vertices), vertMap);
		vertexBuffer->Unmap(0, nullptr);
	//}

	
	//�o�b�t�@�̉��z�A�h���X
	vertex_buffer_view.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	//���_�̑S�o�C�g��
	vertex_buffer_view.SizeInBytes = sizeof(vertices);
	//1���_�������
	vertex_buffer_view.StrideInBytes = sizeof(XMFLOAT3);

	ID3D12Resource* idxBuff = nullptr;
	//�ݒ�́A�o�b�t�@�̃T�C�Y�ȊO���_�o�b�t�@�̐ݒ���g���܂킵��
	//OK���Ǝv���܂��B
	resource_desc.Width = sizeof(indices);
	result = m_Device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&idxBuff));

	//������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
	unsigned short* mappedIdx = nullptr;
	idxBuff->Map(0, nullptr, (void**)&mappedIdx);
	std::copy(std::begin(indices), std::end(indices), mappedIdx);
	idxBuff->Unmap(0, nullptr);

	
	ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

}

//
// �V�F�[�_����̐ݒ�
//
void Object3d::ShaderSetting()
{
	HRESULT result = S_FALSE;

	//�G���[�p
	ID3DBlob* errorBlob=nullptr;

	/* ���_�V�F�[�_�̓ǂݍ��� */
	result = D3DCompileFromFile(
		L"VertexShader.hlsl",/*�V�F�[�_��*/
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,/*�f�t�H���g*/
		"BasicVS", "vs_5_0",/*�o�[�W������5.0*/
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,/*�œK�����Ȃ��@�f�o�b�O�p*/
		0,
		&m_VsBlob, &errorBlob);

	//�߂�l�`�F�b�N
	if (FAILED(result)) {
		assert(0);
	}

	/* �s�N�Z���V�F�[�_�̓ǂݍ��� */
	result = D3DCompileFromFile(
		L"PixelShader.hlsl",/*�V�F�[�_��*/
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,/*�f�t�H���g*/
		"BasicPS", "ps_5_0",/*�o�[�W������5.0*/
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,/*�œK�����Ȃ��@�f�o�b�O�p*/
		0,
		&m_PsBlob, &errorBlob);

	//�߂�l�`�F�b�N
	if (FAILED(result)) {
		assert(0);
	}

}

void Object3d::CreatePipelineState()
{
	HRESULT result = S_FALSE;

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_pipeline = {};

	//���[�g�V�O�l�`���ݒ�
	CreateRootSignature();
	graphics_pipeline.pRootSignature = m_RootSignature.Get();

	//���_�V�F�[�_
	graphics_pipeline.VS.pShaderBytecode = m_VsBlob->GetBufferPointer();
	graphics_pipeline.VS.BytecodeLength = m_VsBlob->GetBufferSize();

	// �s�N�Z���V�F�[�_
	graphics_pipeline.PS.pShaderBytecode = m_PsBlob->GetBufferPointer();
	graphics_pipeline.PS.BytecodeLength = m_PsBlob->GetBufferSize();

	//�T���v���[�}�X�N�ݒ�
	graphics_pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//���X�^���C�U�ݒ�
	graphics_pipeline.RasterizerState.MultisampleEnable = false;//�A���`�G�C�W���O�p
	graphics_pipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O�Ȃ�
	graphics_pipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�h��Ԃ�
	graphics_pipeline.RasterizerState.DepthClipEnable = true;
	//�u�����h�X�e�[�g�ݒ�
	graphics_pipeline.RasterizerState.FrontCounterClockwise = false;
	graphics_pipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	graphics_pipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	graphics_pipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	graphics_pipeline.RasterizerState.AntialiasedLineEnable = false;
	graphics_pipeline.RasterizerState.ForcedSampleCount = 0;
	graphics_pipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	graphics_pipeline.DepthStencilState.DepthEnable = false;
	graphics_pipeline.DepthStencilState.StencilEnable = false;

	//�����_�[�^�[�Q�b�g�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC render_target_blend = {};
	render_target_blend.BlendEnable = false;
	render_target_blend.LogicOpEnable = false;
	render_target_blend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//�����_�[�^�[�Q�b�g�̃Z�b�g
	graphics_pipeline.BlendState.RenderTarget[0] = render_target_blend;

	//���̓��C�A�E�g�ݒ�
	graphics_pipeline.InputLayout.pInputElementDescs = inputLayout;
	graphics_pipeline.InputLayout.NumElements = _countof(inputLayout);
	//���_�W���̐؂藣��
	graphics_pipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//���͂��Ȃ�
	//�v���~�e�B�u�ݒ�(�O�p�`)
	graphics_pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//�����_�[�^�[�Q�b�g�ݒ�(�}���`�ł��Ȃ��̂łP��)
	graphics_pipeline.NumRenderTargets = 1;
	graphics_pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//�A���`�G�C���A�V���O�ݒ�
	graphics_pipeline.SampleDesc.Count = 1;//1pixel������1��T���v�����O
	//graphics_pipeline.SampleDesc.Quality = 0;//

	// ����
	result = m_Device->CreateGraphicsPipelineState(
		&graphics_pipeline, IID_PPV_ARGS(&pipeline_state));

	if (FAILED(result)) {
		assert(0);
	}
}

//
// ���[�g�V�O�l�`���쐬
//
void Object3d::CreateRootSignature()
{
	HRESULT result = S_FALSE;

	//�G���[�p
	ID3DBlob* errorBlob=nullptr;

	D3D12_ROOT_SIGNATURE_DESC root_signature = {};
	root_signature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//�o�C�i���R�[�h�쐬
	ID3DBlob* rootBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&root_signature,// ���[�g�V�O�l�`��
		D3D_ROOT_SIGNATURE_VERSION_1_0,// ver�w��
		&rootBlob,//�V�F�[�_���
		&errorBlob);

	//�߂�l�`�F�b�N
	if(FAILED(result))
	{
		assert(0);
	}

	//����
	result = m_Device->CreateRootSignature(0, rootBlob->GetBufferPointer(),
		rootBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_RootSignature));

	rootBlob->Release();
	//�߂�l�`�F�b�N
	if (FAILED(result))
	{
		assert(0);
	}
}

//
// �r���[�|�[�g�ݒ�
//
void Object3d::ViewPortsetting()
{
	

	constexpr float window_w = 1280;
	constexpr float window_h = 720;

	//��ʃT�C�Y�E������W
	viewport.Width = window_w;
	viewport.Height = window_h;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	//�[�x�l�̍ő�E�ŏ�
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.f;

	m_CmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));

}

void Object3d::ScissorSetting()
{

	constexpr float window_w = 1280;
	constexpr LONG window_h = 720;

	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = scissorRect.left + window_w;
	scissorRect.bottom = scissorRect.top + window_h;

	m_CmdList->RSSetScissorRects(1, &scissorRect);
}



void Object3d::BeginDraw()
{
	m_CmdList->SetPipelineState(pipeline_state.Get());
	ViewPortsetting();
	ScissorSetting();
	
	
	m_CmdList->SetGraphicsRootSignature(m_RootSignature.Get());
	
	m_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CmdList->IASetVertexBuffers(0, 1, &vertex_buffer_view);
	m_CmdList->IASetIndexBuffer(&ibView);
	m_CmdList->DrawIndexedInstanced(6, 1, 0, 0,0);
}

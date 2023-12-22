#include "PipelineSetting.h"
#include <wrl.h>
#include <d3d12.h>
#include <cassert>
#include <d3dcompiler.h>
PipelineSetting::PipelineSetting()
{
	//���[�g�V�O�l�`���ݒ�ǂݍ���
	m_RootSigSet = std::make_unique<RootSigSetting>();
}
PipelineSetting::~PipelineSetting()
{
	//���[�g�V�O�l�`���j��
	m_RootSigSet.reset();
}


HRESULT PipelineSetting::Setting(ID3D12Device*dev)
{
	HRESULT result = S_FALSE;

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
			//�@���x�N�g��
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
}
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_pipeline = {};

	//���[�g�V�O�l�`���ݒ�
	result = m_RootSigSet->Setting(dev);

	graphics_pipeline.pRootSignature = m_RootSigSet->GetRootSig();

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
	//�f�v�X�X�e���V���X�e�[�g�ݒ�
	graphics_pipeline.DepthStencilState.DepthEnable = true;
	graphics_pipeline.DepthStencilState.StencilEnable = false;
	graphics_pipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	graphics_pipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	graphics_pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
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
	result = dev->CreateGraphicsPipelineState(
		&graphics_pipeline, IID_PPV_ARGS(&pipeline_state));

	return result;
}

HRESULT PipelineSetting::ShaderSetting()
{
	HRESULT result = S_FALSE;

	//�G���[�p
	ID3DBlob* errorBlob = nullptr;

	/* ���_�V�F�[�_�̓ǂݍ��� */
	result = D3DCompileFromFile(
		L"VertexShader.hlsl",/*�V�F�[�_��*/
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,/*�f�t�H���g*/
		"main", "vs_5_0",/*�o�[�W������5.0*/
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

	return result;
}
#include "PipelineSetting.h"
#include <wrl.h>
#include <d3d12.h>
#include <cassert>
#include <d3dcompiler.h>
PipelineSetting::PipelineSetting()
{
	//ルートシグネチャ設定読み込み
	m_RootSigSet = std::make_unique<RootSigSetting>();
}
PipelineSetting::~PipelineSetting()
{
	//ルートシグネチャ破棄
	m_RootSigSet.reset();
}


HRESULT PipelineSetting::Setting(ID3D12Device*dev)
{
	HRESULT result = S_FALSE;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_pipeline = {};

	//ルートシグネチャ設定
	result = m_RootSigSet->Setting(dev);

	graphics_pipeline.pRootSignature = m_RootSigSet->GetRootSig();

	//頂点シェーダ
	graphics_pipeline.VS.pShaderBytecode = m_VsBlob->GetBufferPointer();
	graphics_pipeline.VS.BytecodeLength = m_VsBlob->GetBufferSize();

	// ピクセルシェーダ
	graphics_pipeline.PS.pShaderBytecode = m_PsBlob->GetBufferPointer();
	graphics_pipeline.PS.BytecodeLength = m_PsBlob->GetBufferSize();

	//サンプラーマスク設定
	graphics_pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//ラスタライザ設定
	graphics_pipeline.RasterizerState.MultisampleEnable = false;//アンチエイジング用
	graphics_pipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリングなし
	graphics_pipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//塗りつぶす
	graphics_pipeline.RasterizerState.DepthClipEnable = true;
	//ブレンドステート設定
	graphics_pipeline.RasterizerState.FrontCounterClockwise = false;
	graphics_pipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	graphics_pipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	graphics_pipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	graphics_pipeline.RasterizerState.AntialiasedLineEnable = false;
	graphics_pipeline.RasterizerState.ForcedSampleCount = 0;
	graphics_pipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	//デプスステンシルステート設定
	graphics_pipeline.DepthStencilState.DepthEnable = true;
	graphics_pipeline.DepthStencilState.StencilEnable = false;
	graphics_pipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	graphics_pipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	graphics_pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//レンダーターゲット設定
	D3D12_RENDER_TARGET_BLEND_DESC render_target_blend = {};
	render_target_blend.BlendEnable = false;
	render_target_blend.LogicOpEnable = false;
	render_target_blend.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//レンダーターゲットのセット
	graphics_pipeline.BlendState.RenderTarget[0] = render_target_blend;

	//入力レイアウト設定
	graphics_pipeline.InputLayout.pInputElementDescs = inputLayout;
	graphics_pipeline.InputLayout.NumElements = _countof(inputLayout);
	//頂点集合の切り離し
	graphics_pipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;//今はしない
	//プリミティブ設定(三角形)
	graphics_pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//レンダーターゲット設定(マルチでもないので１つで)
	graphics_pipeline.NumRenderTargets = 1;
	graphics_pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	//アンチエイリアシング設定
	graphics_pipeline.SampleDesc.Count = 1;//1pixelあたり1回サンプリング
	//graphics_pipeline.SampleDesc.Quality = 0;//

	// 生成
	result = dev->CreateGraphicsPipelineState(
		&graphics_pipeline, IID_PPV_ARGS(&pipeline_state));

	return result;
}

HRESULT PipelineSetting::ShaderSetting()
{
	HRESULT result = S_FALSE;

	//エラー用
	ID3DBlob* errorBlob = nullptr;

	/* 頂点シェーダの読み込み */
	result = D3DCompileFromFile(
		L"VertexShader.hlsl",/*シェーダ名*/
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,/*デフォルト*/
		"main", "vs_5_0",/*バージョンは5.0*/
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,/*最適化しない　デバッグ用*/
		0,
		&m_VsBlob, &errorBlob);

	//戻り値チェック
	if (FAILED(result)) {
		assert(0);
	}

	/* ピクセルシェーダの読み込み */
	result = D3DCompileFromFile(
		L"PixelShader.hlsl",/*シェーダ名*/
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,/*デフォルト*/
		"BasicPS", "ps_5_0",/*バージョンは5.0*/
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,/*最適化しない　デバッグ用*/
		0,
		&m_PsBlob, &errorBlob);

	return result;
}
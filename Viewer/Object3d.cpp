#include "Object3d.h"

#include <cassert>

#include "WinApp.h"

ID3D12Device*Object3d::m_Device = nullptr;
//ひとまず一つだけ
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
	//デバイスのセット
	if(m_Device==nullptr)
	{
		return false;
	}

	return true;
}

bool Object3d::SetCommandList(ID3D12GraphicsCommandList* cmdlist)
{
	m_CmdList = cmdlist;
	//コマンドリストのセット
	if(m_CmdList==nullptr)
	{
		return false;
	}
	return true;
}

//
// 共通部分の初期化
//
void Object3d::CommonInit()
{
	
	CreateVBView();
	ShaderSetting();
	CreatePipelineState();

}

//
// 頂点バッファビュー作成
//
void Object3d::CreateVBView()
{
	HRESULT result = S_FALSE;
	XMFLOAT3 vertices[] = {
	{-0.04f,-0.03f,0.f},
	{-0.04f,0.03f,0.f},
	{0.04f,-0.03f,0.f},
	{0.04f,0.03f,0.f}
	};

	unsigned short indices[] = {
	0,1,2,
	2,1,3
	};
	//プロパティ設定
	D3D12_HEAP_PROPERTIES heap_properties = {};
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//リソースデスク設定
	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//頂点情報
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

	//戻り値チェック
	if (FAILED(result)) {
		assert(0);
	}

	//バッファーに頂点情報コピー
	XMFLOAT3* vertMap = nullptr;
	result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
	//if (SUCCEEDED(result)) {
		std::copy(std::begin(vertices), std::end(vertices), vertMap);
		vertexBuffer->Unmap(0, nullptr);
	//}

	
	//バッファの仮想アドレス
	vertex_buffer_view.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	//頂点の全バイト数
	vertex_buffer_view.SizeInBytes = sizeof(vertices);
	//1頂点あたりの
	vertex_buffer_view.StrideInBytes = sizeof(vertices[0]);

	ID3D12Resource* idxBuff = nullptr;
	//設定は、バッファのサイズ以外頂点バッファの設定を使いまわして
	//OKだと思います。
	resource_desc.Width = sizeof(indices);
	result = m_Device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&idxBuff));

	//作ったバッファにインデックスデータをコピー
	unsigned short* mappedIdx = nullptr;
	idxBuff->Map(0, nullptr, (void**)&mappedIdx);
	std::copy(std::begin(indices), std::end(indices), mappedIdx);
	idxBuff->Unmap(0, nullptr);

	
	ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

}

//
// シェーダ周りの設定
//
void Object3d::ShaderSetting()
{
	HRESULT result = S_FALSE;

	//エラー用
	ID3DBlob* errorBlob=nullptr;

	/* 頂点シェーダの読み込み */
	result = D3DCompileFromFile(
		L"VertexShader.hlsl",/*シェーダ名*/
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,/*デフォルト*/
		"BasicVS", "vs_5_0",/*バージョンは5.0*/
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

	//戻り値チェック
	if (FAILED(result)) {
		assert(0);
	}

}

void Object3d::CreatePipelineState()
{
	HRESULT result = S_FALSE;

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_pipeline = {};

	//ルートシグネチャ設定
	CreateRootSignature();
	graphics_pipeline.pRootSignature = m_RootSignature.Get();

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

	graphics_pipeline.DepthStencilState.DepthEnable = false;
	graphics_pipeline.DepthStencilState.StencilEnable = false;

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
	result = m_Device->CreateGraphicsPipelineState(
		&graphics_pipeline, IID_PPV_ARGS(&pipeline_state));

	if (FAILED(result)) {
		assert(0);
	}
}

//
// ルートシグネチャ作成
//
void Object3d::CreateRootSignature()
{
	HRESULT result = S_FALSE;

	//エラー用
	ID3DBlob* errorBlob=nullptr;

	D3D12_ROOT_SIGNATURE_DESC root_signature = {};
	root_signature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//バイナリコード作成
	ID3DBlob* rootBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&root_signature,// ルートシグネチャ
		D3D_ROOT_SIGNATURE_VERSION_1_0,// ver指定
		&rootBlob,//シェーダ情報
		&errorBlob);

	//戻り値チェック
	if(FAILED(result))
	{
		assert(0);
	}

	//生成
	result = m_Device->CreateRootSignature(0, rootBlob->GetBufferPointer(),
		rootBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_RootSignature));

	rootBlob->Release();
	//戻り値チェック
	if (FAILED(result))
	{
		assert(0);
	}
}

//
// ビューポート設定
//
void Object3d::ViewPortsetting()
{
	

	constexpr float window_w = 280;
	constexpr float window_h = 720;

	//画面サイズ・左上座標
	viewport.Width = window_w;
	viewport.Height = window_h;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	//深度値の最大・最小
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.f;

	m_CmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width/2, WinApp::window_height/2));

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
	m_CmdList->DrawInstanced(6, 1, 0, 0);
}

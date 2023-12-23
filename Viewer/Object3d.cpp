#include "Object3d.h"

#include <cassert>

#include "Camera.h"
#include "WinApp.h"

bool Object3d::SetDevice(ID3D12Device* device)
{
	m_Device = device;

	//デバイスのセット(null時)
	if(m_Device==nullptr)
	{
		assert(0);
		return false;
	}

	return true;
}

bool Object3d::SetCommandList(ID3D12GraphicsCommandList* cmdlist)
{
	m_CmdList = cmdlist;

	//コマンドリストのセット(null時)
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
	VertexData v1{ {} ,{} };
	//後にバイナリデータからの読み込みに変える //{{頂点座標},{法線}}
	vertices.emplace_back(VertexData({ { XMFLOAT3(-20.f, -20.5f, 0.f) },{XMFLOAT3(0,0,0)} }));
	vertices.emplace_back(VertexData({ { XMFLOAT3(-20.f, +20.5f, 0.f)},{XMFLOAT3(0,0,0)} }));
	vertices.emplace_back(VertexData({ { XMFLOAT3(20.f, -20.5f, 0.f)},{XMFLOAT3(0,0,0)} }));
	vertices.emplace_back(VertexData({ { XMFLOAT3(20.f, 20.5f, 0.f)},{XMFLOAT3(0,0,0)} }));

	//インデックス(四角形)
	indeces.emplace_back(0);
	indeces.emplace_back(1);
	indeces.emplace_back(2);
	indeces.emplace_back(1);
	indeces.emplace_back(2);
	indeces.emplace_back(3);
	
	//パイプライン設定関数用インス
	m_PipelineSet = std::make_unique<PipelineSetting>();
	//定数バッファにデータ転送
	ConstBufferSetting();
	//頂点・インデックス
	CreateVBView();
	//シェーダ周り
	ShaderSetting();
	//パイプライン
	CreatePipelineState();
}

//
// 更新
//
void Object3d::Update()
{
	HRESULT result = S_FALSE;

	// 定数バッファへのデータ転送
	ConstBuffer* constMap = nullptr;
	result = constBuffer->Map(0, nullptr, (void**)&constMap);
	constMap->Color_ = this->Color_;//色

	
	//ワールド座標求める
	XMMATRIX matWorld, matScale, matRot,matTrans;
	matWorld = XMMatrixIdentity();//単位行列
	//拡大・縮小
	matScale = XMMatrixScaling(Scl_.x, Scl_.y,Scl_.z);
	matWorld *= matScale;
	// 回転
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(XMConvertToRadians(Rot_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(Rot_.y));
	matRot *= XMMatrixRotationZ(XMConvertToRadians(Rot_.z));
	matWorld *= matRot;
	// 平行移動
	matTrans = XMMatrixTranslation(Pos_.x, Pos_.y, Pos_.z);
	matWorld *= matTrans;

	constMap->Mat_ = matWorld*Camera::GetIns()->GetMatViewProj();
	
	constBuffer->Unmap(0, nullptr);
}

//
// 頂点バッファビュー作成
//
void Object3d::CreateVBView()
{
	HRESULT result = S_FALSE;

	//プロパティ設定
	D3D12_HEAP_PROPERTIES heap_properties = {};
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//頂点データのサイズ取得
	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices.size());

	//リソースデスク設定
	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//頂点情報
	resource_desc.Width = sizeVB;
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//頂点バッファ設定
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

	//法線
	for (size_t i = 0; i < vertices.size()/3; i++) {
		//三角形のインデックス抜く
		unsigned short index0 = indeces[i * 3 + 0];
		unsigned short index1 = indeces[i * 3 + 1];
		unsigned short index2 = indeces[i * 3 + 2];

		//三角形の構成頂点抜き出す
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].Pos_);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].Pos_);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].Pos_);

		//2辺のベクトル
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
	
		//外積算出後法線求める
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化
		normal = XMVector3Normalize(normal);
		//頂点データの代入
		XMStoreFloat3(&vertices[index0].Normal_, normal);
		XMStoreFloat3(&vertices[index1].Normal_, normal);
		XMStoreFloat3(&vertices[index2].Normal_, normal);
	}
	//バッファーに頂点情報コピー
	VertexData* vertMap = nullptr;
	result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
	
	//頂点のデータコピー
	for (size_t i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];
	}
	vertexBuffer->Unmap(0, nullptr);
	
	//バッファの仮想アドレス
	vertex_buffer_view.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	//頂点の全バイト数
	vertex_buffer_view.SizeInBytes = sizeVB;
	//1頂点あたりの
	vertex_buffer_view.StrideInBytes = sizeof(VertexData);

	//インデックスバッファ設定
	ID3D12Resource* indexBuffer = nullptr;

	//インデックスのサイズ取得
	UINT sizeIB = static_cast<UINT>(sizeof(int) * indeces.size());
	resource_desc.Width = sizeIB;

	//バッファにデータ転送
	result = m_Device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	//作ったバッファにインデックスデータをコピー
	unsigned short* IndexMap = nullptr;
	indexBuffer->Map(0, nullptr, (void**)&IndexMap);
	std::copy(std::begin(indeces), std::end(indeces), IndexMap);

	indexBuffer->Unmap(0, nullptr);//マップ解除

	//バッファビュー設定
	ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

//
//定数バッファ設定
//
void Object3d::ConstBufferSetting()
{
	HRESULT result = S_FALSE;

	// ヒープ設定
	D3D12_HEAP_PROPERTIES cbheap{};
	cbheap.Type = D3D12_HEAP_TYPE_UPLOAD;
	// リソース設定
	D3D12_RESOURCE_DESC cbresdesc{};
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdesc.Width = (sizeof(ConstBuffer) + 0xff) & ~0xff;
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = m_Device->CreateCommittedResource(
		&cbheap,
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffer));

	// 定数バッファへのデータ転送
	ConstBuffer* constMap = nullptr;
	result = constBuffer->Map(0, nullptr, (void**)&constMap);
	constMap->Color_ = XMFLOAT4(0, 0, 0, 1);//色

	{
		constMap->Mat_ = XMMatrixIdentity();//行列
		//透視投影変換行列
		constMap->Mat_ = Camera::GetIns()->GetMatViewProj();
	}
	constBuffer->Unmap(0, nullptr);

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;//定数バッファの数
	// 生成
	result = m_Device->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&cbvDescHeap));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuffer->GetDesc().Width;
	m_Device->CreateConstantBufferView(
		&cbvDesc, cbvDescHeap->GetCPUDescriptorHandleForHeapStart());
}

//
// シェーダ周りの設定
//
void Object3d::ShaderSetting()
{
	HRESULT result = S_FALSE;

	//シェーダ読み込み
	result = m_PipelineSet->ShaderSetting();

	//戻り値チェック
	if (FAILED(result)) {
		assert(0);
	}

}

void Object3d::CreatePipelineState()
{
	HRESULT result=S_FALSE;
	
	result = m_PipelineSet->Setting(m_Device.Get());
	
	if (FAILED(result)) {
		assert(0);
	}
}


//
// ビューポート設定
//
void Object3d::ViewPortsetting()
{
	//ウィンドウの大きさ
	constexpr float window_w = 980;
	constexpr float window_h = 720;

	//画面サイズ・左上座標
	viewport.Width = window_w;
	viewport.Height = window_h;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	//深度値の最大・最小
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.f;

	m_CmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));

}

void Object3d::ScissorSetting()
{
	//ウィンドウの大きさ
	constexpr LONG window_w = 980;
	constexpr LONG window_h = 720;

	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = scissorRect.left + window_w;
	scissorRect.bottom = scissorRect.top + window_h;

	m_CmdList->RSSetScissorRects(1, &scissorRect);
}



void Object3d::BeginDraw()
{

	m_CmdList->SetPipelineState(m_PipelineSet->GetPipelineState());

	//ビューポート・シザー矩形設定
	ViewPortsetting();
	ScissorSetting();

	//ルーロシグネチャ転送
	m_CmdList->SetGraphicsRootSignature(m_PipelineSet->GetRootSig());

	//デスクリプタヒープ
	ID3D12DescriptorHeap* ppHeap[] = { cbvDescHeap.Get() };
	m_CmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
	//定数バッファビューセット
	m_CmdList->SetGraphicsRootDescriptorTable(0, cbvDescHeap->GetGPUDescriptorHandleForHeapStart());
	m_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CmdList->IASetVertexBuffers(0, 1, &vertex_buffer_view);
	m_CmdList->IASetIndexBuffer(&ibView);
	m_CmdList->DrawIndexedInstanced(indeces.size(), 1, 0, 0, 0);
}

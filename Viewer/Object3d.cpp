#include "Object3d.h"

#include <cassert>

ID3D12Device*Object3d::m_Device = nullptr;
//ひとまず一つだけ
Vector3 Object3d::vertices[3] = {};

ComPtr<ID3DBlob>Object3d::m_VsBlob = nullptr;
ComPtr<ID3DBlob>Object3d::m_PsBlob = nullptr;

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

//
// 共通部分の初期化
//
void Object3d::CommonInit()
{
	CreateVBView();
	ShaderSetting();
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

	//リソースデスク設定
	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//頂点情報
	resource_desc.Width = sizeof(&vertices);
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 0;
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
	assert(result);

	//バッファーに頂点情報コピー
	Vector3* vertMap = nullptr;
	result = vertexBuffer->Map(0, nullptr, (void**)vertMap);
	if (SUCCEEDED(result)) {
		std::copy(std::begin(vertices), std::end(vertices), vertMap);
		vertexBuffer->Unmap(0, nullptr);
	}

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
	//バッファの仮想アドレス
	vertex_buffer_view.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	//頂点の全バイト数
	vertex_buffer_view.SizeInBytes = sizeof(vertices);
	//1頂点あたりの
	vertex_buffer_view.StrideInBytes = sizeof(vertices[0]);
}

void Object3d::ShaderSetting()
{
	
}


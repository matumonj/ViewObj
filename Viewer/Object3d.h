#pragma once
#include <array>

#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include "Vector3.h"
#include<d3dcompiler.h>

#include <DirectXMath.h>

#include "ReadBinary.h"
#pragma comment(lib,"d3dcompiler.lib")
using namespace DirectX;
using namespace Microsoft::WRL;
class Object3d
{
private:
#pragma pack(1)
	struct Header
	{
		unsigned int TriangleSize = 0;
		unsigned int VertexSize = 0;
		unsigned int NormalSize = 0;
	}m_header;
#pragma pack()

	// 定数バッファ
	struct ConstBuffer
	{
		XMFLOAT4 Color_ = { 1,1,1,1 };
		XMMATRIX Mat_;
	};

private:
	//頂点
	std::vector<XMFLOAT3> vertices = {};
	//インデックス
	std::vector<int>indeces = {};

private:
	//デバイス
	ComPtr<ID3D12Device>m_Device = nullptr;
	//コマンドリスト
	ComPtr<ID3D12GraphicsCommandList>m_CmdList = nullptr;

	//シェーダ情報転送用
	ComPtr<ID3DBlob>m_VsBlob = nullptr;//頂点シェーダ
	ComPtr<ID3DBlob>m_PsBlob = nullptr;//ピクセルシェーダ
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature>m_RootSignature = nullptr;
	//パイプラインステート
	ComPtr<ID3D12PipelineState>pipeline_state = nullptr;
	//ビューポート・シザー矩形
	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//ルートパラメータ
	D3D12_ROOT_PARAMETER rootParam = {};

	// 定数バッファ転送用のデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> cbvDescHeap = nullptr;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffer = nullptr;

	struct Triangle
	{
		//座標(x,y,z)三角形を作る頂点
		std::array<Vector3, 3>pos = { Vector3(0.f,0.f,0.f),Vector3(0.f,0.f,0.f),Vector3(0.f,0.f,0.f) };
		//トライアングルデータ[0]～[2]は頂点インデックス番号　[3]は法線インデックス番号
		std::array<unsigned short int, 4> index = { 0,0,0,0 };
		//法線
		Vector3 normal = Vector3(0.f, 0.f, 0.f);
	};

public:
	//初期化
	void CommonInit(ReadBinary* data);

	//シェーダ読み込み
	void ShaderSetting();

	//グラフィックスパイプライン初期化
	void CreatePipelineState();

	//ルートシグネチャ初期化
	void CreateRootSignature();
	void RootParameterSetting();

	//ビューポート・シザー矩形設定
	void ViewPortsetting();
	void ScissorSetting();

	//定数バッファへのデータ転送など
	void ConstBufferSetting();

	//描画処理
	void BeginDraw();
	void EndDraw();

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

public:/* セッター */
	bool SetDevice(ID3D12Device* device);
	bool SetCommandList(ID3D12GraphicsCommandList* cmdlist);
private:
	void CreateVBView();
};


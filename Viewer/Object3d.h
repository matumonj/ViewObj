#pragma once
#include <array>

#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include "Vector3.h"
#include<d3dcompiler.h>

#include <DirectXMath.h>
#pragma comment(lib,"d3dcompiler.lib")
using namespace DirectX;
using namespace Microsoft::WRL;
class Object3d
{
private:
#pragma pack(1)
	struct Header
	{
		unsigned int TriangleSize=0;
		unsigned int VertexSize=0;
		unsigned int NormalSize=0;
	}m_header;
#pragma pack()
	//仮性的メンバ
	static std::vector<XMFLOAT3> vertices;
	static std::vector<unsigned short>indeces;
	static ComPtr<ID3DBlob>m_VsBlob;
	static ComPtr<ID3DBlob>m_PsBlob;
	static ComPtr<ID3D12RootSignature>m_RootSignature;
	static ID3D12Device*m_Device;
	static ComPtr<ID3D12GraphicsCommandList>m_CmdList;
	//パイプラインステート
	static ComPtr<ID3D12PipelineState>pipeline_state;
	static D3D12_VIEWPORT viewport ;
	static D3D12_RECT scissorRect ;
	//頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
	//インデックスバッファビューを作成
	static D3D12_INDEX_BUFFER_VIEW ibView;// = {};
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
	void CreateMesh();
	void LoadModel();

	static void CommonInit();
	static void ShaderSetting();
	static void CreatePipelineState();
	static void CreateRootSignature();
	static void ViewPortsetting();
	static void ScissorSetting();

	static void BeginDraw();
	static void EndDraw();

	void Initialize();
	void Update();
	void Draw();

public:
	static bool SetDevice(ID3D12Device* device);
	static bool SetCommandList(ID3D12GraphicsCommandList* cmdlist);
private:
	static void CreateVBView();
};


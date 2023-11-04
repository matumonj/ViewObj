#pragma once
#include <array>

#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include "Vector3.h"
#include<d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

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
	//�����I�����o
	static Vector3 vertices[3];
	static ComPtr<ID3DBlob>m_VsBlob;
	static ComPtr<ID3DBlob>m_PsBlob;

	static ID3D12Device*m_Device;

	struct Triangle
	{
		//���W(x,y,z)�O�p�`����钸�_
		std::array<Vector3, 3>pos = { Vector3(0.f,0.f,0.f),Vector3(0.f,0.f,0.f),Vector3(0.f,0.f,0.f) };
		//�g���C�A���O���f�[�^[0]�`[2]�͒��_�C���f�b�N�X�ԍ��@[3]�͖@���C���f�b�N�X�ԍ�
		std::array<unsigned short int, 4> index = { 0,0,0,0 };
		//�@��
		Vector3 normal = Vector3(0.f, 0.f, 0.f);
	};


public:
	void CreateMesh();
	void LoadModel();

	static void CommonInit();
	static void ShaderSetting();
	void Initialize();
	void Update();
	void Draw();

public:
	static bool SetDevice(ID3D12Device* device);
private:
	static void CreateVBView();
};


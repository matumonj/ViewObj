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
#include"PipelineSetting.h"
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

	// �萔�o�b�t�@
	struct ConstBuffer
	{
		XMFLOAT4 Color_ = { 1,1,1,1 };// �F���
		XMMATRIX Mat_;// �s��
		XMFLOAT3 Normal_{ 0,0,0 };// �@��
	};
	// �萔�o�b�t�@�]���p�̃f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> cbvDescHeap = nullptr;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer = nullptr;

private:
	//���_�f�[�^
	struct VertexData 
	{
		XMFLOAT3 Pos_ = {};
		XMFLOAT3 Normal_ = {};
	};

	//���_
	std::vector<VertexData> vertices = {};
	//�C���f�b�N�X
	std::vector<int>indeces = {};

private:
	//�f�o�C�X
	ComPtr<ID3D12Device>m_Device = nullptr;
	//�R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList>m_CmdList = nullptr;
	//�r���[�|�[�g�E�V�U�[��`
	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//�p�C�v���C���E���[�g�V�O�l�`�����ǂݍ���
	std::unique_ptr<PipelineSetting>m_PipelineSet=nullptr;
	
	struct Triangle
	{
		//���W(x,y,z)�O�p�`����钸�_
		std::array<Vector3, 3>pos = { Vector3(0.f,0.f,0.f),Vector3(0.f,0.f,0.f),Vector3(0.f,0.f,0.f) };
		//�g���C�A���O���f�[�^[0]�`[2]�͒��_�C���f�b�N�X�ԍ��@[3]�͖@���C���f�b�N�X�ԍ�
		std::array<unsigned short int, 4> index = { 0,0,0,0 };
		//�@��
		Vector3 normal = Vector3(0.f, 0.f, 0.f);
	};

private:
	//��{�p�����[�^
	XMFLOAT3 Pos_ = {0,0,0};
	XMFLOAT3 Scl_ = { 1,1,1 };
	XMFLOAT3 Rot_ = {0,0,0};
	//�F
	XMFLOAT4 Color_ = {1,1,1,1};

public:
	//������
	void CommonInit();

	//�V�F�[�_�ǂݍ���
	void ShaderSetting();

	//�O���t�B�b�N�X�p�C�v���C��������
	void CreatePipelineState();

	//���[�g�V�O�l�`��������
	void CreateRootSignature();
	void RootParameterSetting();

	//�r���[�|�[�g�E�V�U�[��`�ݒ�
	void ViewPortsetting();
	void ScissorSetting();

	//�萔�o�b�t�@�ւ̃f�[�^�]���Ȃ�
	void ConstBufferSetting();
	void CreateVBView();
	//�`�揈��
	void BeginDraw();
	void EndDraw();

public:
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();

public:/* �Z�b�^�[ */
	bool SetDevice(ID3D12Device* device);
	bool SetCommandList(ID3D12GraphicsCommandList* cmdlist);

	void SetPos(Vector3 pos) { Pos_ = { pos.x,pos.y,pos.z }; }
	void SetRot(Vector3 rot) { Rot_ = { rot.x,rot.y,rot.z }; }
	void SetScl(Vector3 scl) { Scl_ = {scl.x,scl.y,scl.z}; }

	void SetColor(XMFLOAT4 color) { Color_ = color; }
};


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

	// �萔�o�b�t�@
	struct ConstBuffer
	{
		XMFLOAT4 Color_ = { 1,1,1,1 };
		XMMATRIX Mat_;
	};

private:
	//���_
	std::vector<XMFLOAT3> vertices = {};
	//�C���f�b�N�X
	std::vector<int>indeces = {};

private:
	//�f�o�C�X
	ComPtr<ID3D12Device>m_Device = nullptr;
	//�R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList>m_CmdList = nullptr;

	//�V�F�[�_���]���p
	ComPtr<ID3DBlob>m_VsBlob = nullptr;//���_�V�F�[�_
	ComPtr<ID3DBlob>m_PsBlob = nullptr;//�s�N�Z���V�F�[�_
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature>m_RootSignature = nullptr;
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState>pipeline_state = nullptr;
	//�r���[�|�[�g�E�V�U�[��`
	D3D12_VIEWPORT viewport = {};
	D3D12_RECT scissorRect = {};
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//���[�g�p�����[�^
	D3D12_ROOT_PARAMETER rootParam = {};

	// �萔�o�b�t�@�]���p�̃f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> cbvDescHeap = nullptr;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffer = nullptr;

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
	//������
	void CommonInit(ReadBinary* data);

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
private:
	void CreateVBView();
};


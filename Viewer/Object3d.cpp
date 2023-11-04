#include "Object3d.h"

#include <cassert>

ID3D12Device*Object3d::m_Device = nullptr;
//�ЂƂ܂������
Vector3 Object3d::vertices[3] = {};

ComPtr<ID3DBlob>Object3d::m_VsBlob = nullptr;
ComPtr<ID3DBlob>Object3d::m_PsBlob = nullptr;

bool Object3d::SetDevice(ID3D12Device* device)
{
	m_Device = device;
	//�f�o�C�X�̃Z�b�g
	if(m_Device==nullptr)
	{
		return false;
	}

	return true;
}

//
// ���ʕ����̏�����
//
void Object3d::CommonInit()
{
	CreateVBView();
	ShaderSetting();
}

//
// ���_�o�b�t�@�r���[�쐬
//
void Object3d::CreateVBView()
{
	HRESULT result = S_FALSE;

	//�v���p�e�B�ݒ�
	D3D12_HEAP_PROPERTIES heap_properties = {};
	heap_properties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	//���\�[�X�f�X�N�ݒ�
	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//���_���
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

	//�߂�l�`�F�b�N
	assert(result);

	//�o�b�t�@�[�ɒ��_���R�s�[
	Vector3* vertMap = nullptr;
	result = vertexBuffer->Map(0, nullptr, (void**)vertMap);
	if (SUCCEEDED(result)) {
		std::copy(std::begin(vertices), std::end(vertices), vertMap);
		vertexBuffer->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
	//�o�b�t�@�̉��z�A�h���X
	vertex_buffer_view.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	//���_�̑S�o�C�g��
	vertex_buffer_view.SizeInBytes = sizeof(vertices);
	//1���_�������
	vertex_buffer_view.StrideInBytes = sizeof(vertices[0]);
}

void Object3d::ShaderSetting()
{
	
}


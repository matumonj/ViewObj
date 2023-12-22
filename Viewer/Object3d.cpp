#include "Object3d.h"

#include <cassert>

#include "Camera.h"
#include "WinApp.h"

bool Object3d::SetDevice(ID3D12Device* device)
{
	m_Device = device;

	//�f�o�C�X�̃Z�b�g(null��)
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

	//�R�}���h���X�g�̃Z�b�g(null��)
	if(m_CmdList==nullptr)
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
	VertexData v1{ {} ,{} };
	//��Ƀo�C�i���f�[�^����̓ǂݍ��݂ɕς��� //{{���_���W},{�@��}}
	vertices.emplace_back(VertexData({ { XMFLOAT3(-20.f, -20.5f, 0.f) },{XMFLOAT3(0,0,0)} }));
	vertices.emplace_back(VertexData({ { XMFLOAT3(-20.f, +20.5f, 0.f)},{XMFLOAT3(0,0,0)} }));
	vertices.emplace_back(VertexData({ { XMFLOAT3(20.f, -20.5f, 0.f)},{XMFLOAT3(0,0,0)} }));
	vertices.emplace_back(VertexData({ { XMFLOAT3(20.f, 20.5f, 0.f)},{XMFLOAT3(0,0,0)} }));

	//�C���f�b�N�X(�l�p�`)
	indeces.emplace_back(0);
	indeces.emplace_back(1);
	indeces.emplace_back(2);
	indeces.emplace_back(1);
	indeces.emplace_back(2);
	indeces.emplace_back(3);
	
	//�p�C�v���C���ݒ�֐��p�C���X
	m_PipelineSet = std::make_unique<PipelineSetting>();
	//�萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferSetting();
	//���_�E�C���f�b�N�X
	CreateVBView();
	//�V�F�[�_����
	ShaderSetting();
	//�p�C�v���C��
	CreatePipelineState();
}

//
// �X�V
//
void Object3d::Update()
{
	HRESULT result = S_FALSE;

	// �萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBuffer* constMap = nullptr;
	result = constBuffer->Map(0, nullptr, (void**)&constMap);
	constMap->Color_ = this->Color_;//�F

	
	//���[���h���W���߂�
	XMMATRIX matWorld, matScale, matRot,matTrans;
	matWorld = XMMatrixIdentity();//�P�ʍs��
	//�g��E�k��
	matScale = XMMatrixScaling(Scl_.x, Scl_.y,Scl_.z);
	matWorld *= matScale;
	// ��]
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationX(XMConvertToRadians(Rot_.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(Rot_.y));
	matRot *= XMMatrixRotationZ(XMConvertToRadians(Rot_.z));
	matWorld *= matRot;
	// ���s�ړ�
	matTrans = XMMatrixTranslation(Pos_.x, Pos_.y, Pos_.z);
	matWorld *= matTrans;

	constMap->Mat_ = matWorld*Camera::GetIns()->GetMatViewProj();
	
	constBuffer->Unmap(0, nullptr);
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

	//���_�f�[�^�̃T�C�Y�擾
	UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertices.size());

	//���\�[�X�f�X�N�ݒ�
	D3D12_RESOURCE_DESC resource_desc = {};
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//���_���
	resource_desc.Width = sizeVB;
	resource_desc.Height = 1;
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.Format = DXGI_FORMAT_UNKNOWN;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;


	//���_�o�b�t�@�ݒ�
	ID3D12Resource* vertexBuffer = nullptr;

	result = m_Device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	//�߂�l�`�F�b�N
	if (FAILED(result)) {
		assert(0);
	}

	//�@��
	for (size_t i = 0; i < vertices.size()/3; i++) {
		//�O�p�`�̃C���f�b�N�X����
		unsigned short index0 = indeces[i * 3 + 0];
		unsigned short index1 = indeces[i * 3 + 1];
		unsigned short index2 = indeces[i * 3 + 2];

		//�O�p�`�̍\�����_�����o��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].Pos_);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].Pos_);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].Pos_);

		//2�ӂ̃x�N�g��
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
	
		//�O�ώZ�o��@�����߂�
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K��
		normal = XMVector3Normalize(normal);
		//���_�f�[�^�̑��
		XMStoreFloat3(&vertices[index0].Normal_, normal);
		XMStoreFloat3(&vertices[index1].Normal_, normal);
		XMStoreFloat3(&vertices[index2].Normal_, normal);

	}
	//�o�b�t�@�[�ɒ��_���R�s�[
	VertexData* vertMap = nullptr;
	result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
	//if (SUCCEEDED(result)) {
	for (size_t i = 0; i < vertices.size(); i++) {
		vertMap[i] = vertices[i];
	}
		vertexBuffer->Unmap(0, nullptr);
	//}
//�o�b�t�@�̉��z�A�h���X
	vertex_buffer_view.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	//���_�̑S�o�C�g��
	vertex_buffer_view.SizeInBytes = sizeVB;
	//1���_�������
	vertex_buffer_view.StrideInBytes = sizeof(VertexData);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	ID3D12Resource* indexBuffer = nullptr;

	//�C���f�b�N�X�̃T�C�Y�擾
	UINT sizeIB = static_cast<UINT>(sizeof(int) * indeces.size());
	resource_desc.Width = sizeIB;

	//�o�b�t�@�Ƀf�[�^�]��
	result = m_Device->CreateCommittedResource(
		&heap_properties,
		D3D12_HEAP_FLAG_NONE,
		&resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	//������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
	unsigned short* IndexMap = nullptr;
	indexBuffer->Map(0, nullptr, (void**)&IndexMap);
	std::copy(std::begin(indeces), std::end(indeces), IndexMap);

	indexBuffer->Unmap(0, nullptr);//�}�b�v����

	//�o�b�t�@�r���[�ݒ�
	ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

}

//
//�萔�o�b�t�@�ݒ�
//
void Object3d::ConstBufferSetting()
{
	HRESULT result = S_FALSE;

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbheap{};
	cbheap.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
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

	// �萔�o�b�t�@�ւ̃f�[�^�]��
	ConstBuffer* constMap = nullptr;
	result = constBuffer->Map(0, nullptr, (void**)&constMap);
	constMap->Color_ = XMFLOAT4(0, 0, 0, 1);//�F

	{
		constMap->Mat_ = XMMatrixIdentity();//�s��
		//�������e�ϊ��s��
		constMap->Mat_ = Camera::GetIns()->GetMatViewProj();
	}
	constBuffer->Unmap(0, nullptr);

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;//�萔�o�b�t�@�̐�
	// ����
	result = m_Device->CreateDescriptorHeap(
		&descHeapDesc, IID_PPV_ARGS(&cbvDescHeap));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuffer->GetDesc().Width;
	m_Device->CreateConstantBufferView(
		&cbvDesc, cbvDescHeap->GetCPUDescriptorHandleForHeapStart());
}

//
// �V�F�[�_����̐ݒ�
//
void Object3d::ShaderSetting()
{
	HRESULT result = S_FALSE;

	//�V�F�[�_�ǂݍ���
	result = m_PipelineSet->ShaderSetting();

	//�߂�l�`�F�b�N
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
// �r���[�|�[�g�ݒ�
//
void Object3d::ViewPortsetting()
{
	//�E�B���h�E�̑傫��
	constexpr float window_w = 980;
	constexpr float window_h = 720;

	//��ʃT�C�Y�E������W
	viewport.Width = window_w;
	viewport.Height = window_h;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	//�[�x�l�̍ő�E�ŏ�
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.f;

	m_CmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));

}

void Object3d::ScissorSetting()
{
	//�E�B���h�E�̑傫��
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

	//�r���[�|�[�g�E�V�U�[��`�ݒ�
	ViewPortsetting();
	ScissorSetting();

	//���[���V�O�l�`���]��
	m_CmdList->SetGraphicsRootSignature(m_PipelineSet->GetRootSig());

	//�f�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* ppHeap[] = { cbvDescHeap.Get() };
	m_CmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
	//�萔�o�b�t�@�r���[�Z�b�g
	m_CmdList->SetGraphicsRootDescriptorTable(0, cbvDescHeap->GetGPUDescriptorHandleForHeapStart());
	m_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_CmdList->IASetVertexBuffers(0, 1, &vertex_buffer_view);
	m_CmdList->IASetIndexBuffer(&ibView);
	m_CmdList->DrawIndexedInstanced(indeces.size(), 1, 0, 0, 0);
}

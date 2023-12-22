#include "RootSigSetting.h"

HRESULT RootSigSetting::Setting(ID3D12Device* dev)
{
	HRESULT result = S_FALSE;
	//�G���[�p
	ID3DBlob* errorBlob = nullptr;

	//�f�X�N���v�^�e�[�u��
	D3D12_DESCRIPTOR_RANGE descRange{};
	descRange.NumDescriptors = 1;
	descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descRange.BaseShaderRegister = 0;
	descRange.OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam.DescriptorTable.pDescriptorRanges = &descRange;
	rootParam.DescriptorTable.NumDescriptorRanges = 1;
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//���[�g�V�O�l�`��
	D3D12_ROOT_SIGNATURE_DESC root_signature = {};
	root_signature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	root_signature.pParameters = &rootParam;//���[�g�p�����[�^�̐擪�A�h���X
	root_signature.NumParameters = 1;

	//�o�C�i���R�[�h�쐬
	ID3DBlob* rootBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&root_signature,// ���[�g�V�O�l�`��
		D3D_ROOT_SIGNATURE_VERSION_1_0,// ver�w��
		&rootBlob,//�V�F�[�_���
		&errorBlob);

	//����
	result = dev->CreateRootSignature(0, rootBlob->GetBufferPointer(),
		rootBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_RootSignature));

	rootBlob->Release();
	
	return result;
}
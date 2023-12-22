#pragma once
#include<wrl.h>
#include"RootSigSetting.h"
#include<memory>
using namespace Microsoft::WRL;

//�I�u�W�F�N�g�N���X�����Ȃ�̂ŕ���
class PipelineSetting
{
public:
	PipelineSetting();
	~PipelineSetting();
private:
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState>pipeline_state = nullptr;
	//�V�F�[�_���]���p
	ComPtr<ID3DBlob>m_VsBlob = nullptr;//���_�V�F�[�_
	ComPtr<ID3DBlob>m_PsBlob = nullptr;//�s�N�Z���V�F�[�_

	//�r�����[�g�V�O�l�`���̐�������
	std::unique_ptr<RootSigSetting>m_RootSigSet = nullptr;

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dev">DirectBase�̃f�o�C�X</param>
	/// <returns>�p�C�v���C�����ǂݍ��݌��result</returns>
	HRESULT Setting(ID3D12Device*dev);
	
	/// <summary>
	/// �V�F-�_�ݒ�
	/// </summary>
	/// <returns>�V�F�[�_�ǂݍ��݌��result</returns>
	HRESULT ShaderSetting();
	
public:
	//�Q�b�^�[
	// 
	//�V�F�[�_�p
	ID3DBlob* GetVsBlob()const { return m_VsBlob.Get(); }
	ID3DBlob* GetPsBlob()const { return m_PsBlob.Get(); }
	
	//�p�C�v���C��
	ID3D12PipelineState* GetPipelineState()const { return pipeline_state.Get(); }
	//���[�g�V�O�l�`��
	ID3D12RootSignature* GetRootSig()const { return m_RootSigSet->GetRootSig(); }
};


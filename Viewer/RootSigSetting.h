#pragma once
#include<d3d12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

//�I�u�W�F�N�g�N���X�����Ȃ�̂ŕ���
class RootSigSetting
{
private:
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature>m_RootSignature = nullptr;
	//���[�g�p�����[�^
	D3D12_ROOT_PARAMETER rootParam = {};
public:
	/// <summary>
	/// ���[�g�p�����[�^�E�V�O�l�`���ݒ�
	/// </summary>
	/// <param name="dev">DirectBase��Device</param>
	/// <returns>�V�O�l�`���E�p�����[�^�ݒ���result</returns>
	HRESULT Setting(ID3D12Device*dev);
	
	//�Q�b�^�[
	ID3D12RootSignature* GetRootSig()const { return m_RootSignature.Get(); }
};


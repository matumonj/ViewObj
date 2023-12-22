#include "RootSigSetting.h"

HRESULT RootSigSetting::Setting(ID3D12Device* dev)
{
	HRESULT result = S_FALSE;
	//エラー用
	ID3DBlob* errorBlob = nullptr;

	//デスクリプタテーブル
	D3D12_DESCRIPTOR_RANGE descRange{};
	descRange.NumDescriptors = 1;
	descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descRange.BaseShaderRegister = 0;
	descRange.OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ルートパラメータ
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam.DescriptorTable.pDescriptorRanges = &descRange;
	rootParam.DescriptorTable.NumDescriptorRanges = 1;
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//ルートシグネチャ
	D3D12_ROOT_SIGNATURE_DESC root_signature = {};
	root_signature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	root_signature.pParameters = &rootParam;//ルートパラメータの先頭アドレス
	root_signature.NumParameters = 1;

	//バイナリコード作成
	ID3DBlob* rootBlob = nullptr;
	result = D3D12SerializeRootSignature(
		&root_signature,// ルートシグネチャ
		D3D_ROOT_SIGNATURE_VERSION_1_0,// ver指定
		&rootBlob,//シェーダ情報
		&errorBlob);

	//生成
	result = dev->CreateRootSignature(0, rootBlob->GetBufferPointer(),
		rootBlob->GetBufferSize(),
		IID_PPV_ARGS(&m_RootSignature));

	rootBlob->Release();
	
	return result;
}
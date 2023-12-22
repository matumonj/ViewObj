#pragma once
#include<wrl.h>
#include"RootSigSetting.h"
#include<memory>
using namespace Microsoft::WRL;

//オブジェクトクラス長くなるので分離
class PipelineSetting
{
public:
	PipelineSetting();
	~PipelineSetting();
private:
	//パイプラインステート
	ComPtr<ID3D12PipelineState>pipeline_state = nullptr;
	//シェーダ情報転送用
	ComPtr<ID3DBlob>m_VsBlob = nullptr;//頂点シェーダ
	ComPtr<ID3DBlob>m_PsBlob = nullptr;//ピクセルシェーダ

	//途中ルートシグネチャの生成挟む
	std::unique_ptr<RootSigSetting>m_RootSigSet = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dev">DirectBaseのデバイス</param>
	/// <returns>パイプライン情報読み込み後のresult</returns>
	HRESULT Setting(ID3D12Device*dev);
	
	/// <summary>
	/// シェ-ダ設定
	/// </summary>
	/// <returns>シェーダ読み込み後のresult</returns>
	HRESULT ShaderSetting();
	
public:
	//ゲッター
	// 
	//シェーダ用
	ID3DBlob* GetVsBlob()const { return m_VsBlob.Get(); }
	ID3DBlob* GetPsBlob()const { return m_PsBlob.Get(); }
	
	//パイプライン
	ID3D12PipelineState* GetPipelineState()const { return pipeline_state.Get(); }
	//ルートシグネチャ
	ID3D12RootSignature* GetRootSig()const { return m_RootSigSet->GetRootSig(); }
};


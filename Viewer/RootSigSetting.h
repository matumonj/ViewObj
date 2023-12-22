#pragma once
#include<d3d12.h>
#include <wrl.h>
using namespace Microsoft::WRL;

//オブジェクトクラス長くなるので分離
class RootSigSetting
{
private:
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature>m_RootSignature = nullptr;
	//ルートパラメータ
	D3D12_ROOT_PARAMETER rootParam = {};
public:
	/// <summary>
	/// ルートパラメータ・シグネチャ設定
	/// </summary>
	/// <param name="dev">DirectBaseのDevice</param>
	/// <returns>シグネチャ・パラメータ設定後のresult</returns>
	HRESULT Setting(ID3D12Device*dev);
	
	//ゲッター
	ID3D12RootSignature* GetRootSig()const { return m_RootSignature.Get(); }
};


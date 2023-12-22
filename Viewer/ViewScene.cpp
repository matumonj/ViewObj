#include "ViewScene.h"

#include "Camera.h"
#include"Object3d.h"
void ViewScene::Initialize(DirectXBase*dxbase)
{
	//バイナリの読み込み
	m_binary = std::make_unique<ReadBinary>();
	m_binary->ReadFile("sample.btm");

	//オブジェクト生成(頂点、インデックスはバイナリから引数として読み込む)
	m_Object.reset(new Object3d());
	if (m_Object->SetDevice(dxbase->GetDev()) && m_Object->SetCommandList(dxbase->GetCmdList())) {
		m_Object->CommonInit();//デバイスとコマンドリストがnull出ない時生成
	} else { assert(0); }

}

void ViewScene::Update()
{
	//カメラ更新
	Camera::GetIns()->Update();
	//オブジェクト更新
	if (m_Object) {
		m_Object->SetScl(Vector3(1, 1, 1));
		m_Object->Update();
	}
}

void ViewScene::Draw(DirectXBase*dxbase)
{
	if(!dxbase)
	{
		assert(0);
		return;
	}

	//オブジェクト描画
	dxbase->BeginDraw();
	if (m_Object) {
		m_Object->BeginDraw();
	}
	dxbase->EndDraw();
	
}


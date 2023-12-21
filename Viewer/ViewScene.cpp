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
		m_Object->CommonInit(m_binary.get());
	} else { assert(0); }

}

void ViewScene::Update()
{
	Camera::GetIns()->Update();
	m_Object->Update();
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
	m_Object->BeginDraw();
	dxbase->EndDraw();
	
}


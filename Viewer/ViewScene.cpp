#include "ViewScene.h"
#include"Object3d.h"
void ViewScene::Initialize()
{
	//バイナリの読み込み
	m_binary = std::make_unique<ReadBinary>();
	m_binary->ReadFile("sample.btm");
}

void ViewScene::Update()
{
	
}

void ViewScene::Draw(DirectXBase*dxbase)
{
	dxbase->BeginDraw();
	Object3d::BeginDraw();
	dxbase->EndDraw();
	
}


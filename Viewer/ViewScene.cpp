#include "ViewScene.h"

void ViewScene::Initialize()
{
	//ƒoƒCƒiƒŠ‚Ì“Ç‚İ‚İ
	m_binary = std::make_unique<ReadBinary>();
	m_binary->ReadFile("sample.btm");
}

void ViewScene::Update()
{
	
}

void ViewScene::Draw(DirectXBase*dxbase)
{
	dxbase->BeginDraw();

	dxbase->EndDraw();
}


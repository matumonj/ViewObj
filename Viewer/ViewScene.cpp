#include "ViewScene.h"

void ViewScene::Initialize()
{
	//�o�C�i���̓ǂݍ���
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


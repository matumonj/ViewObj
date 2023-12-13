#include "ViewScene.h"
#include"Object3d.h"
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
	if(!dxbase)
	{
		assert(0);
		return;
	}

	//�I�u�W�F�N�g�`��
	dxbase->BeginDraw();
	Object3d::BeginDraw();
	dxbase->EndDraw();
	
}


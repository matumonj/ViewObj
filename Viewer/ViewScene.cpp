#include "ViewScene.h"

#include "Camera.h"
#include"Object3d.h"
void ViewScene::Initialize(DirectXBase*dxbase)
{
	//�o�C�i���̓ǂݍ���
	m_binary = std::make_unique<ReadBinary>();
	m_binary->ReadFile("sample.btm");

	//�I�u�W�F�N�g����(���_�A�C���f�b�N�X�̓o�C�i����������Ƃ��ēǂݍ���)
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

	//�I�u�W�F�N�g�`��
	dxbase->BeginDraw();
	m_Object->BeginDraw();
	dxbase->EndDraw();
	
}


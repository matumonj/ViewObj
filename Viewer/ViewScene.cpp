#include "ViewScene.h"

#include "Camera.h"
#include"Object3d.h"
ViewScene::~ViewScene()
{
	//�e�C���X�^���X�폜
	m_binary.reset(nullptr);
	m_Object.reset(nullptr);
}


void ViewScene::Initialize(DirectXBase*dxbase)
{
	//�o�C�i���̓ǂݍ���
	m_binary = std::make_unique<ReadBinary>();
	m_binary->ReadFile("sample.btm");

	//�I�u�W�F�N�g����(���_�A�C���f�b�N�X�̓o�C�i����������Ƃ��ēǂݍ���)
	m_Object.reset(new Object3d());
	if (m_Object->SetDevice(dxbase->GetDev()) && m_Object->SetCommandList(dxbase->GetCmdList())) {
		m_Object->CommonInit(m_binary.get());//�f�o�C�X�ƃR�}���h���X�g��null�o�Ȃ�������
	} else { assert(0); }

}

void ViewScene::Update()
{
	//�J�����X�V
	Camera::GetIns()->Update();

	roty++;
	//�I�u�W�F�N�g�X�V
	if (m_Object) {
		m_Object->SetRot({ roty,0,0 });
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

	//�I�u�W�F�N�g�`��
	dxbase->BeginDraw();
	if (m_Object) {
		m_Object->BeginDraw();
	}
	dxbase->EndDraw();
	
}


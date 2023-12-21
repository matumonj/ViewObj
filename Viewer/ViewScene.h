#pragma once
#include<memory>
#include"ReadBinary.h"
#include"DirectXBase.h"
#include "Object3d.h"

class ViewScene
{
public:
	ViewScene(){};
	~ViewScene(){};
public:
	// ������
	void Initialize(DirectXBase* dxbase);
	// �X�V
	void Update();
	// �`��
	void Draw(DirectXBase*dxbase);

private:
	//�ǂݍ��݃t�@�C��
	std::unique_ptr<ReadBinary>m_binary=nullptr;

	std::unique_ptr<Object3d>m_Object = nullptr;
};


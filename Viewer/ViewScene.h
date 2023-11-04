#pragma once
#include<memory>
#include"ReadBinary.h"
#include"DirectXBase.h"
class ViewScene
{
public:
	ViewScene(){};
	~ViewScene(){};
public:
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw(DirectXBase*dxbase);

private:
	//�ǂݍ��݃t�@�C��
	std::unique_ptr<ReadBinary>m_binary=nullptr;
};


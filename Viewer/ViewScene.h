#pragma once
#include<memory>
#include"ReadBinary.h"
#include"DirectXBase.h"
#include "Object3d.h"

class ViewScene
{
public:
	ViewScene(){};
	~ViewScene();
public:
	// 初期化
	void Initialize(DirectXBase* dxbase);
	// 更新
	void Update();
	// 描画
	void Draw(DirectXBase*dxbase);

private:
	//読み込みファイル
	std::unique_ptr<ReadBinary>m_binary=nullptr;

	std::unique_ptr<Object3d>m_Object = nullptr;
	float roty;
};


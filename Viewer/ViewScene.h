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
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw(DirectXBase*dxbase);

private:
	//読み込みファイル
	std::unique_ptr<ReadBinary>m_binary=nullptr;
};


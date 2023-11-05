#include <cassert>
#include <windows.h>
#include <chrono>
#include"ReadBinary.h"
#include "WinApp.h"
#include <wrl.h>
#include"ViewScene.h"
#include"Object3d.h"
using namespace Microsoft::WRL;

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t)
{
	WinApp* winApp = nullptr;
	ReadBinary* R_bin;

	DirectXBase* dxbase;

	ViewScene* scene;
	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX初期化
	dxbase = new DirectXBase();
	dxbase->Initialize(winApp);

	if (Object3d::SetDevice(dxbase->GetDev()) && Object3d::SetCommandList(dxbase->GetCmdList())) {
		Object3d::CommonInit();
	}
	else { assert(0); }
	//描画シーンの初期化
	scene = new ViewScene();
	scene->Initialize();
	//シーンの初期化
	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		//シーンの更新
		scene->Update();
		//シーン描画
		scene->Draw(dxbase);
	}
	//delete dxbase;
	//delete scene;
	delete winApp;
}

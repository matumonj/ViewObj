#include <cassert>
#include <windows.h>
#include <chrono>
#include"ReadBinary.h"
#include "WinApp.h"
#include <wrl.h>

#include "Camera.h"
#include"ViewScene.h"
#include"Object3d.h"
using namespace Microsoft::WRL;

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t)
{
	//WindowsAPIの初期化
	WinApp* winApp = nullptr;
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX初期化
	DirectXBase* dxbase;
	dxbase = new DirectXBase();
	dxbase->Initialize(winApp);

	//カメラパラメータ初期化
	Camera::GetIns()->ParamInit();

	//描画シーンの初期化
	ViewScene* scene=nullptr;
	scene = new ViewScene();
	scene->Initialize(dxbase);

	//シーンの初期化
	while (true)  // ゲームループ
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		if (!scene) {
			assert(0);
			break;
		}

		// シーンの更新
		scene->Update();
		// シーン描画
		scene->Draw(dxbase);
	}

	// 破棄処理
	delete dxbase;
	delete scene;
	delete winApp;
}

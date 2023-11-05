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
	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX������
	dxbase = new DirectXBase();
	dxbase->Initialize(winApp);

	if (Object3d::SetDevice(dxbase->GetDev()) && Object3d::SetCommandList(dxbase->GetCmdList())) {
		Object3d::CommonInit();
	}
	else { assert(0); }
	//�`��V�[���̏�����
	scene = new ViewScene();
	scene->Initialize();
	//�V�[���̏�����
	while (true)  // �Q�[�����[�v
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		//�V�[���̍X�V
		scene->Update();
		//�V�[���`��
		scene->Draw(dxbase);
	}
	//delete dxbase;
	//delete scene;
	delete winApp;
}

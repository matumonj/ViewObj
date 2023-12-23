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
	//WindowsAPI�̏�����
	WinApp* winApp = nullptr;
	winApp = new WinApp();
	winApp->Initialize();

	//DirectX������
	DirectXBase* dxbase;
	dxbase = new DirectXBase();
	dxbase->Initialize(winApp);

	//�J�����p�����[�^������
	Camera::GetIns()->ParamInit();

	//�`��V�[���̏�����
	ViewScene* scene=nullptr;
	scene = new ViewScene();
	scene->Initialize(dxbase);

	//�V�[���̏�����
	while (true)  // �Q�[�����[�v
	{
		if (winApp->ProcessMessage()) {
			break;
		}

		if (!scene) {
			assert(0);
			break;
		}

		// �V�[���̍X�V
		scene->Update();
		// �V�[���`��
		scene->Draw(dxbase);
	}

	// �j������
	delete dxbase;
	delete scene;
	delete winApp;
}

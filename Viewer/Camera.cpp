#include "Camera.h"

#include "WinApp.h"

//カメラのインスタンス
Camera* Camera::GetIns()
{
	static Camera ins;
	return &ins;
}

void Camera::ParamInit()
{
	eye = XMFLOAT3(0, 0, -100);
	target = XMFLOAT3(0, 0, 0);
	upvec = XMFLOAT3(0, 1, 0);

	//射影変換行列作成
	matProj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.f),
		(float)(WinApp::window_width / WinApp::window_height),
		0.1f, 1000.f);
}

void Camera::Update()
{
	//カメラ回転
	angle += XMConvertToRadians(1.f);

	eye.x = -100 * sinf(angle);
	eye.z = -100 * cosf(angle);

	//ビュー行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&upvec));

}
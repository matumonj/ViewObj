#include "Camera.h"

#include "WinApp.h"

//�J�����̃C���X�^���X
Camera* Camera::GetIns()
{
	static Camera ins;
	return &ins;
}

void Camera::ParamInit()
{
	eye = XMFLOAT3(0, 0, -500);
	target = XMFLOAT3(0, 0, 0);
	upvec = XMFLOAT3(0, 1, 0);

	//�ˉe�ϊ��s��쐬
	matProj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.f),
		(float)(WinApp::window_width / WinApp::window_height),
		0.1f, 1000.f);
}

void Camera::Update()
{
	//�J������]
	angle += XMConvertToRadians(1.f);

	//eye.x = -800 * sinf(angle);
	//eye.z = -800 * cosf(angle);

	//�r���[�s��쐬
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&upvec));

}
#include "Camera.h"

void Camera::ParamInit()
{
	eye = XMFLOAT3(0, 0, -100);
	target = XMFLOAT3(0, 0, 0);
	upvec = XMFLOAT3(0, 1, 0);
}

void Camera::Update()
{
	//ビュー行列作成
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&upvec));

	//射影変換行列作成
	matProj = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.f),
		(float)(1280 / 720),
		0.1f, 1000.f);

	matViewProj = matView * matProj;
}
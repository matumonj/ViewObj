#pragma once
#include<DirectXMath.h>
#include"Vector3.h"
using namespace DirectX;
class Camera
{
private:
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 upvec;
	
private:
	void ParamInit();
	void Update();
	float dis;

	XMMATRIX matView,matProj;
	XMMATRIX matViewProj;

	XMMATRIX GetMatView;
	XMMATRIX GetMatProj;
	XMMATRIX GetMatViewProj;


};


#pragma once
#include<DirectXMath.h>
#include"Vector3.h"
using namespace DirectX;
class Camera
{
	public:
		static Camera* GetIns();
private:
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 upvec;

public:
	void ParamInit();
	void Update();

private:
	float angle,dis;
	XMMATRIX matView,matProj;
	XMMATRIX matViewProj;

public:
	XMMATRIX GetMatView()const { return matView; }
	XMMATRIX GetMatProj()const { return matProj; }
	XMMATRIX GetMatViewProj()const{return matView * matProj;}

};


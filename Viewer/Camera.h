#pragma once
#include<DirectXMath.h>
#include"Vector3.h"
using namespace DirectX;
class Camera
{
private:
	Vector3 eye;
	Vector3 target;
	float dis;

	XMMATRIX GetMatView;
	XMMATRIX GetMatProj;


};


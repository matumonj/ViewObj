#include "Vector3.h"

using namespace DirectX;

//コンストラクタ
Vector3::Vector3() : Vector3(0, 0, 0)
{

}

Vector3::Vector3(float x, float y, float z) : XMFLOAT3{ x, y, z }
{

}

Vector3::Vector3(const XMFLOAT3& vec) : XMFLOAT3{ x, y, z } {
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}

Vector3::Vector3(const XMVECTOR& vec) : XMFLOAT3{ x, y, z } {
	XMStoreFloat3(this, vec);
}

//長さを求める
float Vector3::length() const
{
	return (float)sqrtf(x * x + y * y + z * z);
}

//正規化
Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

//内積を求める
float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

//外積を求める
Vector3 Vector3::cross(const Vector3& v) const
{
	Vector3 temp;
	temp.x = this->y * v.z - this->z * v.y;
	temp.y = this->z * v.x - this->x * v.z;
	temp.z = this->x * v.y - this->y * v.x;
	return temp;
}

//単項演算子オーバーロード
Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

//代入演算子オーバーロード
Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3& Vector3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//2項演算子オーバーロード
const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator*(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp *= v2;
}

const Vector3 operator/(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp /= v2;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}


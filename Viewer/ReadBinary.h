#pragma once
#include <array>
#include <DirectXMath.h>
#include <vector>

#include "Vector3.h"

class ReadBinary
{
private:
	//Header部分構造体
#pragma pack(1)
	struct Header
	{
		//三角面の数(tryangle構造体の数)
		unsigned short TriangleSize = 0;
		//頂点数
		unsigned short VertexSize = 0;
		//法線数
		unsigned short NormalSize = 0;
	}m_header;

#pragma pack()
	// とらいあんぐるリスト構造体
	struct TriangleData
	{
		std::array<int, 4>index;
	};
	// トライアングル
	std::vector<TriangleData>m_triangledata;
	// 法線
	std::vector<Vector3>m_Normals;
	// 頂点情報
	std::vector<Vector3>m_Vertexs;
	//オブジェクトに送るインデックス情報
	std::vector<int>m_SendIndexData;
	//オブジェクトに送る頂点情報
	std::vector<DirectX::XMFLOAT3>m_SendVerticesData;
	//
	std::vector<DirectX::XMFLOAT3>m_SendNormalData;
public:
	// 読み込み関数
	void ReadFile(const char* filename);

	std::vector<int>GetIndexData()const { return m_SendIndexData; }

	std::vector<DirectX::XMFLOAT3>GetVerticesData()const { return m_SendVerticesData; }

	std::vector<DirectX::XMFLOAT3>GetNormalData()const { return m_SendNormalData; }
};


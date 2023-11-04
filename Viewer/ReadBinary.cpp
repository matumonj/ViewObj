#include "ReadBinary.h"
#include	<iostream>
#include	<sstream>
#include	<fstream>

#include "WinApp.h"

void ReadBinary::ReadFile(const char* filename)
{
	//  ファイルを開く
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);

	//Header読み込み
	ifs.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));

	// トライアングルサイズ読み込み
	m_triangledata.resize(m_header.TriangleSize);
	for(size_t i=0;i<m_header.TriangleSize;i++)
	ifs.read( reinterpret_cast< char* >( &m_triangledata[i]), sizeof(m_triangledata[i]));

	// 頂点数読み込み
	m_Vertexs.resize(m_header.VertexSize);
	for (size_t i = 0; i < m_header.VertexSize; i++)
	ifs.read( reinterpret_cast< char* >( &m_Vertexs[i]), sizeof(m_Vertexs[i]));

	// 法線数読み込み
	m_Normals.resize(m_header.NormalSize);
	for (size_t i = 0; i < m_header.NormalSize; i++)
	ifs.read( reinterpret_cast< char* >( &m_Normals[i]), sizeof(m_Normals[i]));

}

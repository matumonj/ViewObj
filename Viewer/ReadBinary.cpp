#include "ReadBinary.h"
#include	<iostream>
#include	<sstream>
#include	<fstream>

#include "WinApp.h"

void ReadBinary::ReadFile(const char* filename)
{
	//  �t�@�C�����J��
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);

	//Header�ǂݍ���
	ifs.read(reinterpret_cast<char*>(&m_header), sizeof(m_header));

	// �g���C�A���O���T�C�Y�ǂݍ���
	m_triangledata.resize(m_header.TriangleSize);
	for(size_t i=0;i<m_header.TriangleSize;i++)
	ifs.read( reinterpret_cast< char* >( &m_triangledata[i]), sizeof(m_triangledata[i]));

	// ���_���ǂݍ���
	m_Vertexs.resize(m_header.VertexSize);
	for (size_t i = 0; i < m_header.VertexSize; i++)
	ifs.read( reinterpret_cast< char* >( &m_Vertexs[i]), sizeof(m_Vertexs[i]));

	// �@�����ǂݍ���
	m_Normals.resize(m_header.NormalSize);
	for (size_t i = 0; i < m_header.NormalSize; i++)
	ifs.read( reinterpret_cast< char* >( &m_Normals[i]), sizeof(m_Normals[i]));

}

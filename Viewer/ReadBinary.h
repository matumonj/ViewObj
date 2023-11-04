#pragma once
#include <array>
#include <vector>

#include "Vector3.h"

class ReadBinary
{
private:
	//Header�����\����
#pragma pack(1)
	struct Header
	{
		//�O�p�ʂ̐�(tryangle�\���̂̐�)
		unsigned short TriangleSize = 0;
		//���_��
		unsigned short VertexSize = 0;
		//�@����
		unsigned short NormalSize = 0;
	}m_header;

#pragma pack()
	// �Ƃ炢���񂮂郊�X�g�\����
	struct TriangleData
	{
		std::array<int, 4>index;
	};
	// �g���C�A���O��
	std::vector<TriangleData>m_triangledata;
	// �@��
	std::vector<Vector3>m_Normals;
	// ���_���
	std::vector<Vector3>m_Vertexs;

public:
	// �ǂݍ��݊֐�
	void ReadFile(const char* filename);
};


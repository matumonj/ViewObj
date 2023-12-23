#pragma once
#include <array>
#include <DirectXMath.h>
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
	//�I�u�W�F�N�g�ɑ���C���f�b�N�X���
	std::vector<int>m_SendIndexData;
	//�I�u�W�F�N�g�ɑ��钸�_���
	std::vector<DirectX::XMFLOAT3>m_SendVerticesData;
	//
	std::vector<DirectX::XMFLOAT3>m_SendNormalData;
public:
	// �ǂݍ��݊֐�
	void ReadFile(const char* filename);

	std::vector<int>GetIndexData()const { return m_SendIndexData; }

	std::vector<DirectX::XMFLOAT3>GetVerticesData()const { return m_SendVerticesData; }

	std::vector<DirectX::XMFLOAT3>GetNormalData()const { return m_SendNormalData; }
};


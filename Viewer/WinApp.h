#pragma once
#include <Windows.h>

class WinApp
{
public: //�ÓI�����o�ϐ�
	// �E�B���h�E�T�C�Y
	static const int window_width = 1280;  // ����
	static const int window_height = 720;  // �c��
public:
	//�C���X�^���X�擾�p
	HINSTANCE GetInstance() { return w.hInstance; }
	/// �E�B���h�E�v���V�[�W��
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	/// ������
	void Initialize();
	/// ���b�Z�[�W�̏���
	bool ProcessMessage();
	/// �I��
	void Finalize();
	/// �E�B���h�E�X�^�C���̎w��
	void SetWindowStyle();
	/// �E�B���h�E�n���h���̎擾
	HWND GetHwnd() { return m_hwnd; }

private: //�����o�ϐ�
	HWND m_hwnd = nullptr;
	WNDCLASSEX w;
	LONG m_windowStyle; //�E�B���h�E�X�^�C��
};


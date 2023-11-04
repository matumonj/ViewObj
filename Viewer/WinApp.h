#pragma once
#include <Windows.h>

class WinApp
{
public: //静的メンバ変数
	// ウィンドウサイズ
	static const int window_width = 720;  // 横幅
	static const int window_height = 500;  // 縦幅
public:
	//インスタンス取得用
	HINSTANCE GetInstance() { return w.hInstance; }
	/// ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public:
	/// 初期化
	void Initialize();
	/// メッセージの処理
	bool ProcessMessage();
	/// 終了
	void Finalize();
	/// ウィンドウスタイルの指定
	void SetWindowStyle();
	/// ウィンドウハンドルの取得
	HWND GetHwnd() { return m_hwnd; }

private: //メンバ変数
	HWND m_hwnd = nullptr;
	WNDCLASSEX w;
	LONG m_windowStyle; //ウィンドウスタイル
};


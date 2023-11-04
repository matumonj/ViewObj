#include "WinApp.h"
#pragma comment(lib, "winmm.lib")

LRESULT WinApp::WindowProc(HWND m_hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//メッセージで分岐
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(m_hwnd, msg, wparam, lparam); //標準の処理を行う
}

void WinApp::Initialize() {

	SetWindowStyle();
	//ウィンドウクラスの設定
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"Viewer"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, m_windowStyle, false); // 自動でサイズ補正
	
	// ウィンドウオブジェクトの生成
	m_hwnd = CreateWindow(w.lpszClassName,
		w.lpszClassName,         // タイトルバーの文字
		m_windowStyle,        // ウィンドウスタイル
		GetSystemMetrics(SM_CXSCREEN)/6,// 表示X座標
		GetSystemMetrics(SM_CYSCREEN)/6,// 表示Y座標
		wrc.right - wrc.left,       // ウィンドウ横幅
		wrc.bottom - wrc.top,   // ウィンドウ縦幅
		nullptr,                // 親ウィンドウハンドル
		nullptr,                // メニューハンドル
		w.hInstance,            // 呼び出しアプリケーションハンドル
		nullptr);               // オプション

	// ウィンドウ表示
	ShowWindow(m_hwnd, SW_SHOW);


	timeBeginPeriod(1);
}

bool WinApp::ProcessMessage() {
	MSG msg{};  // メッセージ

	// メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // プロシージャにメッセージを送る
	}

	// ✖ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::Finalize() {
	// ウィンドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);
}

void WinApp::SetWindowStyle() {
	m_windowStyle = WS_SYSMENU | WS_CAPTION | WS_EX_CONTEXTHELP | WS_MINIMIZEBOX;
}
#pragma once

#include "Main.h"

// FPS計測用変数
int		FrameStartTime;
int		counter = 0, FpsTime[2] = { 0, }, FpsTime_i = 0;
double	fps = 0.0;
bool	IsQuit;


void	RenderFPS();

// スタート
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// 設定
	SetupCamera_Ortho(0.f);
	ChangeWindowMode(true);
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32);
	SetMainWindowText("EntreR - Demo Edition");
	SetGraphDisplayArea(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);
	// 初期化
	if (DxLib_Init() == -1) {
		return -1;
	}
	if (Effkseer_Init(2000) == -1) { // 引数には画面に表示する最大パーティクル数を設定する。
		DxLib_End();
		return -1;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DXライブラリのデバイスロストした時のコールバックを設定
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting(WINDOW_SIZE_X, WINDOW_SIZE_Y); // Effekseerに2D描画の設定をする。
	
	// Zバッファ(エフェクト用)
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// ゲーム初期化
	game_init();
	FrameStartTime = GetNowCount();

	IsQuit = false;

	SetDrawScreen(DX_SCREEN_BACK);
	

	// メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !IsQuit)
	{
		// １/６０秒立つまで待つ
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}
		FrameStartTime = GetNowCount();

		// ゲームメイン
		game_main();

		// エフェクトの更新
		UpdateEffekseer2D();
		DrawEffekseer2D();

		RenderFPS();
	}

	// ゲーム終了
	game_end();
	Effkseer_End();
	DxLib_End();

	return 0;
}

void RenderFPS() {
	if (FpsTime_i == 0)
		FpsTime[0] = GetNowCount();               //1周目の時間取得
	if (FpsTime_i == 49) {
		FpsTime[1] = GetNowCount();               //50周目の時間取得
		fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 50.0f);//測定した値からfpsを計算
		FpsTime_i = 0;//カウントを初期化
	}
	else
		FpsTime_i++;//現在何周目かカウント
	if (fps != 0)
		DrawFormatString(1200, 700, 0x00FF00, "FPS %.1f", fps); //fpsを表示
	return;
}
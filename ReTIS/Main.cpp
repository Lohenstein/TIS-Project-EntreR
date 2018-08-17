#pragma once

#include "Main.h"

// FPS計測用変数
int FrameStartTime;
int counter = 0, FpsTime[2] = { 0, }, FpsTime_i = 0;
double fps = 0.0;

void	RenderFPS();

// スタート
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// 設定
	ChangeWindowMode(true);
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32);
	SetMainWindowText("EntreR - Demo Edition");

	// 初期化
	if (DxLib_Init() == -1) {
		return -1;
	}

	SetWaitVSyncFlag(FALSE);
	SetDrawScreen(DX_SCREEN_BACK);

	// ゲーム初期化
	game_init();
	FrameStartTime = GetNowCount();

	// メインループ
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen())
	{
		// １/６０秒立つまで待つ
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}
		FrameStartTime = GetNowCount();

		// ゲームメイン
		game_main();
		RenderFPS();
	}

	// ゲーム終了
	game_end();
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
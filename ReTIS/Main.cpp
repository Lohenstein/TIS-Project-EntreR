#pragma once

#include "Main.h"

// FPS�v���p�ϐ�
int FrameStartTime;
int counter = 0, FpsTime[2] = { 0, }, FpsTime_i = 0;
double fps = 0.0;

void	RenderFPS();

// �X�^�[�g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// �ݒ�
	ChangeWindowMode(true);
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32);
	SetMainWindowText("EntreR - Demo Edition");

	// ������
	if (DxLib_Init() == -1) {
		return -1;
	}

	SetWaitVSyncFlag(FALSE);
	SetDrawScreen(DX_SCREEN_BACK);

	// �Q�[��������
	game_init();
	FrameStartTime = GetNowCount();

	// ���C�����[�v
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen())
	{
		// �P/�U�O�b���܂ő҂�
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}
		FrameStartTime = GetNowCount();

		// �Q�[�����C��
		game_main();
		RenderFPS();
	}

	// �Q�[���I��
	game_end();
	DxLib_End();

	return 0;
}

void RenderFPS() {
	if (FpsTime_i == 0)
		FpsTime[0] = GetNowCount();               //1���ڂ̎��Ԏ擾
	if (FpsTime_i == 49) {
		FpsTime[1] = GetNowCount();               //50���ڂ̎��Ԏ擾
		fps = 1000.0f / ((FpsTime[1] - FpsTime[0]) / 50.0f);//���肵���l����fps���v�Z
		FpsTime_i = 0;//�J�E���g��������
	}
	else
		FpsTime_i++;//���݉����ڂ��J�E���g
	if (fps != 0)
		DrawFormatString(1200, 700, 0x00FF00, "FPS %.1f", fps); //fps��\��
	return;
}
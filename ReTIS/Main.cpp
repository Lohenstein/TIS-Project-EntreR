#pragma once

#include "Main.h"

// FPS�v���p�ϐ�
int		FrameStartTime;
int		counter = 0, FpsTime[2] = { 0, }, FpsTime_i = 0;
double	fps = 0.0;
bool	IsQuit;


void	RenderFPS();

// �X�^�[�g
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// �ݒ�
	SetupCamera_Ortho(0.f);
	ChangeWindowMode(true);
	SetGraphMode(WINDOW_SIZE_X, WINDOW_SIZE_Y, 32);
	SetMainWindowText("EntreR - Demo Edition");
	SetGraphDisplayArea(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);
	// ������
	if (DxLib_Init() == -1) {
		return -1;
	}
	if (Effkseer_Init(2000) == -1) { // �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
		DxLib_End();
		return -1;
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ�
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting(WINDOW_SIZE_X, WINDOW_SIZE_Y); // Effekseer��2D�`��̐ݒ������B
	
	// Z�o�b�t�@(�G�t�F�N�g�p)
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	// �Q�[��������
	game_init();
	FrameStartTime = GetNowCount();

	IsQuit = false;

	SetDrawScreen(DX_SCREEN_BACK);
	

	// ���C�����[�v
	while (!ScreenFlip() && !ProcessMessage() && !ClearDrawScreen() && !IsQuit)
	{
		// �P/�U�O�b���܂ő҂�
		while (GetNowCount() - FrameStartTime < 1000 / 60) {}
		FrameStartTime = GetNowCount();

		// �Q�[�����C��
		game_main();

		// �G�t�F�N�g�̍X�V
		UpdateEffekseer2D();
		DrawEffekseer2D();

		RenderFPS();
	}

	// �Q�[���I��
	game_end();
	Effkseer_End();
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
#include "Main.h"

// �v���C���[�ϐ�
int sdplayerwalk[2];
int sdplayershot;
int sdplayerjump;
int sditemget;

// �G�l�~�[�ϐ�
int sdenemyshot;

void SoundLoad() {
	// �v���C���[�T�E���h
	sdplayerwalk[0] = LoadSoundMem("data/sound/Footstep-high.wav");
	sdplayerwalk[1] = LoadSoundMem("data/sound/Footstep-low.wav");
	sdplayershot = LoadSoundMem("data/sound/shot4.mp3");
	sdplayerjump = LoadSoundMem("data/sound/jump.mp3");
	sdplayerjump = LoadSoundMem("data/sound/jump.mp3");
	//sditemget = LoadSoundMem("data/sound/itemget.mp3");//

	// �G�l�~�[�T�E���h
	sdenemyshot = LoadSoundMem("data/sound/shot1.mp3");
}
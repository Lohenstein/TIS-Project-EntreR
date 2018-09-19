#include "Main.h"

// プレイヤー変数
int sdplayerwalk[2];
int sdplayershot;
int sdplayerjump;
int sditemget;

// エネミー変数
int sdenemyshot;

void SoundLoad() {
	// プレイヤーサウンド
	sdplayerwalk[0] = LoadSoundMem("data/sound/Footstep-high.wav");
	sdplayerwalk[1] = LoadSoundMem("data/sound/Footstep-low.wav");
	sdplayershot = LoadSoundMem("data/sound/shot4.mp3");
	sdplayerjump = LoadSoundMem("data/sound/jump.mp3");
	sdplayerjump = LoadSoundMem("data/sound/jump.mp3");
	//sditemget = LoadSoundMem("data/sound/itemget.mp3");//

	// エネミーサウンド
	sdenemyshot = LoadSoundMem("data/sound/shot1.mp3");
}
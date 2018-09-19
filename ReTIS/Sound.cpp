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
	printf("Loading Sounds...");

	sdplayerwalk[0] = LoadSoundMem("data/sound/Footstep-high.wav");
	//printf("Loaded:data/sound/Footstep-high.wav\n");
	sdplayerwalk[1] = LoadSoundMem("data/sound/Footstep-low.wav");
	//("Loaded:data/sound/Footstep-low.wav\n");
	sdplayershot = LoadSoundMem("data/sound/shot4.mp3");
	//printf("Loaded:data/sound/shot4.mp3\n");
	sdplayerjump = LoadSoundMem("data/sound/jump.mp3");
	//printf("Loaded:data/sound/jump.mp3\n");
	sdplayerjump = LoadSoundMem("data/sound/jump.mp3");
	//printf("Loaded:data/sound/jump.mp3\n");
	//sditemget = LoadSoundMem("data/sound/itemget.mp3");//

	// エネミーサウンド
	sdenemyshot = LoadSoundMem("data/sound/shot1.mp3\n");
	//printf("Loaded:data/sound/shot1.mp3\n");

	printf("Success!\n");
}
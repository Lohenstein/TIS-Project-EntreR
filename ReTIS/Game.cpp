
#include "Main.h"

namespace Game
{

}

int		gamemode = Game::mode_game;
int		font_handle[FONT_MAX];
std::string stagepath;


using	namespace Game;

/*------------------------------------------------------------------------------*
| <<< ゲーム初期化 >>>
*------------------------------------------------------------------------------*/
void	game_init(void)
{
	title.reset(new cTitle);
	gamemode = mode_title;

	// フォントの読み込み
	//--------------------------------------------------------------------------
	LPCSTR font_path = "data/font/みかちゃん-PB.ttf";
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォントの読込に失敗しました。", "Debug - Error", MB_OK);
	}
	LoadBulletImg();
	LoadEffects();

	font_handle[FONT_POSSESSTIME] = CreateFontToHandle("みかちゃん-PB", 48, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_TIME]		  = CreateFontToHandle("みかちゃん-PB", 72, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_MESSAGE]	  = CreateFontToHandle("みかちゃん-PB", 24, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
}
/*------------------------------------------------------------------------------*
| <<< ゲームメイン >>>
*------------------------------------------------------------------------------*/
void	game_main(void)
{
	// 処理
	switch (gamemode)
	{
	case mode_title:
		title->Update();
		break;
	case mode_lobby:
		break;
	case mode_game:
		scene->Update();
		break;
	case mode_over:
		break;
	}

	// 描画
	switch (gamemode)
	{
	case mode_title:
		title->Render();
		break;
	case mode_game:
		scene->Render();
		break;
	case mode_over:
		break;
	}
}

/*------------------------------------------------------------------------------*
| <<< ゲーム終了 >>>
*------------------------------------------------------------------------------*/
void	game_end(void)
{
}
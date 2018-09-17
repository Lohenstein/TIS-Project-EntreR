
#include "Main.h"

namespace Game
{

}

int		gamemode;
int		font_handle[FONT_MAX];
int		icon_handle[14];
bool	IsWindowed;

std::string stagepath;

using	namespace Game;

/*------------------------------------------------------------------------------*
| <<< ゲーム初期化 >>>
*------------------------------------------------------------------------------*/
void	game_init(void)
{
	title.reset(new cTitle);
	gamemode = mode_title;

	IsWindowed = true;

	// フォントの読み込み
	//--------------------------------------------------------------------------
	LPCSTR font_path = "data/font/GN-Kin-iro_SansSerif.ttf";
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォントの読込に失敗しました。", "Debug - Error", MB_OK);
	}
	LoadBulletImg();
	LoadEffects();

	// フォントデータ
	font_handle[FONT_POSSESSTIME] = CreateFontToHandle("GN-きんいろサンセリフ", 48, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_TIME]		  = CreateFontToHandle("GN-きんいろサンセリフ", 72, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_MESSAGE]	  = CreateFontToHandle("GN-きんいろサンセリフ", 24, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_MENU]		  = CreateFontToHandle("GN-きんいろサンセリフ", 32, 2, DX_FONTTYPE_ANTIALIASING);

	// ボタン画像のロード
	icon_handle[ICON_A]		= LoadGraph("data/img/buttons/XB1_A.png");
	icon_handle[ICON_B]		= LoadGraph("data/img/buttons/XB1_B.png");
	icon_handle[ICON_X]		= LoadGraph("data/img/buttons/XB1_X.png");
	icon_handle[ICON_Y]		= LoadGraph("data/img/buttons/XB1_Y.png");
	icon_handle[ICON_PAD]	= LoadGraph("data/img/buttons/XB1_DPad.png");
	icon_handle[ICON_LB]	= LoadGraph("data/img/buttons/XB1_LB.png");
	icon_handle[ICON_RB]	= LoadGraph("data/img/buttons/XB1_RB.png");
	icon_handle[ICON_LT]	= LoadGraph("data/img/buttons/XB1_LT.png");
	icon_handle[ICON_RT]	= LoadGraph("data/img/buttons/XB1_RT.png");
	icon_handle[ICON_LS]	= LoadGraph("data/img/buttons/XB1_LeftStick.png");
	icon_handle[ICON_RS]	= LoadGraph("data/img/buttons/XB1_RightStick.png");
	icon_handle[ICON_MENU]	= LoadGraph("data/img/buttons/XB1_Menu.png");
	icon_handle[ICON_VIEW]	= LoadGraph("data/img/buttons/XB1_View.png");
	icon_handle[ICON_XBOX]	= LoadGraph("data/img/buttons/XB1_XboxButton.png");
}
/*------------------------------------------------------------------------------*
| <<< ゲームメイン >>>
*------------------------------------------------------------------------------*/
void	game_main(void)
{
	//SetupCamera_Ortho(3000.f);
	//SetupCamera_Ortho(3000.f);

	input(); // 入力
	// ------ 処理 ------
	switch (gamemode)
	{
	case mode_title:
		title->Update();
		break;
	case mode_lobby:
		break;
	case mode_game:
		if (!scene->IsPaused) {
			scene->Update();
		}
		break;
	case mode_over:
		break;
	}

	// ------ 描画 ------
	switch (gamemode)
	{
	case mode_title:
		title->Render();
		break;
	case mode_game:
		scene->Render();
		if (scene->IsPaused) scene->DrawPauseMenu();
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
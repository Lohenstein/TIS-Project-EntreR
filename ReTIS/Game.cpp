
#include "Main.h"

namespace Game
{

}

int		gamemode = Game::mode_game;
int		font_handle[FONT_MAX];
std::string stagepath;


using	namespace Game;

/*------------------------------------------------------------------------------*
| <<< �Q�[�������� >>>
*------------------------------------------------------------------------------*/
void	game_init(void)
{
	title.reset(new cTitle);
	gamemode = mode_title;

	// �t�H���g�̓ǂݍ���
	//--------------------------------------------------------------------------
	LPCSTR font_path = "data/font/�݂������-PB.ttf";
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�̓Ǎ��Ɏ��s���܂����B", "Debug - Error", MB_OK);
	}
	LoadBulletImg();
	LoadEffects();

	font_handle[FONT_POSSESSTIME] = CreateFontToHandle("�݂������-PB", 48, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_TIME]		  = CreateFontToHandle("�݂������-PB", 72, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_MESSAGE]	  = CreateFontToHandle("�݂������-PB", 24, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
}
/*------------------------------------------------------------------------------*
| <<< �Q�[�����C�� >>>
*------------------------------------------------------------------------------*/
void	game_main(void)
{
	// ����
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

	// �`��
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
| <<< �Q�[���I�� >>>
*------------------------------------------------------------------------------*/
void	game_end(void)
{
}
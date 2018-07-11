
#include "Main.h"

namespace Game
{
	int mode = mode_game;
	std::unique_ptr<cBase> scene;

}

int		font_handle[FONT_MAX];

using	namespace Game;

/*------------------------------------------------------------------------------*
| <<< �Q�[�������� >>>
*------------------------------------------------------------------------------*/
void	game_init(void)
{
	scene.reset(new cGame);

	// �y����ǂݍ���
	LPCSTR font_path = "data/font/�݂������-PB.ttf";
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�̓Ǎ��Ɏ��s���������...", "���߂�ˁ���", MB_OK);
	}

	font_handle[FONT_POSSESSTIME] = CreateFontToHandle("�݂������-PB", 48, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	font_handle[FONT_TIME]		  = CreateFontToHandle("�݂������-PB", 72, 2, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
}
/*------------------------------------------------------------------------------*
| <<< �Q�[�����C�� >>>
*------------------------------------------------------------------------------*/
void	game_main(void)
{
	// ����
	switch (mode)
	{
	case mode_title:
		break;
	case mode_game:
		scene->Update();
		break;
	case mode_over:
		break;
	}

	// �`��
	switch (mode)
	{
	case mode_title:
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
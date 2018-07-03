
#include "Main.h"

namespace Game
{
	int mode = mode_game;
	std::unique_ptr<cBase> scene;

}

using	namespace Game;

/*------------------------------------------------------------------------------*
| <<< ゲーム初期化 >>>
*------------------------------------------------------------------------------*/
void	game_init(void)
{
	scene.reset(new cGame);
}
/*------------------------------------------------------------------------------*
| <<< ゲームメイン >>>
*------------------------------------------------------------------------------*/
void	game_main(void)
{
	// 処理
	switch (mode)
	{
	case mode_title:
		break;
	case mode_game:
		break;
	case mode_over:
		break;
	}

	// 描画
	switch (mode)
	{
	case mode_title:
		break;
	case mode_game:
		scene->Show();
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
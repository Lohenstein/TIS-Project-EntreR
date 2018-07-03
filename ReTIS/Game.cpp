
#include "Main.h"

namespace Game
{
	int mode = mode_game;
	std::unique_ptr<cBase> scene;

}

using	namespace Game;

/*------------------------------------------------------------------------------*
| <<< ƒQ[ƒ€‰Šú‰» >>>
*------------------------------------------------------------------------------*/
void	game_init(void)
{
	scene.reset(new cGame);
}
/*------------------------------------------------------------------------------*
| <<< ƒQ[ƒ€ƒƒCƒ“ >>>
*------------------------------------------------------------------------------*/
void	game_main(void)
{
	// ˆ—
	switch (mode)
	{
	case mode_title:
		break;
	case mode_game:
		break;
	case mode_over:
		break;
	}

	// •`‰æ
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
| <<< ƒQ[ƒ€I—¹ >>>
*------------------------------------------------------------------------------*/
void	game_end(void)
{
}
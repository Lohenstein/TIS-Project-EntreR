#pragma once

namespace	Game
{
	//--- �萔�錾 ---
	enum mode {
		mode_title,
		mode_game,
		mode_over,
	};
};

class	cGameManager {
public:
	void	Update(void);
	void	Render(void);
};

extern	void	game_init(void);
extern	void	game_main(void);
extern	void	game_end(void);
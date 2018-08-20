#pragma once

#define FONT_MAX			(16)
#define FONT_TIME			(0)
#define FONT_POSSESSTIME	(1)
#define	FONT_MESSAGE		(2)

namespace	Game
{
	//--- íËêîêÈåæ ---
	enum mode {
		mode_title,
		mode_lobby,
		mode_game,
		mode_over,
	};
};

class	cGameManager {
public:
	void	Update(void);
	void	Render(void);
};

extern int	font_handle[FONT_MAX];

extern	void	game_init(void);
extern	void	game_main(void);
extern	void	game_end(void);

extern int gamemode;

extern std::string stagepath;


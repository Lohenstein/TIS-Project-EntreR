#pragma once

#define FONT_MAX			(16)
#define FONT_TIME			(0)
#define FONT_POSSESSTIME	(1)
#define	FONT_MESSAGE		(2)

#define	ICON_A				(0)
#define	ICON_B				(1)
#define	ICON_X				(2)
#define	ICON_Y				(3)
#define ICON_PAD			(4)
#define ICON_LB				(5)
#define ICON_RB				(6)
#define ICON_LT				(7)
#define ICON_RT				(8)
#define ICON_LS				(9)
#define ICON_RS				(10)
#define ICON_MENU			(11)
#define ICON_VIEW			(12)
#define ICON_XBOX			(13)

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

extern int		font_handle[FONT_MAX];

extern	void	game_init(void);
extern	void	game_main(void);
extern	void	game_end(void);

extern int		gamemode;
extern int		icon_handle[14];

extern bool		IsFullscreen;

extern std::string stagepath;


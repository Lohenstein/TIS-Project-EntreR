
//--- ウィンドウサイズ----------------
#define		WINDOW_SIZE_X (1280)
#define		WINDOW_SIZE_Y (720)

#define		BSIZE		  (32)

#define		PI (3.14159265358979)

#define		ENTITY_MAX		(8)
#define		BULLET_MAX		(64)
#define		ENEMY_MAX		(24)
#define		AUTOSCROL_MAX	(5)

#define		d2r(deg) (((deg) / 360) * 2 * PI)
#define		r2d(rad) (((rad) / 2 / PI) * 360)

//--- ライブラリ----------------------
#include "DxLib.h"
#include "EffekseerForDXLib.h"

//--- ヘッダインクルード --------
#include "Include.h"

extern	bool	IsQuit;
extern	int		FrameStartTime;
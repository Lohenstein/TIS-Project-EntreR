#pragma once

#define STICK_UP   (16)
#define STICK_DOWN (17)


extern	int	key[256];
extern	int	pad_b[18];

extern	short	stick_lx, stick_ly;
extern  int		trigger_l, trigger_r;
extern	float	stick_rad;

extern	void input();
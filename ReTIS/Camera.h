#pragma once

typedef struct {
	int top;
	int bottom;
	int left;
	int right;
}WINDOWRECT;

class cCamera {
protected:
	WINDOWRECT window;
	float px, py;
	float camera_px;
	float camera_py;
	int	  sx, sy;
public:
	void	Update(VECTOR focus);
	void	Render(int handle);
	void	SetStageSize(int x, int y) {sx = x, sy = y;}
};
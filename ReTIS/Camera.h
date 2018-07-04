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
public:
	void	Update(VECTOR focus);
	void	Render(int handle);
};
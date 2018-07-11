#pragma once

typedef struct {
	int top;
	int bottom;
	int left;
	int right;
}WINDOWRECT;

class cBackground {
protected:
public:
	int bghandle_09[4];
	VECTOR bghandle_pos_09[4][2];
	cBackground() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 2; j++) {
				bghandle_pos_09[i][j].x = j;
				bghandle_pos_09[i][j].y = -WINDOW_SIZE_Y / 4;
			}
		}
		LoadDivGraph("data/img/bg/Mobile_Backgrounds_09.png", 4, 1, 4, 4096, 1024, bghandle_09);
	}
};

class cCamera {
protected:
	WINDOWRECT window;
	float px, py;
	float camera_px;
	float camera_py;
	int	  ox, oy;
	int	  sx, sy;
	float cloud[2] = {0.f, 4090.f};
	cBackground *bg;
public:
	cCamera() {
		bg = new cBackground();
	};
	~cCamera() {
		delete bg;
		bg = nullptr;
	};
	void	Update(VECTOR focus);
	void	Render(int handle);
	void	SetStageSize(int x, int y) {sx = x, sy = y;}
};

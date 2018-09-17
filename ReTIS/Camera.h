#pragma once

using namespace std;

typedef struct {
	int top;
	int bottom;
	int left;
	int right;
}WINDOWRECT;

class cBackground {
protected:
public:
	int bghandle_09[4];					// 画像を入れる配列
	VECTOR bghandle_pos_09[4][2];
	cBackground() {						// 配列の座標初期化
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 2; j++) {
				bghandle_pos_09[i][j].x = (float)j;
				bghandle_pos_09[i][j].y = -WINDOW_SIZE_Y / 4;
			}
		}
		LoadDivGraph("data/img/bg/Mobile_Backgrounds_09.png", 4, 1, 4, 4096, 1024, bghandle_09);	// 木の背景と雲の背景がこれ　他の09はこの番号を示している
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
	float speed = 2.f;
	int count = 0;
public:
	int	  move = -1;
	float DestinationX[10] = {0,0,0,0,0,0,0,0,0,0 }, DestinationY[10] = { 0,0,0,0,0,0,0,0,0,0 };

	cCamera(string name) {
			//AutoScrolConfig(name);
		bg = new cBackground();
	};
	~cCamera() {
		delete bg;
		bg = nullptr;
	};
	void	Update(VECTOR focus);
	void	Render(int handle,int getx,int gety);
	void	SetStageSize(int x, int y) {sx = x, sy = y;}
	void	AutoScrol(VECTOR focus);
	// csvからオートスクロールする場所を割り出す
	void	AutoScrolConfig(string name);

	float mokutekiX,mokutekiY;
	float ans = -50.f;
};
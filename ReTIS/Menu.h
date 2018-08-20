#pragma once
class cMenu {
public:
	int		title_selected;
	int		y = 280;
	int		x = 550;
	int		selected = 0;
	int		ease_flag = 0;
	//int ease_pos = 0;
	double ease_time = 0.0;

	cMenu() {
		title_selected = LoadGraph("data/img/wall/selected.png");
	}

	int draw(int mx, int my, int n, char **str);
};

extern cMenu	menu;
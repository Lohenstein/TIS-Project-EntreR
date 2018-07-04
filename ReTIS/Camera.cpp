
#include "Main.h"

// -----計算に必要な変数類---------------------------------------------
double	camera_time_x, camera_time_y;
bool	camera_flag_x, camera_flag_y;
bool	camera_flag_half_x, camera_flag_half_y;

void cCamera::Update(VECTOR focus) {

	window.left = (int)focus.x - WINDOW_SIZE_X / 2;
	window.right = (int)focus.x + WINDOW_SIZE_X / 2;
	window.top = (int)focus.y - WINDOW_SIZE_Y / 2;
	window.bottom = (int)focus.y + WINDOW_SIZE_Y / 2;

	// -----カメライージング横軸-------------------------------------------
	if (FocusPos.x == px) {
		camera_time_x = 0.0;
		camera_flag_x = false;
		camera_flag_half_x = false;
	}
	else if (camera_flag_x == false) {
		camera_flag_x = true;
	}
	else if (camera_flag_x) {
		if (camera_time_x >= 0.5) {
			camera_flag_half_x = true;
		}
		if (camera_flag_half_x) {
			if (FocusPos.x == FocusOld.x) {
				camera_time_x += 0.01;
				if (camera_time_x >= 1.0) {
					camera_flag_x = false;
					camera_flag_half_x = false;
					px = FocusPos.x;
				}
			}
			else {
				camera_time_x = 0.5;
			}
		}
		else {
			camera_time_x += 0.01;
		}
	}
	// -----カメライージング縦軸-------------------------------------------
	if (FocusPos.y == py) {
		camera_time_y = 0.0;
		camera_flag_y = false;
		camera_flag_half_y = false;
	}
	else if (camera_flag_y == false) {
		camera_flag_y = true;
	}
	else if (camera_flag_y) {
		if (camera_time_y >= 0.5) {
			camera_flag_half_y = true;
		}
		if (camera_flag_half_y) {
			if (FocusPos.y == FocusOld.y) {
				camera_time_y += 0.01;
				if (camera_time_y >= 1.0) {
					camera_flag_y = false;
					camera_flag_half_y = false;
					py = FocusPos.y;
				}
			}
			else {
				camera_time_y = 0.5;
			}
		}
		else {
			camera_time_y += 0.01;
		}
	}
	// ----イージング-------------------------------------------
	px = (float)Easing::InOutQuad(camera_time_x, 1.0, (double)FocusPos.x, (double)px);
	py = (float)Easing::InOutQuad(camera_time_y, 1.0, (double)FocusPos.y, (double)py);

	camera_px = px - WINDOW_SIZE_X / 2.f;
	camera_py = py - WINDOW_SIZE_Y / 2.f;

	if (camera_px < 0.f) camera_px = 0.f;
	if (camera_px > sx * bsize - WINDOW_SIZE_X) camera_px = sx * bsize - WINDOW_SIZE_X;
}

void cCamera::Render(int handle) {
	DrawRectGraph(0, 0, (int)camera_px, (int)camera_py, WINDOW_SIZE_X, WINDOW_SIZE_Y, handle, false, false);
}

#include "Main.h"

cMenu	menu;
cMenu	stageselect;
cMenu	optionselect;
/*------------------------------------------------------------------------------*
| <<< メニュークラス >>>
*------------------------------------------------------------------------------*/
int cMenu::draw(int mx, int my, int n, char **str) {

	x = mx, y = my;

	// イージング
	if (ease_flag != 0) {
		double tx = Easing::OutQuint(ease_time, 1.0, x - ((double)selected * 36), x - (((double)selected + ease_flag) * 36));
		double ty = Easing::OutQuint(ease_time, 1.0, y + ((double)selected * 61), y + (((double)selected + ease_flag) * 61));
		int color = Easing::OutQuint(ease_time, 1.0, 222, 122);

		DrawGraph(tx, ty, title_selected, TRUE);
		int str_x = (x + 560) - ((selected * 36) + GetDrawFormatStringWidthToHandle(font_handle[FONT_MESSAGE], "%s", str[selected]));
		DrawFormatStringToHandle(str_x, y + (selected * 61) + 8, GetColor(color, color, color), font_handle[FONT_MESSAGE], "%s", str[selected]);

		ease_time += 30;
		if (ease_time > 1.0) ease_flag = 0;
	}

	// 順に表示
	for (int i = 0; i < n; i++) {
		int str_x = (x + 560) - ((i * 36) + GetDrawFormatStringWidthToHandle(font_handle[FONT_MESSAGE], "%s", str[i]));
		if (selected == i) {
			if (ease_flag == 0) {
				DrawGraph(x - (i * 36), y + (i * 61), title_selected, TRUE);
				DrawFormatStringToHandle(str_x, y + (i * 61) + 8, GetColor(222, 222, 222), font_handle[FONT_MESSAGE], "%s", str[i]);
			}
		}
		else {
			DrawFormatStringToHandle(str_x, y + (i * 61) + 8, GetColor(122, 122, 122), font_handle[FONT_MESSAGE], "%s", str[i]);
		}
	}
	// かっこいい横線
	DrawLineAA(x + 600, y, x + 600 - (n * 36), y + (n * 61), GetColor(122, 122, 122));


	// キー入力
	if (key[KEY_INPUT_UP] == 1 || pad_b[XINPUT_BUTTON_DPAD_UP] == 1) {
		selected--;
		ease_flag = 1;
		ease_time = 0.0;
		//PlaySoundMem(menu_sehnd[1], DX_PLAYTYPE_BACK, TRUE);
	}
	if (key[KEY_INPUT_DOWN] == 1 || pad_b[XINPUT_BUTTON_DPAD_DOWN] == 1) {
		selected++;
		ease_flag = -1;
		ease_time = 0.0;
		//PlaySoundMem(menu_sehnd[1], DX_PLAYTYPE_BACK, TRUE);
	}
	// 決定時
	if (key[KEY_INPUT_Z] == 1 || pad_b[XINPUT_BUTTON_B] == 1) {
		//PlaySoundMem(menu_sehnd[0], DX_PLAYTYPE_BACK, TRUE);
		return selected;
	}

	//範囲外防止
	if (selected >= n) selected = --n;
	if (selected <  0) selected = 0;

	return -1;
}

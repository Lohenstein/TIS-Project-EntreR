
#include "Main.h"

void	cDialog::Update() {

	if (str_count >= (int)message.size()) {
		if (end - start > draw_time) {
			// 全文字描画したら消える
			IsHide = true;
			start = 0, end = 0;	// 描画オフセット初期化
			str_count = 0;		// 文字列カウント初期化
			message = "";		// 描画文字列初期化
			out = "";
		}
		end = clock();
	}
	else if (!IsHide) {
		// 一文字ずつ追加していく
		if (end - start > wait_time) {
			out += message[str_count];
			str_count++;
			start = end;
		}
		end = clock();
	}
}

void	cDialog::Render() {

	if (!IsHide) {
		// テキストボックス
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(20, 500, 1260, 700, 0xFFFFFF, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// メッセージ
		DrawFormatStringToHandle(40, 520, 0xFFFFFF, font_handle[FONT_MESSAGE], out.c_str());
	}
}
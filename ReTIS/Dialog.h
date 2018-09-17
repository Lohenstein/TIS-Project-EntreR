#pragma once

using namespace std;

extern void DebugMsgBox(TCHAR * tszParam, ...);

// ダイアログ表示クラス
class cDialog {
public:
	// 変数
	string	message;			// メッセージ文字列全文
	string	out;				// 出力されている文字列
	clock_t wait_time = 50;		// オフセットタイム
	clock_t draw_time = 3000;	// 全文字列表示してから消えるまで
	clock_t start = 0, end = 0; // オフセット計測用変数
	int		str_count = 0;		// 文字数カウント
	bool	IsHide = false;		// ダイアログの表示/非表示

	// 関数
	void	Render();
	void	Update();
	void	SetMessageStr(string str) { message = str; }
	void	Hide(bool flag) { IsHide = flag; }

	// コンストラクタ
	cDialog(string str) {
		SetMessageStr(str);
		start = clock();
	}
	~cDialog() {
		//delete this;
	}
};


#include "Main.h"
/*------------------------------------------------------------------------------*
| <<< cMoveFloor >>>
*------------------------------------------------------------------------------*/
void	cMoveFloor::Update(float s, int p1, int p2) {

	// 移動する角度と距離を求める
	float rad = atan2f(sy[p2] - sy[p1], sx[p2] - sx[p1]);
	float dis = sqrtf((sx[p2] - sx[p1]) * (sx[p2] - sx[p1]) + (sy[p2] - sy[p1]) * (sy[p2] - sy[p1]));

	// 過去の移動を保存
	old = pos;

	// 移動する
	pos.x = sx[p1] + (sinf(rad + PI) * p);
	pos.y = sy[p1] + (cosf(rad + PI) * p);

	// 移動の方向に向かって動かす
	if (flag)
		p += s;
	else
		p -= s;

	// 移動の方向の切り替え
	if (p >= dis || p <= 0.f) {
		if (flag)
			flag = false;
		else
			flag = true;
	}
}

void	 cMoveFloor::Render() {
	DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
		GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
		0x0000FF, true);
}

void	 cMoveFloor::SetPoint(int num, float x, float y) {
	sx[num] = x;
	sy[num] = y;
}
/*------------------------------------------------------------------------------*
| <<< cDropFloor >>>
*------------------------------------------------------------------------------*/
void	cDropFloor::Update() {
	// 落下処理
	if (flag) {
		time++;
		// 床に乗って0.5秒経ったら(60fps)
		const int fps = 60;
		if (time > fps / 2) {
			pos.y -= drop_p;
			drop_p -= gravity;
			const int prev_time = 5;
			if (time > fps * prev_time) {
				drop_p = 0.f;
				flag = false;
				time = 0;
				pos.y = sy;
			}
		}
	}
}

void	 cDropFloor::Render() {
	DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
		GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
		0x0000FF, true);
}

void	cDropFloor::HitAction(cObject *hit) {
	flag = true;
}

void	cDropFloor::SetPoint(float x, float y) {
	pos = { x, y, 0.f };
	sx = x, sy = y;
}
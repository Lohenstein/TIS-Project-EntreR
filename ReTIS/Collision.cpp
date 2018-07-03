#include "Main.h"

// 矩形同士のあたり判定
void CheckHitRectAndRect(cObject *obj1, cObject *obj2) {

	if (obj1->GetPos().x - (obj1->GetSize().x / 2.f) <= obj2->GetPos().x + (obj2->GetSize().x / 2.f) &&
		obj2->GetPos().x - (obj2->GetSize().x / 2.f) <= obj1->GetPos().x + (obj1->GetSize().x / 2.f) &&
		obj1->GetPos().y - (obj1->GetSize().y / 2.f) <= obj2->GetPos().y + (obj2->GetSize().y / 2.f) &&
		obj2->GetPos().y - (obj2->GetSize().y / 2.f) <= obj1->GetPos().y + (obj1->GetSize().y / 2.f)) {

		// 衝突相手の情報を渡して衝突後の処理を行う
		obj1->HitAction(obj2);
		obj2->HitAction(obj1);
	}
}



// 上に載っていられる面積を判別する
float	col_CheckRadian(VECTOR p1, VECTOR p2) {
	float x = (p1.x / 2.f) + (p2.x / 2.f);
	float y = (p1.y / 2.f) + (p2.y / 2.f);
	return atan2f(y, x);
}

// 当たり判定処理後どの方向に戻すのかを角度から求める
// 1 : right, 2 : bottom, 3 : left, 4 : top
int		col_HitRadian(VECTOR p1, VECTOR p2, float rad) {

	// キャラクタ中心とマップチップ中心の角度
	int deg  = (int)r2d(atan2f(p2.y - p1.y, p2.x - p1.x) + PI);
	int cdeg = (int)r2d(rad);

	if (deg >= cdeg && deg < 180 - cdeg) {
		return 2;
	}
	else if (deg >= 180 - cdeg && deg < 180 + cdeg) {
		return 3;
	}
	else if (deg >= 180 + cdeg && deg < 360 - cdeg) {
		return 4;
	}
	else {
		return 1;
	}
	return -1;
}

int		col_HitRadianR(VECTOR p1, VECTOR p2) {

	// キャラクタ中心とマップチップ中心の角度
	int deg = (int)r2d(atan2f(p2.y - p1.y, p2.x - p1.x)) + 180;

	if (deg > 45 && deg <= 135) {
		return 2;
	}
	else if (deg > 135 && deg <= 225) {
		return 3;
	}
	else if (deg > 225 && deg <= 315) {
		return 4;
	}
	else {
		return 1;
	}
	return -1;
}
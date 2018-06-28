#pragma once

class cObject {
protected:
	VECTOR	pos;						// 座標
	VECTOR	size;						// オブジェクトのサイズ
	int		type;						// オブジェクトのタイプ
public:
	VECTOR	GetPos()  { return pos; }	// 位置のアドレスを返す
	VECTOR	GetSize() { return size; }	// オブジェクトのサイズを返す
	int		GetType() { return type; }
	virtual void HitAction(cObject *hit) = 0;			// 衝突後の処理
};

enum eObjectType {
	MapTile,
	MoveFloor,
	DropFloor,
	Character,
	Enemy,
	Player
};

extern void		CheckHitRectAndRect(cObject *obj1, cObject *obj2);
extern float	col_CheckRadian(VECTOR p1, VECTOR p2);
extern int		col_HitRadian(VECTOR p1, VECTOR p2, float rad);
extern int		col_HitRadianR(VECTOR p1, VECTOR p2);
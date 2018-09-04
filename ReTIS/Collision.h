#pragma once

class cObject {
protected:
	VECTOR	pos;						// 座標
	VECTOR	old;						// 差分用過去座標
	VECTOR	size;						// オブジェクトのサイズ
	int		type;						// オブジェクトのタイプ
	float	rad;
	short	angle;
	float	speed;
	short	count;
public:
	VECTOR	GetPos()  { return pos; }	// 位置のアドレスを返す
	VECTOR	GetSize() { return size; }	// オブジェクトのサイズを返す
	VECTOR  GetOld()  { return old; }
	float 	GetSpeed() { return speed; }
	int		GetType() { return type; }
	void	SetPos(float x, float y) {pos = { x, y, 0.f };}
	virtual void HitAction(cObject *hit) = 0;			// 衝突後の処理
	float GetRadrightbottom() { return cos(rad); }
	float GetRadleftbottom() {
		rad = d2r(angle * 2);
		return sin(rad);
	}
	float GetRadlefttop() {
		rad = d2r(angle * 3);
		return sin(rad);
	}
	float GetRadrighttop() {
		rad = d2r(angle * 4);
		return cos(rad);
	}
};

enum eObjectType {
	MapTile,		// マップ
	MoveFloor,		// 動く床
	DropFloor,		// 乗ったら落ちる床
	Character,		// 敵かプレイヤー ※使わない
	Enemy,			// 敵
	Player,			// プレイヤー
	PlayerBullet,	// プレイヤーキャラから発射される弾（乗り移り弾）
	EnemyBullet,	// 敵から発射される弾
	PlayerAttack,	// プレイヤーの攻撃　bullet.Shotで使用
	Bullet,			// 弾 ※使わない
	Clear,			// クリア判定
	NormalCoin,		// コイン
	EneCoin,		// エネルギーコイン
	RareCoin,		// レアコイン
	TimeCoin,		// 時間コイン
	WireAnchor,
	Spring,
	JugemBullet,	// 敵ジュゲム
	Crumblewall,	// 壊れる壁
	Gear,			// 歯車
	MoveWall,
	WireAnchorWire
};

extern void		CheckHitRectAndRect(cObject *obj1, cObject *obj2);
extern void		CheckHitRectAndCircle(cObject *obj1, cObject *obj2);
extern void		CheckHitRectAndRollingRect(cObject *obj1, cObject *obj2);
//extern void		CheckHitRollingRectAndCircle(cObject *obj1, cObject *obj2);
extern float	col_CheckRadian(VECTOR p1, VECTOR p2);
extern int		col_HitRadian(VECTOR p1, VECTOR p2, float rad);
extern int		col_HitRadianR(VECTOR p1, VECTOR p2);
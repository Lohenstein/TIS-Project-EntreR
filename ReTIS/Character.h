#pragma once

class cCharacterBase : public cObject{
protected:
	VECTOR	old;	// 差分用過去座標
	float	speed;
	float	jump = 0.f, gravity = 1.f , inertia = 0; // 重力と慣性
	bool	possess;	// 乗り移られているか
	void	Physical();	// ジャンプとかの計算
public:
	cCharacterBase() {};
	~cCharacterBase() {};
	void	HitAction(cObject *hit);
	void	MoveByAutomation();
	void	MoveByPlayer();
	void	Update();
	void	Render();
	VECTOR  GetOldPos() { return old; }
};

class cPlayer : public cCharacterBase {
protected:
public:
	cPlayer(float x, float y, float w, float h, float s, bool p) {
		pos     = { x, y, 0.f };
		size    = { w, h, 0.f };
		speed   = s;
		possess = p;
		type    = Player;
	}
	~cPlayer(){}
	void	Render();
};

class cEnemy : public cCharacterBase {
protected:
	cBulletManager *bullet;
public:
	cEnemy(float x, float y, float w, float h, float s, bool p) {
		pos     = { x, y, 0.f };
		size    = { w, h, 0.f };
		speed   = s;
		possess = p;
		bullet  = new cBulletManager();
		type    = Enemy;
	}
	~cEnemy() {
		delete bullet;
		bullet = nullptr;
	}
	void	Render();
};

class cCharacterManager {
protected:
public:
	cPlayer *player;
	cCharacterManager() {
		player = new cPlayer(400.f, 100.f, 90.f, 120.f, 6.f, true);
	}
	~cCharacterManager() {
		delete player;
		player = nullptr;
	}
	void	Update();
	void	Render();
	cObject *GetPlayer() { return (cObject*)player; }
};
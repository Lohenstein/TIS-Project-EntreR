#pragma once

class cCharacterBase : public cObject{
protected:
	VECTOR	old;	// 差分用過去座標
	float	speed;
	float	jump = 0.f, gravity = 1.f , inertia = 0; // 重力と慣性
	bool	possess;	// 乗り移られているか
	void	Physical();	// ジャンプとかの計算

	// 着地した時の判定 ------------------
	bool	landing;

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
	/*cEnemy(float x, float y, float w, float h, float s, bool p) {
		pos     = { x, y, 0.f };
		size    = { w, h, 0.f };
		speed   = s;
		possess = p;
		bullet  = new cBulletManager();
		type    = Enemy;
	}*/
	~cEnemy() {
		delete bullet;
		bullet = nullptr;
	}
	void	Render();
};

class cEnemyJumpman : public cEnemy 
{
public:
	cBulletManager *bullet;

	float rad;
	bool move_flag;
	int jump_count;
	int move_dir;

	cEnemyJumpman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		bullet = new cBulletManager();
		type = Enemy;
		landing = false;
		move_flag = false;
		jump_count = 0;
		move_dir = 1;
	}
	~cEnemyJumpman() {
		delete bullet;
		bullet = nullptr;
	}
	
	void Update();
	void move();
	void MoveByAutomation();
};

class cEnemyGunman : public cEnemy
{
public:
	int attack_count;


	cEnemyGunman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		bullet = new cBulletManager();
		type = Enemy;
		landing = false;

		attack_count = 0;
	}

	void Update();
	void move();
	void MoveByAutomation();
};

class cEnemyCannon : public cEnemy
{
public:
	int attack_count;


	cEnemyCannon(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		bullet = new cBulletManager();
		type = Enemy;
		landing = false;

		attack_count = 0;
	}

	void Update();
	void move();
	void MoveByAutomation();
};


class cCharacterManager {
protected:
public:
	cPlayer *player;
	cEnemyJumpman *jumpman;
	cEnemyGunman *gunman;
	cEnemyCannon * cannon;
	cCharacterManager() {
		player = new cPlayer(400.f, 100.f, 90.f, 120.f, 6.f, true);
		jumpman = new cEnemyJumpman(300.f, 100.f, 90.f,120.f, 2.f, false);
		gunman = new cEnemyGunman(200.f, 100.f, 90.f, 120.f, 2.f, false);
		cannon = new cEnemyCannon(200.f, 100.f, 90.f, 120.f, 2.f, false);

	}
	~cCharacterManager() {
		delete player;
		player = nullptr;
	}
	void	Update();
	void	Render();
	cObject *GetPlayer() { return (cObject*)player; }
	cObject *GetEnemyJumpman() { return (cObject*)jumpman; }
	cObject *GetEnemyGunman() { return (cObject*)gunman; }
	cObject *GetEnemyCannon() { return (cObject*)cannon; }
};
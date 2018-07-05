#pragma once

extern VECTOR FocusPos;
extern VECTOR FocusOld;

class cCharacterBase : public cObject{
protected:

	float	speed;
	float	jump = 0.f, gravity = 1.f , inertia = 0; // 重力と慣性
	bool	possess;	// 乗り移られているか
	void	Physical();	// ジャンプとかの計算

	// 着地した時の判定 ------------------
	bool	landing;

	// ワイヤーが触れた時の判定-----------
	bool	wirehit;

public:
	cCharacterBase() {};
	~cCharacterBase() {};
	void	HitAction(cObject *hit);
	void	Collision(cObject *hit);
	void	MoveByAutomation();
	void	MoveByPlayer();
	void	Update();
	void	Render();
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
		pos		= { x, y, 0.f };
		size	= { w, h, 0.f };
		speed	= s;
		possess = p;
		bullet	= new cBulletManager();
		type	= Enemy;
		landing = false;

		attack_count = 0;
	}
	void move();
	void MoveByAutomation();
};

class cEnemyCannon : public cEnemy
{
public:
	int attack_count;


	cEnemyCannon(float x, float y, float w, float h, float s, bool p) {
		pos		= { x, y, 0.f };
		size	= { w, h, 0.f };
		speed	= s;
		possess = p;
		bullet	= new cBulletManager();
		type	= Enemy;
		landing = false;

		attack_count = 0;
	}
	void move();
	void MoveByAutomation();
};

class cEnemyHardBody : public cEnemy{
public:
	float move_speed;
	int life;
	int attack_time;
	bool attack_flag;
	int jump_count;
	int cool_time;

	cEnemyHardBody(float x, float y, float w, float h, float s, bool p) {
		pos			= { x, y, 0.f };
		size		= { w, h, 0.f };
		speed		= s;
		possess		= p;
		bullet		= new cBulletManager();
		landing		= false;
		type		= Enemy;
		move_speed	= 0.8f;
		life		= 100;
		attack_time = 0;
		attack_flag = false;
		jump_count  = 0;
		cool_time	= 0;
	}
	void Update();
	void move();
	void MoveByAutomation();
};

class cEnemyWireman : public cEnemy {
public:
	VECTOR  wirepos;
	float move_speed;		// ワイヤーを伸ばしているときのキャラのスピード
	float wire_length;		// ワイヤーの長さ
	float wire_gravity;		// ワイヤーを伸ばしているときの重力
	float rot;				// 初期プレイヤー角度
	float filing_angle;		// ワイヤー発射角度（45度）
	bool start_wire;		// ワイヤーで動いているか
	int action_count;
	int dir;				// 1,右 -1,左


	cEnemyWireman(float x, float y, float w, float h, float s, bool p) {
		pos		= { x, y, 0.f };
		size	= { w, h, 0.f };
		speed	= s;
		possess = p;
		bullet	= new cBulletManager();
		type	= Enemy;
		landing = false;
		
		wirepos = { x,y,0.f };
		rot = 135.f;
		filing_angle = 45 * PI / 180;
		wire_gravity = 0.2f;
		wire_length = 100;
		move_speed = 4;
		start_wire = false;
		action_count = 100;
		dir = -1;
	}
	void Update();
	void move();
	void MoveByAutomation();
};


class cEnemyFryingman : public cEnemy {
public:

	float move_speed;
	float angle;
	int radian;
	int length;
	int move_flow;		// 0,降下 1,回転 2,発砲 3,上昇（消える）
	int rotation_time;	// どのくらい回っているか

	cEnemyFryingman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		bullet = new cBulletManager();
		landing = false;
		type = Enemy;
		move_speed = 0.8f;
		angle = 30.f;
		radian = 0;
		length = 180;
		move_flow = -1;
		rotation_time = 200;
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
	cEnemyHardBody * hardbody;
	cEnemyWireman *wireman;
	cEnemyFryingman *fryingman;

	cCharacterManager() {
		player = new cPlayer(400.f, 100.f, 90.f, 120.f, 6.f, true);
		jumpman = new cEnemyJumpman(300.f, 100.f, 90.f,120.f, 2.f, false);
		gunman = new cEnemyGunman(200.f, 100.f, 90.f, 120.f, 2.f, false);
		cannon = new cEnemyCannon(200.f, 100.f, 90.f, 120.f, 2.f, false);
		hardbody = new cEnemyHardBody(1000.f, 100.f, 90.f, 120.f, 2.f, false);
		wireman = new cEnemyWireman(200.f, 100.f, 100.f, 100.f, 2.f, false);
		fryingman = new cEnemyFryingman(500.f, -100.f, 90.f, 90.f, 2.f, false);
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
	cObject *GetEnemyHardBody() { return (cObject*)hardbody; }
	cObject *GetEnemyWireman() { return (cObject*)wireman; }
	cObject *GetEnemyFryingman() { return (cObject*)fryingman; }

};
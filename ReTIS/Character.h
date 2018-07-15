#pragma once

extern VECTOR FocusPos;
extern VECTOR FocusOld;
extern VECTOR FocusCam;
extern VECTOR WirePos;
extern int AnchorStretch;
class cCharacterBase : public cObject{
protected:
	float	speed;
	float	jump = 0.f, gravity = 1.f , inertia = 0; // 重力と慣性
	int		hp = 5, invincible_time = 0;
	int		jump_count = 0;
	bool	invincible = false;
	void	Physical();	// ジャンプとかの計算

	// 着地した時の判定 ------------------
	bool	landing;

	// ワイヤーが触れた時の判定-----------
	bool	wirehit;

	// 何かにあたったとき-----------------
	bool	ceiling;

public:
	cCharacterBase() {};
	~cCharacterBase() {
	};
	bool	possess;	// 乗り移られているか
	void	HitAction(cObject *hit);
	void	Collision(cObject *hit);
	void	MoveByAutomation();
	void	MoveByPlayer();
	void	Damaged();
	void	Update();
	void	Render();
	int		GetHp() { return hp; }
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
public:
	/*cEnemy(float x, float y, float w, float h, float s, bool p) {
		pos     = { x, y, 0.f };
		size    = { w, h, 0.f };
		speed   = s;
		possess = p;
		bullet  = new cBulletManager();
		type    = Enemy;
	}*/
	~cEnemy() {}
	void	Render();
};

class cEnemyJumpman : public cEnemy 
{
protected:
public:

	float rad;
	bool move_flag;
	int jump_count;
	int move_dir;

	cEnemyJumpman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Enemy;
		landing = false;
		move_flag = false;
		jump_count = 0;
		move_dir = 1;
	}
	~cEnemyJumpman() {}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
};

class cEnemyGunman : public cEnemy
{
protected:
public:
	int attack_count;


	cEnemyGunman(float x, float y, float w, float h, float s, bool p) {
		pos		= { x, y, 0.f };
		size	= { w, h, 0.f };
		speed	= s;
		possess = p;
		type	= Enemy;
		landing = false;

		attack_count = 0;
	}
	~cEnemyGunman() {}

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
		type	= Enemy;
		landing = false;

		attack_count = 0;
	}
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
	void MoveByPlayer();
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
	bool now_wire;			//
	int action_count;
	int dir;				// 1,右 -1,左
	int move_pattern;
	int wire_count;
	int count;
	float Wire_and_Player;
	

	cEnemyWireman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Enemy;
		landing = false;

		wirepos = { x,y,0.f };
		rot = 90.f;
		filing_angle = 45 * PI / 180;
		wire_gravity = 0.4f;
		wire_length = 100;
		move_speed = 6;
		start_wire = false;
		action_count = 5;
		dir = -1;
		now_wire = false;
		wire_count = 0;
		count = 0;
	}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
	void WireRender();
};

class cEnemyWireAnchor : public cEnemy
{
public:
	VECTOR  wirepos;

	bool dir;
	float wire_angle;
	float wire_speed;
	float move_speed;
	int move_pattern;
	int count;

	cEnemyWireAnchor(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Enemy;
		landing = false;
		count = 0;

		wirepos = { x,y,0.f };
	}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
	void WireRender();

};

/*
class cEnemyWireman : public cEnemy {
public:
VECTOR  wirepos;
float move_speed;		// ワイヤーを伸ばしているときのキャラのスピード
float wire_length;		// ワイヤーの長さ
float wire_gravity;		// ワイヤーを伸ばしているときの重力
float rot;				// 初期プレイヤー角度
float filing_angle;		// ワイヤー発射角度（45度）
bool start_wire;		// ワイヤーで動いているか
bool now_wire;			//
int action_count;
int dir;				// 1,右 -1,左
int move_pattern;


cEnemyWireman(float x, float y, float w, float h, float s, bool p) {
pos		= { x, y, 0.f };
size	= { w, h, 0.f };
speed	= s;
possess = p;
type	= Enemy;
landing = false;

wirepos = { x,y,0.f };
rot = 90.f;
filing_angle = 45 * PI / 180;
wire_gravity = 0.4f;
wire_length = 100;
move_speed = 6;
start_wire = false;
action_count = 5;
dir = -1;
now_wire = false;
}
void Update();
void MoveByPlayer();
void MoveByAutomation();
void WireRender();
};
*/


class cEnemyFryingman : public cEnemy {
protected:
public:

	VECTOR bulletpos;
	VECTOR bulletsize;

	float move_speed;
	float angle;
	int radian;
	int length;
	int move_flow;		// 0,降下 1,回転 2,発砲 3,上昇（消える）
	int rotation_time;	// どのくらい回っているか
	bool firing;		
	int time_sub;
	float lockon;



	cEnemyFryingman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		landing = false;
		type = Enemy;
		move_speed = 0.8f;
		angle = 30.f;
		radian = 0;
		length = 180;
		move_flow = -1;
		rotation_time = 200;
		firing = false;
		bulletsize = {50,50,0};
	}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
};

class cCharacterManager {
protected:
	int		possess_time = 0;
public:
	cPlayer			*player;
	cEnemyJumpman	*jumpman[ENEMY_MAX];
	cEnemyHardBody	*hardbody[ENEMY_MAX];
	cEnemyWireman	*wireman[ENEMY_MAX];
	cEnemyFryingman *fryingman[ENEMY_MAX];
	cEnemyWireAnchor*wireanchor[ENEMY_MAX];

	cCharacterManager() {
		player		 = new cPlayer(400.f, 100.f, 90.f, 120.f, 6.f, true);
		jumpman[0]	 = new cEnemyJumpman(300.f, 100.f, 90.f,120.f, 2.f, false);
		hardbody[0]	 = new cEnemyHardBody(1000.f, 100.f, 90.f, 120.f, 2.f, false);
		wireman[0]	 = new cEnemyWireman(300.f, 100.f, 90.f, 100.f, 2.f, false);
		fryingman[0] = new cEnemyFryingman(500.f, -100.f, 90.f, 90.f, 2.f, false);
		wireanchor[0] = new cEnemyWireAnchor(100, -100, 10, 10, 2, false);
	}
	~cCharacterManager() {
		delete player;
		player = nullptr;
		for (int i = 0; i < ENEMY_MAX; i++) {
			delete jumpman[i];
			delete hardbody[i];
			delete wireman[i];
			delete fryingman[i];
			jumpman[i]   = nullptr;
			hardbody[i]  = nullptr;
			wireman[i]	 = nullptr;
			fryingman[i] = nullptr;
		}
	}
	void	Update();
	void	Render();
	void	PossessListener();
	cObject *GetPlayer()                { return (cObject*)player; }
	cObject *GetEnemyJumpman(int num)   { return (cObject*)jumpman[num];   }
	cObject *GetEnemyHardBody(int num)  { return (cObject*)hardbody[num];  }
	cObject *GetEnemyWireman(int num)   { return (cObject*)wireman[num];   }
	cObject *GetEnemyFryingman(int num) { return (cObject*)fryingman[num]; }
	cObject *GetEnemyWireAnchor(int num) { return (cObject*)wireanchor[num]; }
	cObject *WireAnchor(int num)		{ return (cObject*)wireanchor[num]; }

	int		GetPlayerHp() { return player->GetHp(); }

};
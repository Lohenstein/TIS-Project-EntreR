#pragma once

extern VECTOR FocusPos;
extern VECTOR FocusOld;
extern VECTOR FocusCam;
extern VECTOR MouseAdd;
extern bool	  IsClearFlag, IsOverFlag;
extern int mp;

extern int coin, ecoin, rcoin;

using namespace std;

class cCharacterBase : public cObject {
protected:
	float	speed;
	float	jump = 0.f, gravity = 1.f, inertia = 0; // 重力と慣性
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

	// 足場の下に当たっているかの判定-----
	bool	bottomhit;
	bool	righthit;
	bool	lefthit;


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
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Player;
		IsClearFlag = false;
		IsOverFlag  = false;
		mp = 150;
		coin = 0, ecoin = 0, rcoin = 0;
	}
	~cPlayer() {}
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
	bool direction;

	bool attack_flag;
	int image_change;

	int move_pattern;

	VECTOR player_attack;

	cEnemyJumpman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Enemy;
		landing = false;
		hp = 1;

		move_flag = false;
		jump_count = 0;
		direction = true;
		attack_flag = false;
		image_change = 0;
	}
	~cEnemyJumpman() {}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
	void Render(int image[120]);
};

class cEnemyGunman : public cEnemy
{
protected:
public:
	int move_pattern;
	int attack_count;
	bool attack;
	VECTOR bulletsize;
	VECTOR bulletpos;
	int image_change;
	bool bullet_fire;

	bool direction;
	int player_move_pattern;
	bool attack_flag;
	float lockon;

	cEnemyGunman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Enemy;
		landing = false;
		hp = 1;

		attack_count = 0;
		bulletsize = { 50,50,0 };
		bulletpos = { 0,0,0 };
		image_change = 0;
		move_pattern = 0;
		bullet_fire = false;

		direction = true;
		player_move_pattern = 0;
		attack_count = false;
		lockon = 0.f;
	}
	~cEnemyGunman() {}

	void MoveByAutomation();
	void MoveByPlayer();
	void Update();
	void Render(int image[]);
};


class cEnemyHardBody : public cEnemy {
public:
	float move_speed;
	int life;
	int attack_time;
	bool attack_flag;
	int jump_count;
	int cool_time;
	int image_change;

	cEnemyHardBody(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		landing = false;
		type = Enemy;
		hp = 6;
		
		move_speed = 0.8f;
		life = 100;
		attack_time = 0;
		attack_flag = false;
		jump_count = 0;
		cool_time = 0;
		image_change = 0;
	}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
};


class cEnemyWiremanManager : public cEnemy {
public:
	VECTOR WirePos;
	int AnchorStretch;
	int EnemyAnchorStretch;

	class Wireman : public cEnemy {
	public:
		int enemy_count;
		int EnemyAnchorStretch;

		int mouse_state;
		int mouse_posx;
		int mouse_posy;
		float move_speedx;
		float move_speedy;
		float anchor_speed;
		float wire_radian;
		float mouse_changeposx;
		float mouse_changeposy;

		int debug_int;
		int image_change;
		bool direction;			


		Wireman(float x, float y, float w, float h, float s, bool p) {
			pos = { x, y, 0.f };
			size = { w, h, 0.f };
			speed = s;
			possess = p;
			type = Enemy;
			landing = false;
			hp = 6;

			enemy_count = 0;

			mouse_state = 0;
			mouse_posx = 0;
			mouse_posy = 0;
			move_speedx = 0.f;
			move_speedy = 0.f;
			anchor_speed = 10.f;
			wire_radian = 0.f;
			mouse_changeposx = 0.f;
			mouse_changeposy = 0.f;
			debug_int = 0;
			image_change = 0;

			direction = true;
		}
		void Update(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch);
		void MoveByPlayer(VECTOR *WirePos, int *AnchorStretch);
		void MoveByAutomation(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch);
		void WireRender(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch);
		void MouseStateGet();
		void Render(int image[],int *AnchorStretch, int *EnemyAnchorStretch);
	};

	class Anchor : public cEnemy
	{
	public:
		VECTOR  wirepos;

		bool dir;
		float wire_angle;
		float wire_speed;
		float move_speed;
		int move_pattern;
		int count;

		Anchor(float x, float y, float w, float h, float s, bool p) {
			pos = { x, y, 0.f };
			size = { w, h, 0.f };
			speed = s;
			possess = p;
			type = Enemy;
			landing = false;
			count = 0;

			wirepos = { x,y,0.f };
		}
		void Update(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch);
		void MoveByAutomation(VECTOR *WirePos, int *AnchorStretch);
	};
};


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
	int image_change;

	cEnemyFryingman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		landing = false;
		type = Enemy;
		hp = 2;
		
		move_speed = 0.8f;
		angle = 30.f;
		radian = 0;
		length = 180;
		move_flow = -1;
		rotation_time = 200;
		firing = false;
		bulletsize = { 50,50,0 };
		image_change = 0;
	}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
	void Render(int image[]);
};


class cEnemyBossmiddle :public cEnemy {
protected:
public:
	int move_time;
	int move_pattern;
	float move_speed;

	VECTOR Player_old;
	VECTOR Attack_pos;

	bool direction;
	int image_change;

	cEnemyBossmiddle(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		landing = false;
		type = Enemy;
		hp = 5;

		move_time = 0;
		move_pattern = 0;
		move_speed = 1.5f;
		image_change = 0;
		direction = false;
	}
	void Update();
	void MoveByAutomation();
	void Render(int image[]);
};

class cEnemyCircularSaw :public cEnemy {
public:
	int		now_point;
	bool	flag;
	float	p;
	float	sx[2] = { 0, 0 }, sy[2] = { 0, 0 };
	int		image_change;

	// コンストラクタ
	cEnemyCircularSaw(int w, int h, int x1, int y1, int x2, int y2) {
		flag = true;
		size = { (float)w, (float)h, 0.f };
		p = 0.f;
		now_point = 0;
		type = Enemy;
		possess = false;

		SetPoint(0, x1, y1);
		SetPoint(1, x2, y2);
		image_change = 0;
	}
	void	Update(float s, int p1, int p2);
	void	SetPoint(int num, float x, float y);
	void	HitAction(cObject *hit) {};
	void	Render(int image[]);
};

class cEnemyCannon : public cEnemy
{
public:
	int attack_count;
	int move_pattern;
	float angle;

	int image_change;
	bool direction;
	VECTOR bulletpos;
	VECTOR bulletsize;

	cEnemyCannon(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Enemy;
		landing = false;

		bulletsize = { 50,50,0 };
		angle = 0.f;
		image_change = 0;
		attack_count = 0;
		direction = false;
		move_pattern = 0;
	}
	void Update();
	void Render(int image[]);
	void MoveByAutomation();
};

class cMoveFloor : public cEnemy {
public:
	int		now_point;
	bool	flag;
	float	p;
	float	sx[2] = { 0, 0 }, sy[2] = { 0, 0 };

	// コンストラクタ
	cMoveFloor(int w, int h, int x1, int y1, int x2, int y2) {
		flag = true;
		size = { (float)w, (float)h, 0.f };
		p = 0.f;
		now_point = 0;
		type = MoveFloor;
		possess = false;

		SetPoint(0, x1, y1);
		SetPoint(1, x2, y2);
	}
	void	Render();
	void	Update(float s, int p1, int p2);
	void	SetPoint(int num, float x, float y);
	void	HitAction(cObject *hit) {};
};

class cDropFloor : public cEnemy {
protected:
	int		time;
	bool	flag;
	float	sx, sy;
	float	gravity = 1.f;
	float	drop_p = 0.f;
public:
	cDropFloor(int w, int h, int x, int y) {
		flag = false;
		size = { (float)w, (float)h, 0.f };
		SetPoint(x, y);
		time = 0;
		type = DropFloor;
		possess = false;
	}
	void	Render();
	void	Update();
	void	SetPoint(float x, float y);
	void	HitAction(cObject *hit);
};

class cClearCollision : public cEnemy {
public:
	bool IsClear = false; // クリア判定
	cClearCollision(int x, int y) {
		size = { 96.f, 128.f, 0.f };
		pos  = { (float)x, (float)y, 0.f };
		type = Clear;
	}
	void	HitAction(cObject *hit) {
		if (hit->GetType() == Player) IsClear = true;
	}
	void	DebugRender() {
		DrawBoxAA(pos.x - GetSize().x / 2.f, pos.y - GetSize().y / 2.f,
			pos.x + GetSize().x / 2.f, pos.y + GetSize().y / 2.f,
			0xFFFFFF, false);
	}
};

class cCoin : public cEnemy {
public:
	bool getcoin;
	int  cointype;

	VECTOR CoinPos;
	cCoin(int x, int y, int ctype) {
		pos = { (float)x, (float)y, 0.f };
		cointype = ctype;
		switch (ctype) {
		case 0: // 普通のコイン
			type = NormalCoin;
			size = { 32.f, 32.f, 0.f };
			break;
		case 1: // エネルギーコイン
			type = EneCoin;
			size = { 32.f, 32.f, 0.f };
			break;
		case 2: // レアコイン
			type = RareCoin;
			size = { 64.f, 64.f, 0.f };
			break;
		}
		hp = 1;
	}
	void	Update();
	void	Render(int image[]);
	void	HitAction(cObject *hit) {
		hp = 0;
	}
};


class cCharacterManager {
protected:
	int		possess_time = 0;

public:
	// Character
	cPlayer							*player;
	cClearCollision					*clear;
	cEnemyJumpman					*jumpman[ENEMY_MAX];
	cEnemyHardBody					*hardbody[ENEMY_MAX];
	cEnemyWiremanManager::Wireman	*wireman[ENEMY_MAX];
	cEnemyFryingman					*fryingman[ENEMY_MAX];
	cEnemyWiremanManager::Anchor	*wireanchor[ENEMY_MAX];
	cEnemyWiremanManager			*wmanager[ENEMY_MAX];
	cEnemyGunman					*gunman[ENEMY_MAX];
	cEnemyBossmiddle				*bossmiddle[ENEMY_MAX];

	// Entity
	cEnemyCircularSaw				*circularsaw[ENEMY_MAX];
	cEnemyCannon					*cannon[ENEMY_MAX];
	cDropFloor						*dropfloor[ENEMY_MAX];
	cMoveFloor						*movefloor[ENEMY_MAX];
	cCoin							*coin[ENEMY_MAX];

	int		wireman_img[273];
	int		jumpman_img[120];
	int		bossmiddle_img[200];
	int		fryingman_img[123];
	int		gunman_img[234];
	int		circularsaw_img[5];
	int		cannon_img[20];
	int		coin_img[3];

	void	Update();
	void	Render();
	void	PossessListener();
	void	LoadCharacters(string name);
	void	DeleteCharacters();
	void	DeleteDeathCharacters();
	cCharacterManager(string name) {
		/*
		wireman[0]	  = new cEnemyWiremanManager::Wireman(300.f, 100.f, 80.f, 220.f, 2.f, false);
		wireanchor[0] = new cEnemyWiremanManager::Anchor(100, -100, 10, 10, 2, false);
		wmanager[0]   = new cEnemyWiremanManager;
		*/
		LoadCharacters(name);
		LoadDivGraph("data/img/enemy/Jumpman.PNG", 120, 30, 4, 300, 300, jumpman_img);
		LoadDivGraph("data/img/enemy/Gunman.PNG", 234, 39, 6, 300, 300, gunman_img);
		LoadDivGraph("data/img/enemy/Fryingman.PNG", 123, 41, 3, 300, 300, fryingman_img);
		LoadDivGraph("data/img/enemy/Bossmiddle.PNG", 200, 50, 4, 300, 300, bossmiddle_img);
		LoadDivGraph("data/img/enemy/Wireman.PNG", 273, 39, 7, 300, 300, wireman_img);
		LoadDivGraph("data/img/enemy/CircularSaw.PNG", 5, 5, 1, 300, 300, circularsaw_img);
		LoadDivGraph("data/img/enemy/BigGun.PNG", 20, 20, 1, 300, 300, cannon_img);
		coin_img[0] = LoadGraph("data/img/enemy/NormalCoin.PNG");
		coin_img[1] = LoadGraph("data/img/enemy/BigCoin.PNG");
		coin_img[2] = LoadGraph("data/img/enemy/EnergyCoin.PNG");
	}
	~cCharacterManager() {
		DeleteCharacters();
	}

	cObject *GetPlayer() { return (cObject*)player; }
	cObject *GetClear() { return (cObject*)clear; }
	cObject *GetEnemyJumpman(int num) { return (cObject*)jumpman[num]; }
	cObject *GetEnemyHardBody(int num) { return (cObject*)hardbody[num]; }
	cObject *GetEnemyWireman(int num) { return (cObject*)wireman[num]; }
	cObject *GetEnemyFryingman(int num) { return (cObject*)fryingman[num]; }
	cObject *GetEnemyWireAnchor(int num) { return (cObject*)wireanchor[num]; }
	cObject *WireAnchor(int num) { return (cObject*)wireanchor[num]; }
	cObject *GetEnemyGunman(int num) { return (cObject*)gunman[num]; }
	cObject *GetEnemyBossmiddle(int num) { return (cObject*)bossmiddle[num]; }
	cObject *GetCircularSaw(int num) { return (cObject*)circularsaw[num]; }
	cObject *GetCannon(int num) { return (cObject*)cannon[num]; }
	cObject *GetDropFloor(int num) { return (cObject*)dropfloor[num]; }
	cObject *GetMoveFloor(int num) { return (cObject*)movefloor[num]; }
	cObject *GetCoin(int num) { return (cObject*)coin[num]; }

	int		GetPlayerHp() { return player->GetHp(); }

};

enum character {
	eCharacter,		// 0
	ePlayer,		// 1
	eJumpman,		// 2
	eHardbody,		// 3
	eFryingman,		// 4
	eGunman,		// 5
	eBossmiddle,	// 6
	eCircularsaw,	// 7
	eCannon,		// 8
	eMoveFloor,		// 9
	eDropFloor,		// 10
	eClear,			// 11(クリア判定)
	eCoin			// 12
};
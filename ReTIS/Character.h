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
	bool	rect = false;
	int		anim = 0;
	int		animjump = 0;
	int		animmode = 0;
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
	int img[4][30];
public:
	cAnchor *anchor;
	float	rad2anchor, dis2anchor, wrad, swing;
	bool	IsAnchored = false;
	VECTOR	savepos;
	cPlayer(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		type = Player;
		IsClearFlag = false;
		IsOverFlag  = false;
		mp = 300;
		coin = 0, ecoin = 0, rcoin = 0;
		LoadDivGraph("data/img/amecha/walk.png", 30, 1, 30, 606, 551, img[0]);
		LoadDivGraph("data/img/amecha/idol.png", 30, 1, 30, 606, 544, img[1]);
		LoadDivGraph("data/img/amecha/jump.png", 30, 1, 30, 606, 558, img[2]);
		LoadDivGraph("data/img/amecha/shot.png", 30, 1, 30, 606, 544, img[3]);
	}
	~cPlayer() {
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 4; j++) {
				DeleteGraph(img[j][i]);
			}
		}
	}
	void	UpdateAnchor();
	void	Render();
	void	Update();
	void	HitAction(cObject *hit);
	cObject *GetAnchor() { return (cObject*)anchor; }
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
		type = Enemy;
		landing = false;
		hp = 2;

		attack_count = 0;
		bulletsize = { 10,10,0 };
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
	void Render(int image[],int imagedead[]);
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
	int cooltime;	
	bool firing;
	int time_sub;
	float lockon;
	int image_change;

	cEnemyFryingman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		landing = false;
		type = Enemy;
		hp = 2;
		
		move_speed = 0.8f;
		angle = 30.f;
		radian = 0;
		length = 180;
		move_flow = 0;
		cooltime = 0;
		firing = false;
		bulletsize = { 50,50,0 };
		image_change = 0;
	}
	void Update();
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

	int change;
	bool direction;
	int image_change;

	cEnemyBossmiddle(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		landing = false;
		type = Enemy;
		hp = 2;

		move_time = 0;
		move_pattern = 0;
		move_speed = 1.5f;
		image_change = 0;
		direction = false;
		change = 0;
	}
	void Update();
	void MoveByAutomation();
	void Render(int image[]);
};

class cEnemyJugem :public cEnemy {
protected:
public:
	bool direction;
	int image_change;

	short move_pattern;
	short count;

	cEnemyJugem(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		landing = false;
		type = Enemy;
		hp = 5;

		image_change = 0;
		direction = false;
		move_pattern = 0;
		direction = false;
		count = 0;
	}
	void Update();
	void MoveByAutomation();
	void Render(int image[]);
};

class cEnemyBoss :public cEnemy {
protected:
public:
	bool direction;
	int image_change;

	short enemy_move;
	short attack_count;
	VECTOR attackpos[5];
	float lockon;
	cEnemyBoss(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		landing = false;
		type = Enemy;
		hp = 5;

		image_change = 0;
		direction = false;
		enemy_move = 0;
		attack_count = 0;
		for (int i = 0; i < 4; i++) {
			attackpos[i] = {0,0,0};
		}
		lockon = 0.f;
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

class cEventsSwitch : public cEnemy{
public:
	int attack_count;
	int move_pattern;
	float angle;

	int image_change;
	bool direction;
	VECTOR bulletpos;
	VECTOR bulletsize;

	cEventsSwitch(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		type = Enemy;
		landing = false;
		hp = 2;

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

class cSpring : public cEnemy {
protected:
	bool	flag;
	float	sx, sy;
	int		num = 0;
public:
	cSpring(int x, int y) {
		pos = {(float)x, (float)y, 0.f};
		flag = false;
		size = { 250.f, 250.f, 0.f };
		type = Spring;
	}
	void	Render(int image[30]);
	void	Update();
	int		GetNum() { return num; }
	void	HitAction(cObject *hit);
};

class cCharacterManager {
protected:
	int		possess_time = 0;

public:
	// Character
	cPlayer							*player;
	cClearCollision					*clear;
	cEnemyHardBody					*hardbody[ENEMY_MAX];
	////cEnemyWiremanManager::Wireman	*wireman[ENEMY_MAX];
	cEnemyFryingman					*fryingman[ENEMY_MAX];
	//////cEnemyWiremanManager::Anchor	*wireanchor[ENEMY_MAX];
	//			*wmanager[ENEMY_MAX];
	cEnemyGunman					*gunman[ENEMY_MAX];
	cEnemyJumpman					*jumpman[ENEMY_MAX];
	cEnemyBossmiddle				*bossmiddle[ENEMY_MAX];
	cEnemyJugem						*jugem[ENEMY_MAX];
	cEnemyBoss						*boss[ENEMY_MAX];

	// Entity
	cEnemyCircularSaw				*circularsaw[ENEMY_MAX];
	cEnemyCannon					*cannon[ENEMY_MAX];
	cDropFloor						*dropfloor[ENEMY_MAX];
	cMoveFloor						*movefloor[ENEMY_MAX];
	cCoin							*coin[ENEMY_MAX];
	cEventsSwitch					*eventswitch;
	cSpring							*spring[ENEMY_MAX];


	int		wireman_img[273];
	int		jumpman_img[120];
	int		bossmiddle_img[200];
	int		fryingman_img[123];
	int		gunman_imgdead[234];
	int		gunman_img[111];
	int		circularsaw_img[5];
	int		cannon_img[20];
	int		coin_img[3];
	int		spring_img[30];
	int		floorimg;

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
		LoadCharacters(name);//24 5
		LoadDivGraph("data/img/enemy/Jumpman.PNG", 120, 30, 4, 300, 300, jumpman_img);
		LoadDivGraph("data/img/enemy/Gunman.PNG", 234, 39, 6, 300, 300, gunman_imgdead);
		LoadDivGraph("data/img/enemy/GUNMAN1.PNG", 111, 37, 6, 300, 300, gunman_img);
		LoadDivGraph("data/img/enemy/Fryingman.PNG", 123, 41, 3, 300, 300, fryingman_img);
		LoadDivGraph("data/img/enemy/Bossmiddle.PNG", 200, 50, 4, 300, 300, bossmiddle_img);
		LoadDivGraph("data/img/enemy/Wireman.PNG", 273, 39, 7, 300, 300, wireman_img);
		LoadDivGraph("data/img/enemy/CircularSaw.PNG", 5, 5, 1, 300, 300, circularsaw_img);
		LoadDivGraph("data/img/enemy/BigGun.PNG", 20, 20, 1, 300, 300, cannon_img);
		coin_img[0] = LoadGraph("data/img/enemy/NormalCoin.PNG");
		coin_img[1] = LoadGraph("data/img/enemy/BigCoin.PNG");
		coin_img[2] = LoadGraph("data/img/enemy/EnergyCoin.PNG");
		LoadDivGraph("data/img/enemy/spring.png", 30, 30, 1, 250, 250, spring_img);
	}
	~cCharacterManager() {
		DeleteCharacters();
		for (int i = 0; i < 273; i++) { DeleteGraph(wireman_img[i]); }
		for (int i = 0; i < 120; i++) { DeleteGraph(jumpman_img[i]); }
		for (int i = 0; i < 200; i++) { DeleteGraph(bossmiddle_img[i]); }
		for (int i = 0; i < 123; i++) { DeleteGraph(fryingman_img[i]); }
		for (int i = 0; i < 234; i++) { DeleteGraph(gunman_img[i]); }
		for (int i = 0; i <   5; i++) { DeleteGraph(circularsaw_img[i]); }
		for (int i = 0; i <  20; i++) { DeleteGraph(cannon_img[i]); }
		for (int i = 0; i <   3; i++) { DeleteGraph(coin_img[i]); }
		DeleteGraph(floorimg);
	}

	cObject *GetPlayer() { return (cObject*)player; }
	cObject *GetAnchor() { return (cObject*)player->GetAnchor(); }
	cObject *GetClear() { return (cObject*)clear; }
	cObject *GetEnemyJumpman(int num) { return (cObject*)jumpman[num]; }
	cObject *GetEnemyHardBody(int num) { return (cObject*)hardbody[num]; }
	cObject *GetEnemyFryingman(int num) { return (cObject*)fryingman[num]; }
	cObject *GetEnemyGunman(int num) { return (cObject*)gunman[num]; }
	cObject *GetEnemyBossmiddle(int num) { return (cObject*)bossmiddle[num]; }
	cObject *GetCircularSaw(int num) { return (cObject*)circularsaw[num]; }
	cObject *GetCannon(int num) { return (cObject*)cannon[num]; }
	cObject *GetDropFloor(int num) { return (cObject*)dropfloor[num]; }
	cObject *GetMoveFloor(int num) { return (cObject*)movefloor[num]; }
	cObject *GetCoin(int num) { return (cObject*)coin[num]; }
	cObject *GetJugem(int num) { return (cObject*)jugem[num]; }
	cObject *GetBoss(int num) { return (cObject*)boss[num]; }
	cObject *GetSpring(int num) { return (cObject*)spring[num]; }

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
	eCoin,			// 12
	eBoss,			// 13
	eEvents,		// 14
	eJugem,			// 15
	eSpring			// 16
};

// 最大　＋ 14
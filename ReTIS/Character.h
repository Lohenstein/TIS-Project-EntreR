#pragma once

extern int	  anchorimg, wireimg;
extern VECTOR FocusPos;
extern VECTOR FocusOld;
extern VECTOR FocusCam;
extern VECTOR MouseAdd;
extern bool	  IsClearFlag, IsOverFlag, IsBended[120];
extern int mp;

extern int coin, ecoin, rcoin;

using namespace std;

class cCharacterBase :  public cObject {
protected:
	float	jump = 0.f, gravity = 1.f, inertia = 0; // 重力と慣性
	int		hp = 5, invincible_time = 0;
	int		jump_count = 0;
	bool	rect = false;
	int		anim = 0;
	int		animjump = 0;
	int		animmode = 0;
	bool	invincible = false;
	int		effect_hnd;
	void	Physical();	// ジャンプとかの計算
	short	count = 0;
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
	void	SetEffect(float zoom, int effect_num);
	void	UpdateEffect();
	void	Update();
	void	Render();
	int		GetHp() { return hp; }
};

class cPlayer : public cCharacterBase {
protected:
	int img[4][30];
	bool springon;
public:
	cAnchor		*anchor;
	cAnchorWire *anchorwire[120];
	VECTOR		bend_save[120];

	float	rad2anchor, dis2anchor, wrad, wrad_old, swing;
	bool	IsAnchored = false;
	bool	IsFall = false;
	bool	IsFrying = false;
	bool	addtimeswitch = false;
	int 	anchor_dir = 0;
	int		save_speed;
	float	save_distance;
	VECTOR	savepos;
	cPlayer(float x, float y, float w, float h, float s, bool p) {
		pos			= { x, y, 0.f };
		size		= { w, h, 0.f };
		speed		= s;
		type		= Player;
		IsClearFlag = false;
		IsOverFlag  = false;
		mp			= 300;
		count		= 0;
		springon	= false;
		coin = 0, ecoin = 0, rcoin = 0;
		LoadDivGraph("data/img/amecha/walk.png", 30, 1, 30, 606, 551, img[0]);
		LoadDivGraph("data/img/amecha/idol.png", 30, 1, 30, 606, 544, img[1]);
		LoadDivGraph("data/img/amecha/jump.png", 30, 1, 30, 606, 558, img[2]);
		LoadDivGraph("data/img/amecha/shot.png", 30, 1, 30, 606, 544, img[3]);
		wireimg   = LoadGraph("data/img/parts/wire.png");
		anchorimg = LoadGraph("data/img/parts/anchor.png");
		for (int i = 0; i < 120; i++) {
			bend_save[i] = { -1.f, -1.f, -1.f };
		}
	}
	~cPlayer() {
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 4; j++) {
				DeleteGraph(img[j][i]);
			}
		}
	}
	void	UpdateAnchor();
	void	DetachAnchor();
	void	Render();
	void	Update();
	void	BendWire();
	void	HitAction(cObject *hit);
	/*void	HitCheck(cGame &main, cObject *anc) {
		main.CollisionAroundMaptile(anc);
	}*/
	cObject *GetAnchor() { return (cObject*)anchor; }
	cObject *GetAnchorWire(int num) { return (cObject*)anchorwire[num]; }
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
		hp = 3;

		move_flag = false;
		jump_count = 0;
		direction = true;
		attack_flag = false;
		image_change = 0;
	}
	~cEnemyJumpman() {}
	void Update();
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
	float handradian;

	cEnemyGunman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		type = Enemy;
		landing = false;
		hp = 3;

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
		handradian = 0;
	}
	~cEnemyGunman() {}

	void MoveByAutomation();
	void Update();
	void Render(int image[],int hand[],int gunhand[]);
};

class cEnemyHardBody : public cEnemy {
protected:
	bool attack_flag;
	short count;
	short image_change;
	short move_pattern;
	bool direction;
	short rolling_count;
	float rectx;
	float recty;
	float lockon;
	
public:
	VECTOR bulletpos;
	VECTOR bulletsize;
	cEnemyHardBody(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		landing = false;
		type = Enemy;
		hp = 1;		//hpが2以上のとき倒れなくなる
		
		rectx = x;
		recty = y;
		rad = 45;
		attack_flag = false;
		jump_count = 0;
		count = 0;
		image_change = 0;
		direction = false;
		bulletsize = { 10,10,0 };
		rolling_count = 0;
		angle = 45;
		move_pattern = 0;
		lockon = 0.f;
	}
	//void HitAction(cObject *hit);
	void Update();
	void MoveByAutomation();
	void Render(int img[]);
	float GetPosx() { return rectx; }
	float GetPosy() { return recty; }
	float GetRadrightbottom()  { return cos(rad); }
	float GetRadleftbottom() { 
		rad = (float)d2r(angle * 2);
		return cosf(rad);
	}
	float GetRadlefttop() {
		rad = (float)d2r(angle * 3);
		return sinf(rad);
	}
	float GetRadrighttop() {
		rad = (float)d2r(angle * 4);
		return sinf(rad);
	}
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
	void HitAction(cObject *hit);
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
		hp = 5;

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
	float move_radian;

	int move_number;
	short move_pattern;
	short count;
	float lockon;
	VECTOR bulletsize;
	int		bulletspeed;
	cEnemyJugem(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		landing = false;
		lefthit = false;
		righthit = false;
		type = Enemy;
		hp = 2;

		move_number = 0;
		move_radian = 0.f;
		image_change = 0;
		direction = false;
		move_pattern = 0;
		count = 0;
		lockon = 0.f;
		bulletsize = { 10,10,0 };
		bulletspeed = 15;
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
	VECTOR bulletsize;
	VECTOR BossPos;
	float lockon;
	int rad;

	cEnemyBoss(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		type = Enemy;
		hp = 1;

		image_change = 0;
		direction = false;
		enemy_move = 0;
		attack_count = 0;
		for (int i = 0; i < 4; i++) {
			attackpos[i] = {0,0,0};
		}
		lockon = 0.f;
		rad = 0;
		bulletsize = {10,10,0};
		//BossPos = size;
		//BossPos = VScale(pos, 0.1);
	}
	void Update();
	void MoveByAutomation();
	void Render(int model,int attachs);
	VECTOR GetBossPos() { return pos; }
	void BossMovel() { pos.x += 10; }
	void BossMover() { pos.y += 10; }
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

		SetPoint(0, (float)x1, (float)y1);
		SetPoint(1, (float)x2, (float)y2);
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

		SetPoint(0, (float)x1, (float)y1);
		SetPoint(1, (float)x2, (float)y2);
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
		SetPoint((float)x, (float)y);
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
	bool addtimeon;
	bool getcoin;
	int  cointype;
	int	 image_change;

	VECTOR CoinPos;
	cCoin(int x, int y, int ctype) {
		pos = { (float)x, (float)y, 0.f };
		cointype = ctype;
		//DebugMsgBox("%04d", (TCHAR)type);
		switch (cointype) {
		case 0: // 普通のコイン
			cointype = NormalCoin;
			type = NormalCoin;
			size = { 16.f, 16.f, 0.f };
			image_change = 39;
			break;
		case 1: // エネルギーコイン
			cointype = EneCoin;
			type = EneCoin;
			size = {16.f, 16.f, 0.f };
			image_change = 0;
			break;
		case 2: // レアコイン
			cointype = RareCoin;
			type = RareCoin;
			size = { 32.f, 32.f, 0.f };
			image_change = 78;
			break;
		case 3:
			cointype = TimeCoin;
			type = TimeCoin;
			size = { 16.f,16.f,0.f };
			image_change = 0;
			break;
			addtimeon = false;
		}
		//DebugMsgBox("%04d", (TCHAR)type);
		hp = 1;
		addtimeon = false;
	}
	void	Update();
	void	Render(int coin[],int Time[],int Chocolate[]);
	void	MoveByAutomation();
	void	HitAction(cObject *hit) {
		hp = 0;
		// mp = 300;
	}
};

class cCrumbleWall : public cEnemy{
public:
	int attack_count;
	int move_pattern;
	float angle;

	int image_change;
	bool direction;
	VECTOR bulletpos;
	VECTOR bulletsize;
	cCrumbleWall(float x, float y, float w, float h) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		type = Crumblewall;
		landing = false;
		hp = 2;

		image_change = 0;
	}
	void Update();
	void Render();
	void MoveByAutomation();
	void HitAction(cObject *hit);
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
		size = { 250.f / 2.f, 250.f / 2.f, 0.f };
		sx = 250 / 2.f, sy = 250 / 2.f;
		type = Spring;
	}
	void	Render(int image[30]);
	void	Update();
	int		GetNum() { return num; }
	void	HitAction(cObject *hit);
};

class cGear : public cEnemy {
protected:
	float	sx, sy;
	int		num = 0;
public:
	cGear(int x, int y) {
		pos = { (float)x, (float)y, 0.f };
		size = { 250.f / 2.f, 250.f / 2.f, 0.f };
		sx = 250 / 2.f, sy = 250 / 2.f;
		type = Gear;
	}
	void	Render(int img[]);
	void	MoveByAutomation();
	void	Update();
	int		GetNum() { return num; }
};

class cWall :public cEnemy {
protected:
	int count;
public:
	cWall() {
		type = MapTile;
		count = 10;
		size = { 128,128,0 };
		pos = { 0,0,0 };
		hp = 5;
	}
	void Update(bool flag,VECTOR wallpos);
	//void Render();
	void Update();
};

class cMoveWall : public cEnemy {
protected:
	
	bool	flag;
	int		switch_num;
	int		wall_num;
	VECTOR wallpos;
	int		image_change;
public:
	cMoveWall(int x, int y, int sx, int sy) {
		pos = { (float)x, (float)y, 0.f };
		wallpos = { (float)sx,(float)sy,0.f };
		flag = false;
		size = { 300.f / 2.f, 300.f / 2.f, 0.f };
		sx = (int)(250 / 2.f), sy = (int)(250 / 2.f);
		type = NothingObject;
		hp = 2;

		switch_num = 0;
		wall_num = 0;
		image_change = 0;
	}

	void	RenderSwitch(int img[]);
	void	RenderWall(int hp);
	void	Update();
	void	MoveByAutomation();
	int 	GetHp() { return hp; }
	bool	GetFlag() { return flag; }
	VECTOR	GetWallPos() { return wallpos; }
	void	Switchon() {}
};


class cCharacterManager {
protected:
	int		possess_time = 0;

public:
	// Character
	cPlayer							*player;
	cClearCollision					*clear;
	cEnemyHardBody					*hardbody[ENEMY_MAX];
	cEnemyFryingman					*fryingman[ENEMY_MAX];
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
	cCrumbleWall					*crumblewall[ENEMY_MAX];
	cSpring							*spring[ENEMY_MAX];
	cGear							*gear[ENEMY_MAX];
	cMoveWall						*movewall[ENEMY_MAX];
	// cMoveWall::cWall				*wall[ENEMY_MAX];
	// こっちのwallを読み込んでいる
	cWall							*wall[ENEMY_MAX];

	int		wireman_img[273];
	int		jumpman_img[120];
	int		bossmiddle_img[200];
	int		hardbody_img[20];
	int		fryingman_img[123];
	int		gunman_img[148];
	int		gunman_handg[1];
	int		gunman_hand[1];
	int		circularsaw_img[5];
	int		cannon_img[19];
	int		spring_img[30];
	int		floorimg;
	int		boss_img[280];
	int		jugem_img[48];
	int		allcoin_img[117];
	int		chocolate_img[38];
	int		watch_img[39];
	int		switch_img[20];
	int		gear_img[13];
	int		boss_3d_down;
	int		boss_3d_beam;
	int		boss_3d_lockon;
	int		boss_3d_barrage;
	int		boss_3d_cleave;
	int		boss_move_3d[5];
	int		attachIndex = MV1AttachAnim(boss_3d_down, 0, -1, FALSE);
	float	playtime = 0.f;

	float angleY = 1.f;

	VECTOR ScreenPos;

	void	Update(int gettime);
	void	Render();
	void	PossessListener();
	void	LoadCharacters(string name);
	void	DeleteCharacters();
	void	DeleteDeathCharacters();
	void	BossRender();
	cCharacterManager(string name) {
		LoadCharacters(name);
		LoadDivGraph("data/img/enemy/Jumpman.PNG", 120, 30, 4, 300, 300, jumpman_img);
		LoadDivGraph("data/img/enemy/GUNMAN2.PNG", 148, 37, 4, 300, 300, gunman_img);
		LoadDivGraph("data/img/enemy/hand.PNG", 1,1,1, 300, 300, gunman_hand);
		LoadDivGraph("data/img/enemy/gunhand.PNG", 1,1,1, 300, 300, gunman_handg);
		LoadDivGraph("data/img/enemy/BigGun.PNG", 19,19, 1, 300, 300, cannon_img);
		LoadDivGraph("data/img/enemy/hardbody.PNG", 30, 15, 2, 300, 300, hardbody_img);
		LoadDivGraph("data/img/enemy/Fryingman.PNG", 120, 40, 3, 300, 300, fryingman_img);
		LoadDivGraph("data/img/enemy/Jugem.PNG", 48, 24, 2, 300, 300, jugem_img);
		LoadDivGraph("data/img/enemy/Bossmiddle.PNG", 200, 50, 4, 300, 300, bossmiddle_img);
		LoadDivGraph("data/img/enemy/Wireman.PNG", 273, 39, 7, 300, 300, wireman_img);
		LoadDivGraph("data/img/enemy/CircularSaw.PNG", 5, 5, 1, 300, 300, circularsaw_img);
		LoadDivGraph("data/img/enemy/AllCoin.PNG", 117, 39, 3, 300, 300, allcoin_img);
		LoadDivGraph("data/img/enemy/BigGun.PNG", 19, 19, 1, 300, 300, cannon_img);
		//LoadDivGraph("data/img/enemy/Boss.PNG", 280, 40, 7, 600, 600, boss_img);
		LoadDivGraph("data/img/enemy/spring.PNG", 30, 30, 1, 250, 250, spring_img);
		LoadDivGraph("data/img/enemy/Chocolate.PNG", 38, 38, 1, 300, 300, chocolate_img);
		LoadDivGraph("data/img/enemy/Watch.PNG", 39, 39, 1, 300, 300, watch_img);
		LoadDivGraph("data/img/enemy/Switch.PNG", 20, 20, 1, 300, 300, switch_img);
		LoadDivGraph("data/img/enemy/Gear.PNG", 13, 13, 1, 300, 300, gear_img);
		boss_3d_down = MV1LoadModel("data/img/enemy/ボス_ダウン.mv1");
		boss_3d_beam = MV1LoadModel("data/img/enemy/ボス_ビーム.mv1");
		boss_3d_lockon = MV1LoadModel("data/img/enemy/ボス_自機狙い.mv1");
		boss_3d_barrage = MV1LoadModel("data/img/enemy/ボス_弾幕.mv1");
		boss_3d_cleave = MV1LoadModel("data/img/enemy/ボス_薙ぎ払い.mv1");
		boss_move_3d[0] = MV1LoadModel("data/img/enemy/ボス_ダウン.mv1");
		for (int i = 0; i < 4; i++) {
			boss_move_3d[i] = MV1SetRotationXYZ(boss_move_3d[i], VGet(2.827435f, -3.979354f, 0));
		}
		MV1SetScale(boss_3d_down, VGet(0.5, 0.5, 0.5));
	}
	~cCharacterManager() {
		DeleteCharacters();
		for (int i = 0; i < 273; i++) { DeleteGraph(wireman_img[i]); }
		for (int i = 0; i < 120; i++) { DeleteGraph(jumpman_img[i]); }
		for (int i = 0; i < 200; i++) { DeleteGraph(bossmiddle_img[i]); }
		for (int i = 0; i < 30; i++) { DeleteGraph(hardbody_img[i]); }
		for (int i = 0; i < 123; i++) { DeleteGraph(fryingman_img[i]); }
		for (int i = 0; i < 148; i++) { DeleteGraph(gunman_img[i]); }
		for (int i = 0; i <   5; i++) { DeleteGraph(circularsaw_img[i]); }
		for (int i = 0; i <  19; i++) { DeleteGraph(cannon_img[i]); }
		for (int i = 0; i < 117; i++) { DeleteGraph(allcoin_img[i]); }
		for (int i = 0; i < 280; i++) { DeleteGraph(boss_img[i]); }
		for (int i = 0; i < 75; i++) { DeleteGraph(jugem_img[i]); }
		for (int i = 0; i < 30;  i++) { DeleteGraph(spring_img[i]); }
		for (int i = 0; i < 38; i++) { DeleteGraph(chocolate_img[i]); }
		for (int i = 0; i < 39; i++) { DeleteGraph(watch_img[i]); }
		for (int i = 0; i < 13; i++) { DeleteGraph(gear_img[i]); }
		MV1DeleteModel(boss_3d_down);
		MV1DeleteModel(boss_3d_beam);
		MV1DeleteModel(boss_3d_lockon);
		MV1DeleteModel(boss_3d_barrage);
		MV1DeleteModel(boss_3d_cleave);
		DeleteGraph(floorimg);
	}

	cObject *GetPlayer() { return (cObject*)player; }
	cObject *GetAnchor() { return (cObject*)player->GetAnchor(); }
	cObject *GetAnchorWire(int num) { return (cObject*)player->GetAnchorWire(num); }
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
	cObject *GetEnemyJugem(int num) { return (cObject*)jugem[num]; }
	cObject *GetEnemyBoss() { return (cObject*)boss; }
	cObject *GetSpring(int num) { return (cObject*)spring[num]; }
	cObject *GetCrumbleWall(int num) { return (cObject*)crumblewall[num]; }
	cObject *GetGear(int num) { return (cObject*)gear[num]; }
	cObject *GetMoveWall(int num) { return (cObject*)movewall[num]; }
	cObject *GetWall(int num) { return (cObject*)wall[num]; }
	int GetSwitchHp(int num) { return movewall[num]->GetHp(); }

	bool	GetAddSwitch() { return player->addtimeswitch; }
	bool	GetAddSwitchChange() { return player->addtimeswitch = false; }
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
	eSpring,		// 16
	eCrumblewall,	// 17
	eGear,			// 18
	eMovewall,		// 19
};
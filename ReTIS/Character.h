#pragma once

extern VECTOR FocusPos;
extern VECTOR FocusOld;
extern VECTOR FocusCam;
extern VECTOR MouseAdd;

using namespace std;

class cCharacterBase : public cObject {
protected:
	float	speed;
	float	jump = 0.f, gravity = 1.f, inertia = 0; // d—Í‚ÆŠµ«
	int		hp = 5, invincible_time = 0;
	int		jump_count = 0;
	bool	invincible = false;
	void	Physical();	// ƒWƒƒƒ“ƒv‚Æ‚©‚ÌŒvŽZ

						// ’…’n‚µ‚½Žž‚Ì”»’è ------------------
	bool	landing;

	// ƒƒCƒ„[‚ªG‚ê‚½Žž‚Ì”»’è-----------
	bool	wirehit;

	// ‰½‚©‚É‚ ‚½‚Á‚½‚Æ‚«-----------------
	bool	ceiling;

	// ‘«ê‚Ì‰º‚É“–‚½‚Á‚Ä‚¢‚é‚©‚Ì”»’è-----
	bool	bottomhit;
	bool	righthit;
	bool	lefthit;


public:
	cCharacterBase() {};
	~cCharacterBase() {
	};
	bool	possess;	// æ‚èˆÚ‚ç‚ê‚Ä‚¢‚é‚©
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
	int move_dir;

	int image_change;

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

	cEnemyGunman(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		type = Enemy;
		landing = false;

		attack_count = 0;
		bulletsize = { 20,20,0 };
		image_change = 0;
		move_pattern = 0;
		bullet_fire = false;

		direction = true;
		player_move_pattern = 0;
	}
	~cEnemyGunman() {}

	void MoveByAutomation();
	void MoveByPlayer();
	void Update();
	void Render(int image[]);
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
		type = Enemy;
		landing = false;

		attack_count = 0;
	}
	void MoveByAutomation();
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
	int move_flow;		// 0,~‰º 1,‰ñ“] 2,”­–C 3,ã¸iÁ‚¦‚éj
	int rotation_time;	// ‚Ç‚Ì‚­‚ç‚¢‰ñ‚Á‚Ä‚¢‚é‚©
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


	int image_change;

	cEnemyBossmiddle(float x, float y, float w, float h, float s, bool p) {
		pos = { x, y, 0.f };
		size = { w, h, 0.f };
		speed = s;
		possess = p;
		landing = false;
		type = Enemy;

		move_time = 0;
		move_pattern = 0;
		move_speed = 1.5f;
		image_change = 0;
	}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
	void Render(int image[]);
};


class cCharacterManager {
protected:
	int		possess_time = 0;

public:
	cPlayer			*player;
	cEnemyJumpman	*jumpman[ENEMY_MAX];
	cEnemyHardBody	*hardbody[ENEMY_MAX];
	cEnemyWiremanManager::Wireman	*wireman[ENEMY_MAX];
	cEnemyFryingman *fryingman[ENEMY_MAX];
	cEnemyWiremanManager::Anchor*wireanchor[ENEMY_MAX];
	cEnemyWiremanManager *wmanager[ENEMY_MAX];
	cEnemyGunman *gunman[ENEMY_MAX];
	cEnemyBossmiddle *bossmiddle[ENEMY_MAX];

	int		wireman_img[273];
	int		jumpman_img[120];
	int		bossmiddle_img[200];
	int		fryingman_img[123];
	int		gunman_img[234];

	void	Update();
	void	Render();
	void	PossessListener();
	void	LoadCharacters(string name);
	void	DeleteCharacters();

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
	}
	~cCharacterManager() {
		DeleteCharacters();
	}

	cObject *GetPlayer() { return (cObject*)player; }
	cObject *GetEnemyJumpman(int num) { return (cObject*)jumpman[num]; }
	cObject *GetEnemyHardBody(int num) { return (cObject*)hardbody[num]; }
	cObject *GetEnemyWireman(int num) { return (cObject*)wireman[num]; }
	cObject *GetEnemyFryingman(int num) { return (cObject*)fryingman[num]; }
	cObject *GetEnemyWireAnchor(int num) { return (cObject*)wireanchor[num]; }
	cObject *WireAnchor(int num) { return (cObject*)wireanchor[num]; }
	cObject *GetEnemyGunman(int num) { return (cObject*)gunman[num]; }
	cObject *GetEnemyBossmiddle(int num) { return (cObject*)bossmiddle[num]; }

	int		GetPlayerHp() { return player->GetHp(); }

};

enum character {
	eCharacter,
	ePlayer,
	eJumpman,
	eHardbody,
	eFryingman,
	eGunman,
	eBossmiddle
};
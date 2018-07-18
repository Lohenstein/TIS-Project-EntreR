#pragma once

extern VECTOR FocusPos;
extern VECTOR FocusOld;
extern VECTOR FocusCam;
extern VECTOR MouseAdd;
class cCharacterBase : public cObject{
protected:
	float	speed;
	float	jump = 0.f, gravity = 1.f , inertia = 0; // èdóÕÇ∆äµê´
	int		hp = 5, invincible_time = 0;
	int		jump_count = 0;
	bool	invincible = false;
	void	Physical();	// ÉWÉÉÉìÉvÇ∆Ç©ÇÃåvéZ

	// íÖínÇµÇΩéûÇÃîªíË ------------------
	bool	landing;

	// ÉèÉCÉÑÅ[Ç™êGÇÍÇΩéûÇÃîªíË-----------
	bool	wirehit;

	// âΩÇ©Ç…Ç†ÇΩÇ¡ÇΩÇ∆Ç´-----------------
	bool	ceiling;

public:
	cCharacterBase() {};
	~cCharacterBase() {
	};
	bool	possess;	// èÊÇËà⁄ÇÁÇÍÇƒÇ¢ÇÈÇ©
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


class cEnemyWiremanManager : public cEnemy {
public:
	VECTOR WirePos;
	int AnchorStretch;
	class Wireman : public cEnemy {
	public:
		int enemy_count;
		int mouse_state;
		int mouse_posx;
		int mouse_posy;
		float move_speedx;
		float move_speedy;
		float anchor_speed;
		float wire_radian;
		float mouse_changeposx;
		float mouse_changeposy;


		Wireman(float x, float y, float w, float h, float s, bool p) {
			pos = { x, y, 0.f };
			size = { w, h, 0.f };
			speed = s;
			possess = p;
			type = Enemy;
			landing = false;

			mouse_state = 0;
			mouse_posx = 0;
			mouse_posy = 0;
			move_speedx = 0.f;
			move_speedy = 0.f;
			anchor_speed = 10.f;
			wire_radian = 0.f;
			mouse_changeposx = 0.f;
			mouse_changeposy = 0.f;
		}
		void Update(VECTOR *WirePos,int *AnchorStretch);
		void MoveByPlayer(VECTOR *WirePos, int *AnchorStretch);
		void MoveByAutomation(VECTOR *WirePos, int *AnchorStretch);
		void WireRender(VECTOR *WirePos, int *AnchorStretch);
		void MouseStateGet();
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
		void Update(VECTOR *WirePos, int *AnchorStretch);
		void MoveByPlayer(VECTOR *WirePos, int *AnchorStretch);
		void MoveByAutomation(VECTOR *WirePos, int *AnchorStretch);
		void WireRender(VECTOR *WirePos, int *AnchorStretch);

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
	int move_flow;		// 0,ç~â∫ 1,âÒì] 2,î≠ñC 3,è„è∏Åiè¡Ç¶ÇÈÅj
	int rotation_time;	// Ç«ÇÃÇ≠ÇÁÇ¢âÒÇ¡ÇƒÇ¢ÇÈÇ©
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
	cEnemyWiremanManager::Wireman	*wireman[ENEMY_MAX];
	cEnemyFryingman *fryingman[ENEMY_MAX];
	cEnemyWiremanManager::Anchor*wireanchor[ENEMY_MAX];
	cEnemyWiremanManager *wmanager[ENEMY_MAX];

	cCharacterManager() {
		player		 = new cPlayer(400.f, 100.f, 90.f, 120.f, 6.f, true);
		jumpman[0]	 = new cEnemyJumpman(300.f, 100.f, 90.f,120.f, 2.f, false);
		hardbody[0]	 = new cEnemyHardBody(1000.f, 100.f, 90.f, 120.f, 2.f, false);
		wireman[0]	 = new cEnemyWiremanManager::Wireman(300.f, 100.f, 90.f, 100.f, 2.f, false);
		fryingman[0] = new cEnemyFryingman(500.f, -100.f, 90.f, 90.f, 2.f, false);
		wireanchor[0] = new cEnemyWiremanManager::Anchor(100, -100, 10, 10, 2, false);
		wmanager[0] = new cEnemyWiremanManager;
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
#pragma once

extern VECTOR FocusPos;
extern VECTOR FocusOld;

class cCharacterBase : public cObject{
protected:
	float	speed;
	float	jump = 0.f, gravity = 1.f , inertia = 0; // �d�͂Ɗ���
	bool	possess;	// ���ڂ��Ă��邩
	int		hp = 5, invincible_time = 0;
	int		jump_count = 0;
	bool	invincible = false;
	void	Physical();	// �W�����v�Ƃ��̌v�Z

	// ���n�������̔��� ------------------
	bool	landing;

	// ���C���[���G�ꂽ���̔���-----------
	bool	wirehit;

	// �����ɂ��������Ƃ�-----------------
	bool	ceiling;

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

	void MoveByPlayer();
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
	void MoveByPlayer();
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
	float move_speed;		// ���C���[��L�΂��Ă���Ƃ��̃L�����̃X�s�[�h
	float wire_length;		// ���C���[�̒���
	float wire_gravity;		// ���C���[��L�΂��Ă���Ƃ��̏d��
	float rot;				// �����v���C���[�p�x
	float filing_angle;		// ���C���[���ˊp�x�i45�x�j
	bool start_wire;		// ���C���[�œ����Ă��邩
	int action_count;
	int dir;				// 1,�E -1,��


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
		wire_gravity = 0.5f;
		wire_length = 100;
		move_speed = 6;
		start_wire = false;
		action_count = 100;
		dir = -1;
	}
	void Update();
	void MoveByPlayer();
	void MoveByAutomation();
	void WireRender();
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
	int move_flow;		// 0,�~�� 1,��] 2,���C 3,�㏸�i������j
	int rotation_time;	// �ǂ̂��炢����Ă��邩
	bool firing;		
	int time_sub;



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
public:
	cPlayer			*player;
	cEnemyJumpman	*jumpman[ENEMY_MAX];
	cEnemyHardBody	*hardbody[ENEMY_MAX];
	cEnemyWireman	*wireman[ENEMY_MAX];
	cEnemyFryingman *fryingman[ENEMY_MAX];

	cCharacterManager() {
		player		 = new cPlayer(400.f, 100.f, 90.f, 120.f, 6.f, true);
		jumpman[0]	 = new cEnemyJumpman(300.f, 100.f, 90.f,120.f, 2.f, false);
		hardbody[0]	 = new cEnemyHardBody(1000.f, 100.f, 90.f, 120.f, 2.f, false);
		wireman[0]	 = new cEnemyWireman(200.f, 100.f, 100.f, 100.f, 2.f, false);
		fryingman[0] = new cEnemyFryingman(500.f, -100.f, 90.f, 90.f, 2.f, false);
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
	cObject *GetPlayer()                { return (cObject*)player; }
	cObject *GetEnemyJumpman(int num)   { return (cObject*)jumpman[num];   }
	cObject *GetEnemyHardBody(int num)  { return (cObject*)hardbody[num];  }
	cObject *GetEnemyWireman(int num)   { return (cObject*)wireman[num];   }
	cObject *GetEnemyFryingman(int num) { return (cObject*)fryingman[num]; }

	int		GetPlayerHp() { return player->GetHp(); }

};
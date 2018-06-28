#pragma once

class cCharacterBase : public cObject{
protected:
	VECTOR	old;	// �����p�ߋ����W
	float	speed;
	float	jump = 0.f, gravity = 1.f , inertia = 0; // �d�͂Ɗ���
	bool	possess;	// ���ڂ��Ă��邩
	void	Physical();	// �W�����v�Ƃ��̌v�Z
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
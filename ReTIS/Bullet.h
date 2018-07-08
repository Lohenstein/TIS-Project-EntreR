#pragma once

class cBullet : public cObject {
protected:
	float speed;
	float rad;
	bool  flag;
public:
	cBullet(VECTOR p, VECTOR s, float sp, float r, eObjectType t){
		pos   = p;
		size  = s;
		speed = sp;
		rad   = r;
		flag  = true;
		type = t;
	}
	~cBullet() {}; 
	void	Update();
	void	Render();
	void	HitAction(cObject *hit);
	bool	GetFlag() { return flag; }
};

class cBulletManager {
protected:
	cBullet *bullet[BULLET_MAX];
public:
	cBulletManager()  { 
		for (int i = 0; i < BULLET_MAX; i++) bullet[i] = nullptr;
	}
	~cBulletManager() { 
		for (int i = 0; i < BULLET_MAX; i++) {
			delete bullet[i];
			bullet[i] = nullptr;
		}
	}
	void	Update();
	void	Render();
	void	Shot(VECTOR p, VECTOR s, float sp, float r, eObjectType t);
	cObject *GetBullet(int num) { return (cObject*)bullet[num]; }
};

extern cBulletManager bullet;
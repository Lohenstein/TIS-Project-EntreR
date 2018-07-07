#pragma once

#define BULLET_MAX 100

class cBullet : public cObject {
protected:
	float speed;
	float rad;
	bool  flag;
public:
	cBullet(VECTOR p, VECTOR s, float sp, float r){
		pos   = p;
		size  = s;
		speed = sp;
		rad   = r;
		flag  = true;
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
	void	Shot(VECTOR p, VECTOR s, float sp, float r);
};

extern cBulletManager bullet;
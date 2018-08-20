#pragma once

class cBullet : public cObject {
protected:
	float speed;
	float rad;
	bool  flag;
public:
	bool direction;
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

class cAnchor : public cObject {
protected:
	float speed;
	float rad;
	float distance;
	bool  flag;
public:
	cAnchor(VECTOR p, VECTOR s, float sp, float r, eObjectType t) {
		pos = p;
		size = s;
		speed = sp;
		distance = 0.f;
		rad = r;
		flag = true;
		type = t;
	}
	~cAnchor() {};
	void	Update();
	void	Render(VECTOR cpos);
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
	void	CurvedShot(VECTOR p, VECTOR s, float sp, float r, eObjectType t,bool direction);
	cObject *GetBullet(int num) { return (cObject*)bullet[num]; }
};

extern cBulletManager bullet;
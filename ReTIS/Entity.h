#pragma once

class cEntity : public cObject {
protected:
public:
	cEntity() {};
	~cEntity() {};
};

class cMoveFloor : public cEntity {
protected:
	int		now_point;
	bool	flag;
	float	p;
	float	sx[10], sy[10];
public:
	// コンストラクタ
	cMoveFloor() {
		flag = true;
		size = { 160.f, 30.f, 0.f };
		p	 = 0.f;
		now_point = 0;
		type = MoveFloor;
	}
	cMoveFloor(float h, float w) {
		flag = true;
		size = { w, h, 0.f };
		p    = 0.f;
		now_point = 0;
		type = MoveFloor;
	}
	void	Render();
	void	Update(float s, int p1, int p2);
	void	SetPoint(int num, float x, float y);
	void	HitAction(cObject *hit) {};
};

class cDropFloor : public cEntity {
protected:
	int		time;
	bool	flag;
	float	sx, sy;
	float	gravity = 1.f;
	float	drop_p  = 0.f;
public:
	cDropFloor() {
		flag = false;
		size = { 160.f, 30.f, 0.f };
		time = 0;
	}
	void	Render();
	void	Update();
	void	SetPoint(float x, float y);
	void	HitAction(cObject *hit);
};
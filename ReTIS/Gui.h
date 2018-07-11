#pragma once

class cGuiGame {
protected:
	int hp;
	int imgh_hp = LoadGraph("data/img/hp/hpheart.png");
public:
	void	SetHp(int hitpoint) { hp = hitpoint; }
	void	Update();
	void	Render();
};

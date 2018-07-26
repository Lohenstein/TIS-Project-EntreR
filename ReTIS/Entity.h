#pragma once

using namespace std;

/*
class cEntityManager {
public:

	cDropFloor *dropfloor[ENEMY_MAX];
	cMoveFloor *movefloor[ENEMY_MAX];

	void	LoadEntities(string name);

	void	Render();
	void	Update();

	cEntityManager(string name) {
		LoadEntities(name);
	}
	~cEntityManager() {
		for (int i = 0; i < ENEMY_MAX; i++) {
			delete dropfloor[i];
			delete movefloor[i];
			dropfloor[i] = nullptr;
			movefloor[i] = nullptr;
		}
	}

	cObject *GetDropFloor(int num) { return (cObject*)dropfloor[num]; }
	cObject *GetMoveFloor(int num) { return (cObject*)movefloor[num]; }
};
*/

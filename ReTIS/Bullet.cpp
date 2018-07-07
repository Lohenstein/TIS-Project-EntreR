
#include "Main.h"

cBulletManager bullet;

void cBullet::Update() {

	pos.x = sin(rad) * speed;
	pos.y = cos(rad) * speed;

	if (GetPos().x > WINDOW_SIZE_X || GetPos().x < 0 ||
		GetPos().y > WINDOW_SIZE_Y || GetPos().y < 0) {
		//‰æ–ÊŠO‚È‚çÁ‚·
		flag = false;
	}
}

void cBullet::Render() {
	DrawCircleAA(GetPos().x, GetPos().y, 10.f, 10, 0xFFFFFF, true);
}

void cBullet::HitAction(cObject *hit) {
	flag = false;
}

void cBulletManager::Update() {

	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i] != nullptr) {
			bullet[i]->Update();
			if (bullet[i]->GetFlag() == false) {
				delete bullet[i];
				bullet[i] = nullptr;
			}
		}
	}
}

void cBulletManager::Render() {
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i] != nullptr) {
			bullet[i]->Render();
		}
	}
}

void cBulletManager::Shot(VECTOR p, VECTOR s, float sp, float r) {
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i] == nullptr) {
			bullet[i] = new cBullet(p, s, sp, r);
			break;
		}
	}
}

#include "Main.h"

cBulletManager bullet;

void cBullet::Update() {

	pos.x += sin(rad) * speed;
	pos.y += cos(rad) * speed;

	if (GetPos().x > FocusPos.x + WINDOW_SIZE_X || GetPos().x < FocusPos.x - WINDOW_SIZE_X ||
		GetPos().y > FocusPos.y + WINDOW_SIZE_Y || GetPos().y < FocusPos.y - WINDOW_SIZE_Y) {
		//‰æ–ÊŠO‚È‚çÁ‚·
		flag = false;
	}
}

void cBullet::Render() {
	DrawCircleAA(GetPos().x, GetPos().y, 10.f, 10, 0xFFFFFF, true);
}

void cBullet::HitAction(cObject *hit) {
	//flag = false;
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

void cBulletManager::Shot(VECTOR p, VECTOR s, float sp, float r, eObjectType t) {
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i] == nullptr) {
			bullet[i] = new cBullet(p, s, sp, r, t);
			break;
		}
	}
}

#include "Main.h"

cBulletManager bullet;

int		bullet_img[48];

void	LoadBulletImg() {
	LoadDivGraph("data/img/bullet/16x16.png", 48, 6, 8, 16, 16, bullet_img);
}

void	cAnchor::HitAction(cObject *hit) {
	if (hit->GetType() == MapTile) {
		flag = false;
	}
}

void	cAnchor::Update() {
	pos.x += cos(rad) * speed;
	pos.y += sin(rad) * speed;
}

void	cAnchor::Render(VECTOR cpos) {
	DrawCircleAA(GetPos().x, GetPos().y, 10.f, 10, 0xFF0000, true);
	DrawLineAA(GetPos().x, GetPos().y, cpos.x, cpos.y, 0xFF00000);
}

void	cAnchorWire::HitAction(cObject *hit) {
	if (hit->GetType() == MapTile) {
		flag = false;
	}
}

void	cAnchorWire::Update(VECTOR a) {
	pos = a;
}

void	cAnchorWire::Render() {
	DrawCircleAA(GetPos().x, GetPos().y, 4.f, 10, 0xFF0000, true);
}

void	cBullet::Update() {
	if (type == PlayerBullet || type == EnemyBullet)
	pos.x += cos(rad) * speed;
	pos.y += sin(rad) * speed;

	if (speed == 0)
		pos.y -= 10000;

	if (GetPos().x > FocusPos.x + WINDOW_SIZE_X || GetPos().x < FocusPos.x - WINDOW_SIZE_X ||
		GetPos().y > FocusPos.y + WINDOW_SIZE_Y || GetPos().y < FocusPos.y - WINDOW_SIZE_Y) {
		//‰æ–ÊŠO‚È‚çÁ‚·
		flag = false;
	}
	else if (type == JugemBullet) {
		pos.x += cos(rad) * speed;
		pos.y += sin((direction ? rad -= 0.05 : rad += 0.05)) * speed;

		if (speed == 0)
			pos.y -= 10000;

		if (GetPos().x > FocusPos.x + WINDOW_SIZE_X || GetPos().x < FocusPos.x - WINDOW_SIZE_X ||
			GetPos().y > FocusPos.y + WINDOW_SIZE_Y || GetPos().y < FocusPos.y - WINDOW_SIZE_Y) {
			//‰æ–ÊŠO‚È‚çÁ‚·
			flag = false;
		}
	}
	anim++;
		if (anim >= 3) anim = 0;
}

void	cBullet::Render() {
	switch (type) {
	case PlayerBullet: 
		DrawGraph(pos.x, pos.y, bullet_img[32 + (anim * 6)], true);
		break;
	default:
		DrawGraph(pos.x, pos.y, bullet_img[34 + (anim * 6)], true);
		break;
	}
}

void	cBullet::HitAction(cObject *hit) {
	switch (hit->GetType()) {
	case Enemy:
		switch (this->GetType()) {
		case PlayerBullet:
			flag = false;
			break;
		case EnemyBullet:
			break;
		case JugemBullet:
			break;
		}
		break;
	case MapTile:
		flag = false;
		break;
	case Player:
		switch (this->GetType()) {
		case PlayerBullet:
			break;
		case EnemyBullet:
			flag = false;
			break;
		case JugemBullet:
			flag = false;
			break;
		}
		break;
	}
}

void	 cBulletManager::Update() {

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

void cBulletManager::CurvedShot(VECTOR p, VECTOR s, float sp, float r, eObjectType t, bool direction) {
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i] == nullptr) {
			bullet[i] = new cBullet(p, s, sp, r, t);
			bullet[i]->direction = direction;
			break;
		}
	}
}
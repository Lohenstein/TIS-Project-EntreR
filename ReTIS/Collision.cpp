#include "Main.h"

// ��`���m�̂����蔻��
void	CheckHitRectAndRect(cObject *obj1, cObject *obj2) {

	if (obj1->GetPos().x - (obj1->GetSize().x / 2.f) <= obj2->GetPos().x + (obj2->GetSize().x / 2.f) &&
		obj2->GetPos().x - (obj2->GetSize().x / 2.f) <= obj1->GetPos().x + (obj1->GetSize().x / 2.f) &&
		obj1->GetPos().y - (obj1->GetSize().y / 2.f) <= obj2->GetPos().y + (obj2->GetSize().y / 2.f) &&
		obj2->GetPos().y - (obj2->GetSize().y / 2.f) <= obj1->GetPos().y + (obj1->GetSize().y / 2.f)) {

		// �Փˑ���̏���n���ďՓˌ�̏������s��
		obj1->HitAction(obj2);
		obj2->HitAction(obj1);
	}
}

bool	CheckHitMapTile(cObject *obj1, int x, int y) {
	if (obj1->GetPos().x - (obj1->GetSize().x / 2.f) <= x * bsize + bsize &&
		x * bsize <= obj1->GetPos().x + (obj1->GetSize().x / 2.f) &&
		obj1->GetPos().y - (obj1->GetSize().y / 2.f) <= y * bsize + bsize &&
		y * bsize <= obj1->GetPos().y + (obj1->GetSize().y / 2.f)) {

		return true;
	}
	return false;
}

bool	CheckCollisionAroundMaptile(cObject *hit) {

	VECTOR pos = hit->GetPos();

	int csize = 20;
	int x = (int)pos.x / (int)bsize, y = (int)pos.y / (int)bsize;
	int startx = x - csize / 2, starty = y - csize / 2;
	int endx = x + csize / 2, endy = y + csize / 2;

	if (startx < 0) startx = 0;
	if (endx > stage_size_x) endx = stage_size_x;
	if (starty < 0) starty = 0;
	if (endy > stage_size_y) endy = stage_size_y;

	for (int i = startx; i < endx; i++) {
		for (int j = starty; j < endy; j++) {
			if (stage_collision[i][j]) {
				if (CheckHitMapTile(hit, i, j)) {
					return true;
				}
			}
		}
	}
	return false;
}

void	CheckHitRectAndCircle(cObject *obj1, cObject *obj2) {
	if ((obj2->GetPos().x > obj1->GetPos().x - (obj1->GetSize().x / 2.f) && obj2->GetPos().x < obj1->GetPos().x + (obj1->GetSize().x / 2.f) && obj2->GetPos().y > obj1->GetPos().y - (obj1->GetSize().y / 2.f) - obj2->GetSize().z && obj2->GetPos().y < obj1->GetPos().y + (obj1->GetSize().y / 2.f) + obj2->GetSize().z) ||
		(obj2->GetPos().x > obj1->GetPos().x - (obj1->GetSize().x / 2.f) - obj2->GetSize().z && obj2->GetPos().x < obj1->GetPos().x + (obj1->GetSize().x / 2.f) + obj2->GetSize().z && obj2->GetPos().y > obj1->GetPos().y - (obj1->GetSize().y / 2.f) && obj2->GetPos().y < obj1->GetPos().y + (obj1->GetSize().y / 2.f)) ||
		((obj1->GetPos().x - (obj1->GetSize().x / 2.f) - obj2->GetPos().x) * (obj1->GetPos().x - (obj1->GetSize().x / 2.f) - obj2->GetPos().x) + (obj1->GetPos().y - (obj1->GetSize().y / 2.f) - obj2->GetPos().y) * (obj1->GetPos().y - (obj1->GetSize().y / 2.f) - obj2->GetPos().y) < obj2->GetSize().z * obj2->GetSize().z) ||
		((obj1->GetPos().x + (obj1->GetSize().x / 2.f) - obj2->GetPos().x) * (obj1->GetPos().x - (obj1->GetSize().x / 2.f) - obj2->GetPos().x) + (obj1->GetPos().y - (obj1->GetSize().y / 2.f) - obj2->GetPos().y) * (obj1->GetPos().y - (obj1->GetSize().y / 2.f) - obj2->GetPos().y) < obj2->GetSize().z * obj2->GetSize().z) ||
		((obj1->GetPos().x - (obj1->GetSize().x / 2.f) - obj2->GetPos().x) * (obj1->GetPos().x - (obj1->GetSize().x / 2.f) - obj2->GetPos().x) + (obj1->GetPos().y - (obj1->GetSize().y / 2.f) - obj2->GetPos().y) * (obj1->GetPos().y + (obj1->GetSize().y / 2.f) - obj2->GetPos().y) < obj2->GetSize().z * obj2->GetSize().z) ||
		((obj1->GetPos().x + (obj1->GetSize().x / 2.f) - obj2->GetPos().x) * (obj1->GetPos().x - (obj1->GetSize().x / 2.f) - obj2->GetPos().x) + (obj1->GetPos().y - (obj1->GetSize().y / 2.f) - obj2->GetPos().y) * (obj1->GetPos().y + (obj1->GetSize().y / 2.f) - obj2->GetPos().y) < obj2->GetSize().z * obj2->GetSize().z)) {

		// �Փˑ���̏���n���ďՓˌ�̏������s��
		obj1->HitAction(obj2); // �~
		obj2->HitAction(obj1); // �l�p
	}
}

void	CheckHitRectAndRollingRect(cObject *obj1, cObject *obj2){
	if (obj1->GetPos().x - (obj1->GetSize().x / 2.f) <= (obj2->GetPos().x + (obj2->GetRadrighttop() * (obj2->GetSize().x / 2.f))) &&
		(obj2->GetPos().x - (obj2->GetRadlefttop() * (obj2->GetSize().x / 2.f)) <= obj1->GetPos().x + (obj1->GetSize().x / 2.f)) &&
		(obj1->GetPos().y - (obj1->GetSize().y / 2.f) <= (obj2->GetPos().y + (obj2->GetRadrightbottom() * (obj2->GetSize().y / 2.f))) &&
		(obj2->GetPos().y - (obj2->GetRadleftbottom() * (obj2->GetSize().y / 2.f)) <= obj1->GetPos().y + (obj1->GetSize().y / 2.f)))) {
		// �Փˑ���̏���n���ďՓˌ�̏������s��
		obj1->HitAction(obj2);	// �l�p
		obj2->HitAction(obj1);	// �l�p�irollingrect�j
	}
	else if (obj1->GetPos().x - (obj1->GetSize().x / 2.f) <= (obj2->GetPos().x + (obj2->GetRadlefttop() * (obj2->GetSize().x / 2.f))) &&
		(obj2->GetPos().x - (obj2->GetRadrightbottom() * (obj2->GetSize().x / 2.f)) <= obj1->GetPos().x + (obj1->GetSize().x / 2.f)) &&
		(obj1->GetPos().y - (obj1->GetSize().y / 2.f) <= (obj2->GetPos().y + (obj2->GetRadleftbottom() * (obj2->GetSize().y / 2.f))) &&
		(obj2->GetPos().y - (obj2->GetRadrighttop() * (obj2->GetSize().y / 2.f)) <= obj1->GetPos().y + (obj1->GetSize().y / 2.f)))) {
		// �Փˑ���̏���n���ďՓˌ�̏������s��
		obj1->HitAction(obj2);	// �l�p
		obj2->HitAction(obj1);	// �l�p�irollingrect�j
	}
}

// ��ɍڂ��Ă�����ʐς𔻕ʂ���
float	col_CheckRadian(VECTOR p1, VECTOR p2) {
	float x = (p1.x / 2.f) + (p2.x / 2.f);
	float y = (p1.y / 2.f) + (p2.y / 2.f);
	return atan2f(y, x);
}

// �����蔻�菈����ǂ̕����ɖ߂��̂����p�x���狁�߂�
// 1 : right, 2 : bottom, 3 : left, 4 : top
int		col_HitRadian(VECTOR p1, VECTOR p2, float rad) {

	// �L�����N�^���S�ƃ}�b�v�`�b�v���S�̊p�x
	int deg  = (int)r2d(atan2f(p2.y - p1.y, p2.x - p1.x) + PI);
	int cdeg = (int)r2d(rad);

	if (deg >= cdeg && deg < 180 - cdeg) {
		return 2;
	}
	else if (deg >= 180 - cdeg && deg < 180 + cdeg) {
		return 3;
	}
	else if (deg >= 180 + cdeg && deg < 360 - cdeg) {
		return 4;
	}
	else {
		return 1;
	}
	return -1;
}

int		col_HitRadianR(VECTOR p1, VECTOR p2) {

	// �L�����N�^���S�ƃ}�b�v�`�b�v���S�̊p�x
	int deg = (int)r2d(atan2f(p2.y - p1.y, p2.x - p1.x)) + 180;

	if (deg > 45 && deg <= 135) {
		return 2;
	}
	else if (deg > 135 && deg <= 225) {
		return 3;
	}
	else if (deg > 225 && deg <= 315) {
		return 4;
	}
	else {
		return 1;
	}
	return -1;
}
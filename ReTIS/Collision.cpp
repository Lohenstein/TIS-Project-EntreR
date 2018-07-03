#include "Main.h"

// ��`���m�̂����蔻��
void CheckHitRectAndRect(cObject *obj1, cObject *obj2) {

	if (obj1->GetPos().x - (obj1->GetSize().x / 2.f) <= obj2->GetPos().x + (obj2->GetSize().x / 2.f) &&
		obj2->GetPos().x - (obj2->GetSize().x / 2.f) <= obj1->GetPos().x + (obj1->GetSize().x / 2.f) &&
		obj1->GetPos().y - (obj1->GetSize().y / 2.f) <= obj2->GetPos().y + (obj2->GetSize().y / 2.f) &&
		obj2->GetPos().y - (obj2->GetSize().y / 2.f) <= obj1->GetPos().y + (obj1->GetSize().y / 2.f)) {

		// �Փˑ���̏���n���ďՓˌ�̏������s��
		obj1->HitAction(obj2);
		obj2->HitAction(obj1);
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
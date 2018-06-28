
#include "Main.h"
/*------------------------------------------------------------------------------*
| <<< cCharacterBase >>>
*------------------------------------------------------------------------------*/
void	cCharacterBase::MoveByAutomation() {
	old = pos;
}
void	cCharacterBase::MoveByPlayer() {
	old = pos;	// �ߋ����W

	if (key[KEY_INPUT_A] == 2 || key[KEY_INPUT_D] == 2) {
		if (key[KEY_INPUT_A] == 2) {
			inertia -= 4;				// �ړ��ʃƂ�����
		}
		if (key[KEY_INPUT_D] == 2) {
			inertia += 4;				// �ړ��ʃƂ𑝉�
		}
	}
	else {
		// �L�[�����������ȊO�͎�������
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
	if (key[KEY_INPUT_SPACE] == 1) jump = 20.f;
}

void	cCharacterBase::Physical() {

	if (inertia >  90) inertia = 90;	// �͂ݏo�����~�b�^
	if (inertia < -90) inertia = -90;

	pos.x += sin((float)d2r(inertia)) * speed;

	pos.y -= jump;
	jump -= gravity;
}

void	cCharacterBase::Update() {
	if (possess) {
		MoveByPlayer();		// ���ڂ��Ă�����蓮����
	}
	else {
		MoveByAutomation();	// ���̑��͎���
	}
	Physical();
}

void	cCharacterBase::Render() {
	DrawBoxAA(pos.x - GetSize().x / 2.f, pos.y - GetSize().y / 2.f,
		pos.x + GetSize().x / 2.f, pos.y + GetSize().y / 2.f,
		0xFFFFFF, false);
}

void	cCharacterBase::HitAction(cObject *hit) {

	switch (hit->GetType()) {
	// ----�}�b�v�^�C���Ƃ̓����蔻��-----------------------------------------------------------
	case MapTile:
		// �v���C���[���`�b�v����݂Ăǂ̕����ɂ���̂����ߋ��̍��W�̍������狁�߂�
		float rad = col_CheckRadian(hit->GetSize(), GetSize());
		int	  dir = col_HitRadian(GetPos(), hit->GetPos(), rad);
		//int   o_dir = col_HitRadian(GetOldPos(), hit->GetPos(), rad);

		// �������ɏ���
		// *����������s�������
		switch (dir) {
		case 1: // right
			pos.x = hit->GetPos().x + hit->GetSize().x / 2.f + GetSize().x / 2.f + 1.f;
			inertia = 0;
			break;
		case 2: // bottom
			pos.y = hit->GetPos().y + hit->GetSize().y / 2.f + GetSize().y / 2.f + 1.f;
			jump = 0.f;
			break;
		case 3: // left
			pos.x = hit->GetPos().x - hit->GetSize().x / 2.f - GetSize().x / 2.f - 1.f;
			inertia = 0;
			break;
		case 4: // top
			pos.y = hit->GetPos().y - hit->GetSize().y / 2.f - GetSize().y / 2.f - 1.f;
			jump = 0.f;
			break;
		default:
			break;
		}
		break;
	}
}
/*------------------------------------------------------------------------------*
| <<< cPlayer >>>
*------------------------------------------------------------------------------*/
void	cPlayer::Render() {
	DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
		GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
		0xFF0000, true);
}
/*------------------------------------------------------------------------------*
| <<< cEnemy >>>
*------------------------------------------------------------------------------*/
void	cEnemy::Render() {
	DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
		GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
		0xFFFF00, true);
}
/*------------------------------------------------------------------------------*
| <<< cCharacterManager >>>
*------------------------------------------------------------------------------*/
void	cCharacterManager::Render() {
	player->Render();
}
void	cCharacterManager::Update() {
	player->Update();
}
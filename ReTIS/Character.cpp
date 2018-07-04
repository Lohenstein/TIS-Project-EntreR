
#include "Main.h"
VECTOR FocusPos, FocusOld;
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
		FocusOld = FocusPos;
		FocusPos = pos;
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

			// ���n����---------------------
			landing = true;
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
/*---------------------------------------
---------------------------------------*
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
	jumpman->Render();
	//nman->Render();
	hardbody->Render();
	wireman->Render();
	fryingman->Render();
}
void	cCharacterManager::Update() {
	player->Update();
	jumpman->Update();
	gunman->Update();
	hardbody->Update();
	wireman->Update();
	fryingman->Update();
}

void cEnemyJumpman::Update()
{
		if (move_dir > 0) {
			pos.x+=5;
		}
		else {
			pos.x-=5;
		}
		if (landing == true && jump_count != 3) {
			jump = 20.f;
			jump_count++;
			landing = false;
		}
		if (landing == true && jump_count == 3) {
			jump = 40.f;
			jump_count = 0;
			move_dir *= -1;
		}
	Physical();

	// �d�͌v�Z�̏������̗�
}

void cEnemyJumpman::MoveByAutomation()
{

}


// �K���}���̏���

void cEnemyGunman::MoveByAutomation()
{
	pos.x++;
}

VECTOR	GetObjectPos(cCharacterBase *character) {
	return character->GetPos();
}


void cEnemyHardBody::Update()
{
	if (landing == false) {
		stop_time += 2;
	}
	else if (landing == true && stop_time != 0) {
		stop_time--;
	}
	else {
		pos.x += move_speed;
	}
	Physical();
}

void cEnemyHardBody::MoveByAutomation()
{

}


void cEnemyWireman::Update()
{

}

void cEnemyWireman::MoveByAutomation() 
{

}


void cEnemyFryingman::Update()
{
	// �v���C���[���t�@���l���̃Z���T�[�Ɉ������������Ƃ�
	if (move_flow == -1) {
		if (FocusPos.x >= pos.x) {
			move_flow = 0;
		}
	}
	// �~���J�n
	else if (move_flow == 0) {
		pos.x = FocusPos.x + length;
		pos.y+=5;
		if (pos.y >= FocusPos.y) {
			move_flow = 1;
		}
	}
	//��]�J�n
	else if (move_flow == 1){
		rotation_time--;
		angle+=10;
		pos.x = FocusPos.x + cos(angle * PI / 180) * length;
		pos.y = FocusPos.y + sin(angle * PI / 180) * length;
		if (rotation_time <= 0) {
			move_flow = 2;
			rotation_time = 100;	// �N�[���^�C���Ɏg�p
		}
	}
	// �e����
	else if (move_flow == 2) {
		// �e���˂̏���
		rotation_time--;
		if (rotation_time <= 0)
			move_flow = 3;
	}
	// �t�@���l���P��
	else if (move_flow == 3) {
		pos.y -= 5;
	}

}
void cEnemyFryingman::MoveByAutomation()
{

}
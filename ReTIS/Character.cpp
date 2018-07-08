
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

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// �ړ��ʃƂ�����
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// �ړ��ʃƂ𑝉�
		}
	}
	else {
		// �L�[�����������ȊO�͎�������
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
	if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
		jump = 20.f;
		++jump_count;
	}
	if (key[KEY_INPUT_X] == 1) {
		bullet.Shot(pos, { 3.f, 3.f, 0.f }, 20.f, PI/2, PlayerBullet);
	}
}

void	cCharacterBase::Physical() {

	if (inertia >  90) inertia = 90;	// �͂ݏo�����~�b�^
	if (inertia < -90) inertia = -90;

	pos.x += sin((float)d2r(inertia)) * speed;

	pos.y -= jump;
	jump -= gravity;
}

void	cCharacterBase::Update() {
	// ���ڂ�
	if (possess) {
		MoveByPlayer();		// ���ڂ��Ă�����蓮����
		FocusOld = FocusPos;
		FocusPos = pos;
	}
	else {
		MoveByAutomation();	// ���̑��͎���
	}
	// ���G����
	if (invincible) {
		const int invicible_time_max = 200;
		++invincible_time;
		if (invincible_time >= invicible_time_max)
			invincible = false;
	}
	// �d��
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
	case MoveFloor:
		Collision(hit);
		if (landing) {
			pos.x += hit->GetPos().x - hit->GetOld().x;
			jump = -4.f;
		}
		break;
	case Enemy:
		Damaged();
		break;
	case EnemyBullet:
		if (possess) {
			Damaged();
		}
		break;
	case PlayerBullet:
		possess = true;
		break;
	case MapTile:
		Collision(hit);
		break;
	}
}
void	cCharacterBase::Damaged() {
	if (invincible == false) {
		invincible = true;
		invincible_time = 0;
		hp--;
		if (hp <= 0) {
			// GameOver
		}
	}
}
void	cCharacterBase::Collision(cObject *hit) {
	// �v���C���[���`�b�v����݂Ăǂ̕����ɂ���̂����ߋ��̍��W�̍������狁�߂�
	float rad = col_CheckRadian(hit->GetSize(), GetSize());
	int	  dir = col_HitRadian(GetPos(), hit->GetPos(), rad);
	//int   o_dir = col_HitRadian(GetOldPos(), hit->GetPos(), rad);
	// �������ɏ���
	landing = false;
	ceiling = false;
	// *����������s�������
	switch (dir) {
	case 1: // right
		pos.x = hit->GetPos().x + hit->GetSize().x / 2.f + GetSize().x / 2.f + 1.f;
		inertia = 0;

		// �Փ˔���---------------------
		ceiling = true;
		break;
	case 2: // bottom
		pos.y = hit->GetPos().y + hit->GetSize().y / 2.f + GetSize().y / 2.f + 1.f;
		jump = 0.f;

		// �Փ˔���---------------------
		ceiling = true;
		break;
	case 3: // left
		pos.x = hit->GetPos().x - hit->GetSize().x / 2.f - GetSize().x / 2.f - 1.f;
		inertia = 0;

		// �Փ˔���---------------------
		ceiling = true;
		break;
	case 4: // top
		pos.y = hit->GetPos().y - hit->GetSize().y / 2.f - GetSize().y / 2.f - 1.f;
		jump = 0.f;

		// ���n����---------------------
		landing = true;
		jump_count = 0;
		break;
	default:
		break;
	}
}
/*------------------------------------------------------------------------------*
| <<< cPlayer >>>
*------------------------------------------------------------------------------*/
void	cPlayer::Render() {
	if (invincible) {
		if (invincible_time % 3 == 0) {
			DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
				GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
				0xFF0000, true);
		}
	}
	else {
		DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
			GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
			0xFF0000, true);
	}
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
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]	 != nullptr) jumpman[i]   ->Render();
		if (hardbody[i]  != nullptr) hardbody[i]  ->Render();
		if (wireman[i]	 != nullptr) wireman[i]   ->Render();
		if (fryingman[i] != nullptr) fryingman[i] ->Render();
		if (wireman[i]   != nullptr) wireman[i]   ->WireRender();
	}
}
void	cCharacterManager::Update() {
	player->Update();
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]   != nullptr) jumpman[i]   ->Update();
		if (hardbody[i]  != nullptr) hardbody[i]  ->Update();
		if (wireman[i]   != nullptr) wireman[i]   ->Update();
		if (fryingman[i] != nullptr) fryingman[i] ->Update();
		if (wireman[i]   != nullptr) wireman[i]	  ->Update();
	}
}

/*------------------------------------------------------------------------------*
| <<< cEnemyJumpman >>>
*------------------------------------------------------------------------------*/
void cEnemyJumpman::Update()
{
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

void cEnemyJumpman::MoveByAutomation()
{
	if (move_dir > 0) {
		pos.x += 5;
	}
	else {
		pos.x -= 5;
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
}


/*------------------------------------------------------------------------------*
| <<< cEnemyGunman >>>
*------------------------------------------------------------------------------*/
void cEnemyGunman::MoveByAutomation()
{
	
}

/*------------------------------------------------------------------------------*
| <<< cEnemyHardbody >>>
*------------------------------------------------------------------------------*/
void cEnemyHardBody::Update()
{
	if (possess) {
		MoveByPlayer();
		FocusOld = FocusPos;
		FocusPos = pos;
	}
	else {
		MoveByAutomation();
	}

	Physical();

}

void	cEnemyHardBody::MoveByPlayer() {
	old = pos;	// �ߋ����W

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// �ړ��ʃƂ�����
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// �ړ��ʃƂ𑝉�
		}
	}
	else {
		// �L�[�����������ȊO�͎�������
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
	if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
		jump = 20.f;
		++jump_count;
	}
}

void cEnemyHardBody::MoveByAutomation()
{
	if (pos.x - 300 <= FocusPos.x && pos.x + 300 >= FocusPos.x && attack_flag == false) {
		attack_flag = true;
	}
	
	switch (attack_time)
	{
	case 0:
		if (attack_flag == true) {
			attack_time = 1;
		}
		break;
	case 1:
		if (landing == true && jump_count != 3) {
			jump = 15.f;
			jump_count++;
			landing = false;
		}
		else if (jump_count == 3) {
			jump = 30.f;
			landing = false;
			attack_time = 2;
			jump_count = 0;
		}
		
		break;
	case 2:
		if (landing == false) {
			if (pos.x < FocusPos.x)
				pos.x += 2;
			else pos.x -= 2;
		}
		else if (landing == true) {
			cool_time++;
			if (cool_time = 100) {
				attack_time = 3;
				cool_time = 0;
			}
		}
		break;
	case 3:
		attack_flag = false;
		attack_time = 0;
		break;
	default:
		break;
	}

}


void cEnemyWireman::Update()
{
	if (possess) {
		MoveByPlayer();
		FocusOld = FocusPos;
		FocusPos = pos;
	}
	else {
		MoveByAutomation();
	}

	Physical();
}

void cEnemyWireman::WireRender()
{
	if (start_wire == true) {
		DrawLine(pos.x, pos.y, wirepos.x, wirepos.y, 0xffffff);
	}
}

void	cEnemyWireman::MoveByPlayer() {
	old = pos;	// �ߋ����W

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// �ړ��ʃƂ�����
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// �ړ��ʃƂ𑝉�
		}
	}
	else {
		// �L�[�����������ȊO�͎�������
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
	if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
		jump = 20.f;
		++jump_count;
	}
}

void cEnemyWireman::MoveByAutomation() 
{
	if (action_count >= 0 && start_wire == false) {
		if (dir == -1)
			pos.x += 8;
		else if (dir == 1)
			pos.x -= 8;
		action_count--;
		wirepos.x = pos.x;
		wirepos.y = pos.y;
	}
	else if (action_count <= 0 && start_wire == false) {
		if (dir == -1)
			wirepos.x += cos(filing_angle) * 5;
		else if (dir == 1)
			wirepos.x -= cos(filing_angle) * 5;
		wirepos.y -= sin(filing_angle) * 5;
		action_count--;
		if (action_count <= -10) {
			wirepos.y -= 100;
			start_wire = true;
			action_count = 100;
		}
	}
	if (start_wire == true) {
		float rad = rot * PI / 180;
		float px = wirepos.x + cos(rad) * wire_length;			// �U��q�{�̂̍��W
		float py = wirepos.y + sin(rad) * wire_length;			// 

																// �d�͈ړ��ʂ𔽉f�����d��̈ʒu
		float vx = px - wirepos.x;								// �x�_����d��܂ł̃x�N�g�����o��
		float vy = py - wirepos.y;								//
		float t = -(vy * wire_gravity) / (vx * vx + vy * vy);	//  
		float gx = px + t * vx;
		float gy = py + wire_gravity + t * vy;

		// �Q�̏d��̈ʒu�̊p�x��
		float r = atan2(gy - wirepos.y, gx - wirepos.x) * 180 / PI;
		
		
		rad = rot * PI / 180;

			pos.x = wirepos.x + cos(rad) * wire_length;
			pos.y = wirepos.y + sin(rad) * wire_length;

		// �p�x�����p���x�ɉ��Z
		float sub = r - rot;

		sub -= floor(sub / 360.0) * 360.0;
		if (sub < 0.0) {
			sub += 360.0;
		}
		else if (sub > 180.0) {
			sub -= 360.0;
		}

		move_speed += sub;

		// �p�x�Ɋp���x�����Z
		rot += move_speed;

		if (dir == 1 && rot > 170)
			start_wire = false;
		if (dir == -1 && rot < 10)
			start_wire = false;

		// �V�����d��̈ʒu
		// �d��̍��W
		vx = px;
		vy = py;
		if (start_wire == false) {
			jump = 20.f;
			dir *= -1;
			move_speed = -6;
			if (dir == -1)
				move_speed = 6;
			rot = 90.f;
			if (dir == -1)
				rot = 90.f;

		}
		if (ceiling == true) {
			start_wire = false;
			dir *= -1;
			move_speed = -6;
			if (dir == -1)
				move_speed = 6;
			rot = 90.f;
			if (dir == -1)
				rot = 90.f;
		}
	}
}

/*------------------------------------------------------------------------------*
| <<< cEnemyFryingman >>>
*------------------------------------------------------------------------------*/
void cEnemyFryingman::Update()
{
	if (possess) {
		MoveByAutomation();
		FocusOld = FocusPos;
		FocusPos = pos;
	}
	else {
		MoveByAutomation();
	}
}

void	cEnemyFryingman::MoveByPlayer() {
	old = pos;	// �ߋ����W

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// �ړ��ʃƂ�����
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// �ړ��ʃƂ𑝉�
		}
	}
	else {
		// �L�[�����������ȊO�͎�������
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
	if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
		jump = 20.f;
		++jump_count;
	}
}

void cEnemyFryingman::MoveByAutomation()
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
			std::random_device rnd;
			std::mt19937 mt(rnd());
			std::uniform_int_distribution<int> rand5(0, 36);
			rotation_time -= rand5(mt);
		}
	}
	// ��]�J�n
	else if (move_flow == 1){
		rotation_time--;
		angle+=10;
		pos.x = FocusPos.x + cos(angle * PI / 180) * length;
		pos.y = FocusPos.y + sin(angle * PI / 180) * length;
		if (rotation_time <= 0) {
			move_flow = 2;
			rotation_time = 100;	// �N�[���^�C���Ɏg�p
			bulletpos = pos;
		}
	}
	// �e����
	else if (move_flow == 2) {
		firing = true;

		bulletpos = pos;

		rotation_time--;
		if (rotation_time <= 0)
			move_flow = 3;
	}
	// �t�@���l���P��
	else if (move_flow == 3) {
		pos.y -= 5;
	}
	if (firing == true) {
		bullet.Shot(pos,bulletsize, 10,tan(30*PI/180), EnemyBullet);
	}
}
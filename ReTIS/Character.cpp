
#include "Main.h"
VECTOR FocusPos, FocusOld, WirePos ,FocusCam,MouseAdd;
bool AnchorStretch = true;

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
		bullet.Shot(pos, { 3.f, 3.f, 0.f }, 20.f, 0, PlayerBullet);
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
	bottomhit = false;
	// *����������s�������
	switch (dir) {
	case 1: // right
		pos.x = hit->GetPos().x + hit->GetSize().x / 2.f + GetSize().x / 2.f + 1.f;
		inertia = 0;

		// �Փ˔���---------------------
		ceiling = true;
		righthit = true;
		break;
	case 2: // bottom
		pos.y = hit->GetPos().y + hit->GetSize().y / 2.f + GetSize().y / 2.f + 1.f;
		jump = 0.f;

		// �Փ˔���---------------------
		ceiling = true;
		bottomhit = true;
		break;
	case 3: // left
		pos.x = hit->GetPos().x - hit->GetSize().x / 2.f - GetSize().x / 2.f - 1.f;
		inertia = 0;

		// �Փ˔���---------------------
		ceiling = true;
		lefthit = true;
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
		if (jumpman[i] != nullptr) jumpman[i]->Render(jumpman_img);
		if (hardbody[i] != nullptr) hardbody[i]->Render();
		if (wireman[i] != nullptr) wireman[i]->Render(wireman_img,&wmanager[i]->AnchorStretch,&wmanager[i]->EnemyAnchorStretch);
		if (fryingman[i] != nullptr) fryingman[i]->Render(fryingman_img);
		if (wireman[i] != nullptr) wireman[i]->WireRender(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (wireanchor[i] != nullptr) wireanchor[i]->Render();
		if (gunman[i] != nullptr)	gunman[i]->Render(gunman_img);
		if (bossmiddle[i] != nullptr)bossmiddle[i]->Render(bossmiddle_img);
		if (circularsaw[i] != nullptr)circularsaw[i]->Render(circularsaw_img);
		if (cannon[i] != nullptr)cannon[i]->Render(cannon_img);
	}
	if (possess_time != 0) {
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "%d", (600 - possess_time) / 60);
		DrawFormatStringToHandle(FocusPos.x - w / 2, FocusPos.y - 120, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "%d", (600 - possess_time) / 60);
	}
}
void	cCharacterManager::Update() {
	player->Update();
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i] != nullptr) jumpman[i]->Update();
		if (hardbody[i] != nullptr) hardbody[i]->Update();
		if (wireman[i] != nullptr) wireman[i]->Update(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (fryingman[i] != nullptr) fryingman[i]->Update();
		if (wireanchor[i] != nullptr) wireanchor[i]->Update(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (gunman[i] != nullptr)	gunman[i]->Update();
		if (bossmiddle[i] != nullptr)bossmiddle[i]->Update();
		if (circularsaw[i] != nullptr)circularsaw[i]->Update();
		if (cannon[i] != nullptr)cannon[i]->Update();
	}
	PossessListener();
}
void	cCharacterManager::PossessListener() {
	bool IsEnemyHavePossess = false;
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i] != nullptr) if (jumpman[i]->possess == true) IsEnemyHavePossess = true;
		if (hardbody[i] != nullptr) if (hardbody[i]->possess == true) IsEnemyHavePossess = true;
		if (wireman[i] != nullptr) if (wireman[i]->possess == true) IsEnemyHavePossess = true;
		if (fryingman[i] != nullptr) if (fryingman[i]->possess == true) IsEnemyHavePossess = true;
		if (gunman[i] != nullptr) if (gunman[i]->possess == true) IsEnemyHavePossess = true;
	}
	if (IsEnemyHavePossess) {
		player->possess = false;
		possess_time++;
		const int possess_time_max = 60 * 10; // 10�b
		if (possess_time >= possess_time_max) {
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (jumpman[i] != nullptr) jumpman[i]->possess = false;
				if (hardbody[i] != nullptr) hardbody[i]->possess = false;
				if (wireman[i] != nullptr) wireman[i]->possess = false;
				if (fryingman[i] != nullptr) fryingman[i]->possess = false;
				if (gunman[i] != nullptr) gunman[i]->possess = false;
			}
			player->possess = true;
			possess_time = 0;
		}
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
		attack_flag = false;
	}
	Physical();
}

void	cEnemyJumpman::MoveByPlayer() {
	old = pos;	// �ߋ����W

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 6;				// �ړ��ʃƂ�����
										// ���x�������G�Ȃ̂ŏ����グ�Ă��܂�
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 6;				// �ړ��ʃƂ𑝉�
		}
	}
	else {
		// �L�[�����������ȊO�͎�������
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}

	if (key[KEY_INPUT_SPACE] == 1) {
		jump = 30.f;					// *�W�����v�͂���������ƃu���b�N�̏Փ˔��肪����ɂ���Ȃ��Ȃ�܂�
		++jump_count;
		landing = false;
	}

	if (key[KEY_INPUT_C] == 1) {
		attack_flag = true;
	}
}

void cEnemyJumpman::MoveByAutomation()
{
	if (possess == false) {
		if (direction == true) {
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
			jump = 20.f;
			jump_count = 0;
			if (direction == true)
				direction = false;
			else direction = true;
		}
	}
}

void cEnemyJumpman::Render(int image[120])
{
	if (possess == false) {
		if (landing == false) {
			if (image_change > 28) {
				image_change = 0;
			}
			if (image_change != 28)
				image_change++;
		}
		if (landing == true)
			image_change = 0;
	}
	else if (possess == true) {
		if (landing == false) {
			if (image_change > 29)
				image_change = 0;
			if (image_change == 28)
				image_change = 28;
			else image_change++;
		}
		else if (landing == true) {
			if (key[KEY_INPUT_LEFT] != 0 ) {
				if (image_change < 90)
					image_change = 90;
				image_change++;
				if (image_change > 119)
					image_change = 90;
				direction = false;
			}
			else if (key[KEY_INPUT_RIGHT] != 0 ) {
				if (image_change < 90)
					image_change = 90;
				image_change++;
				if (image_change > 119)
					image_change = 90;
				direction = true;
			}
			else if (attack_flag == true) {
				if (image_change < 30 || image_change > 59)
					image_change = 30;
					image_change++;
					if (image_change == 59)
						attack_flag = false;
			}
			else image_change = 0;
		}
	}
	

	if (direction == true)
		DrawRotaGraph(pos.x, pos.y, 0.7, 0, image[image_change], TRUE, TRUE);
	else if (direction == false)
		DrawRotaGraph(pos.x, pos.y, 0.7, 0, image[image_change], TRUE, FALSE);

	DrawCircle(pos.x - 40, pos.y - 50, 10, 0xfffff, true);
	DrawCircle(pos.x + 40, pos.y + 50, 10, 0xfffff, true);
	/*if (landing == true)
		DrawCircle(FocusPos.x, FocusPos.y, 100, 0xfffff, true);*/
}

/*------------------------------------------------------------------------------*
| <<< cEnemyGunman >>>
*------------------------------------------------------------------------------*/
void cEnemyGunman::MoveByAutomation()
{

	if (hp <= 0)
		move_pattern = 3;
	switch (move_pattern) {
	case 0:
		attack_count++;
		if (attack_count == 100)
		{
			lockon = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x);
			bulletpos = pos;
			attack_flag = true;
			attack_count = 0;
			move_pattern = 1;
			image_change = 40;
			if (pos.x < FocusPos.x) {
				bulletpos.x += 100;
				direction = false;
			}
			else {
				bulletpos.x -= 100;
				direction = true;
			}
		}
		break;
	case 1:
		if (attack_count == 0)
			attack_flag = false;
		attack_count++;		
		if (attack_count == 200) {
			attack_count = 0;
			move_pattern = 2;
		}
		break;
	case 2:
		attack_count++;
		if (attack_count == 200) {
			move_pattern = 0;
			attack_count = 0;
		}
		break;
	case 3:
		if (image_change < 156 && image_change > 194)
			image_change = 156;
		image_change++;
		if (image_change == 194)
			image_change == 194;
		break;
	}
}

void cEnemyGunman::MoveByPlayer()
{
	old = pos;	// �ߋ����W

	switch (player_move_pattern) {
	case 0:
		if (key[KEY_INPUT_C] != 0) {
			player_move_pattern = 1;
			image_change = 40;
			attack_count = 0;
		}

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
		if (landing == true)
			jump_count = 0;
		if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
			jump = 20.f;
			++jump_count;
		}
		break;
	case 1:
		attack_count++;
		if (attack_count == 90) {
			player_move_pattern = 0;
			attack_count = 0;
		}
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void cEnemyGunman::Update()
{
	if (possess) {
		MoveByPlayer();		// ���ڂ��Ă�����蓮����
		FocusOld = FocusPos;
		FocusPos = pos;
		move_pattern = 0;
	}
	else {
		MoveByAutomation();	// ���̑��͎���
		player_move_pattern = 0;
	}
	Physical();
}

void cEnemyGunman::Render(int image[])
{
	if (possess == false) {
		switch (move_pattern) {
		case 0:
			image_change = 195;
			break;
		case 1:
			if (image_change != 63)
				image_change++;
			bulletpos.x++;
			//			image_change = 40;
			break;
		case 2:
			image_change = 1;
			break;
		}
	}
	else if (possess == true) {
		switch (player_move_pattern) {
		case 0:
			if (key[KEY_INPUT_SPACE] != 0) {

			}
			if (key[KEY_INPUT_LEFT] != 0) {
				if (image_change < 194)
					image_change = 195;
				direction = true;
				image_change++;
				if (image_change >= 233) {
					image_change = 195;
				}
			}
			else if (key[KEY_INPUT_RIGHT] != 0) {
				if (image_change < 194)
					image_change = 195;
				direction = false;
				image_change++;
				if (image_change >= 233) {
					image_change = 195;
				}
			}
			else image_change = 195;
			break;
		case 1:
			if (image_change != 63)
				image_change++;
			bulletpos.x++;
			break;
		case 2:
			break;
		}
	}
	if (direction == true)
		DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, FALSE);
	else if (direction == false)
		DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, TRUE);
	if (attack_flag == true) {
	bullet.Shot(bulletpos, bulletsize, 10, cos(0.5 * 2 * PI / 360), EnemyBullet);	
	}

	DrawCircle(pos.x - 40, pos.y - 60, 10, 0xfffff, true);
	DrawCircle(pos.x + 40, pos.y + 60, 10, 0xfffff, true);

	//DrawCircle(pos.x, pos.y, 10, 0xffffff, true);
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
	if (landing == true)
		jump_count = 0;
	if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
		jump = 20.f;
		++jump_count;
	}
}

void cEnemyHardBody::MoveByAutomation()
{
	if (possess == false) {
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
		}
	}
}

/*------------------------------------------------------------------------------*
| <<< cEnemyWireManager >>>
*------------------------------------------------------------------------------*/
void cEnemyWiremanManager::Wireman::MouseStateGet()
{
	SetMouseDispFlag(TRUE);
	GetMousePoint(&mouse_posx, &mouse_posy);
	mouse_changeposx = mouse_posx + MouseAdd.x - WINDOW_SIZE_X / 2;
	mouse_changeposy = mouse_posy + MouseAdd.y - WINDOW_SIZE_Y / 2;
}

void cEnemyWiremanManager::Anchor::Update(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch)
{
	if (*AnchorStretch == 0 && *EnemyAnchorStretch == 0) {
		pos.x = -100;
		pos.y = -100;
	}
	else pos = *WirePos;
	if (ceiling == true && *AnchorStretch == 1) {
		*AnchorStretch = 2;
		ceiling = false;
	}
	if (ceiling == true && *EnemyAnchorStretch == 1) {
		*EnemyAnchorStretch = 2;
		ceiling = false;
	}
}

void cEnemyWiremanManager::Anchor::MoveByAutomation(VECTOR *WirePos, int *AnchorStretch)
{

}

void cEnemyWiremanManager::Wireman::Update(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch)
{
	if (possess) {
		MoveByPlayer(WirePos, AnchorStretch);
		FocusOld = FocusPos;
		FocusPos = pos;
		*EnemyAnchorStretch = 0;
	}
	else {
		MoveByAutomation(WirePos, AnchorStretch, EnemyAnchorStretch);
		*AnchorStretch = 0;
	}
	//if (*AnchorStretch != 2 || *EnemyAnchorStretch != 2)
	Physical();
	MouseStateGet();
}

void cEnemyWiremanManager::Wireman::WireRender(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch)
{
	if (*AnchorStretch != 0 || *EnemyAnchorStretch != 0) {
		DrawLineAA(pos.x, pos.y, WirePos->x, WirePos->y, 0xffffff);
	}
	DrawFormatString(FocusCam.x, FocusCam.y + 20, 0xfffff, "%f %f", MouseAdd.x, MouseAdd.y);
	DrawFormatString(FocusCam.x, FocusCam.y + 30, 0xfffff, "%f %f", WirePos->x, WirePos->y);
	DrawCircle(mouse_changeposx, mouse_changeposy, 30, 0xfffff, true);
	DrawFormatString(FocusPos.x, FocusPos.y + 60, 0xfffff, "%d", *AnchorStretch);
}

void	cEnemyWiremanManager::Wireman::MoveByPlayer(VECTOR *WirePos, int *AnchorStretch)
{
	mouse_state = GetMouseInput();
	if (*AnchorStretch == 0) {

	}
	if (*AnchorStretch == 0 && (mouse_state & MOUSE_INPUT_LEFT)) {

		*AnchorStretch = 1;
		*WirePos = pos;
		wire_radian = atan2(mouse_changeposy - pos.y, mouse_changeposx - pos.x);
	}

	else if (*AnchorStretch == 1 && (mouse_state & MOUSE_INPUT_LEFT)) {
		WirePos->x += anchor_speed * cos(wire_radian);
		WirePos->y += anchor_speed * sin(wire_radian);
	}
	else if (*AnchorStretch == 2 && (mouse_state & MOUSE_INPUT_LEFT)) {
		if (WirePos->x < pos.x) {

			if (move_speedx < 15)
				move_speedx += 5;
			pos.x -= 1 + move_speedx;
			debug_int = 1;

			if (WirePos->y <= pos.y) {
				move_speedy += 5;
			}
			else {
				move_speedy -= 5;
			}
			if (bottomhit == true || WirePos->y > pos.y) {
				bottomhit = false;
				move_speedy = 5.f;
			}
			pos.y -= 1 + move_speedy;
		}
		else {
			if (move_speedx > -15)
				move_speedx -= 5;
			pos.x -= move_speedx;
			debug_int = 2;

			if (WirePos->y <= pos.y) {
				move_speedy += 5;
			}
			else {
				move_speedy -= 5;
			}
			if (bottomhit == true || WirePos->y > pos.y) {
				bottomhit = false;
				move_speedy = 5.f;
			}
		}
		pos.y -= move_speedy;

	}
	else if (*AnchorStretch == 2) {
		inertia += move_speedx;
		if (move_speedy > 0)
			jump -= move_speedy;
		*AnchorStretch = 3;
	}
	else {
		*AnchorStretch = 0;
		wire_radian = 0.f;
		move_speedx = 0.f;
		move_speedy = 0.f;
	}

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
	if (landing == true)
		jump_count = 0;
	if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
		jump = 20.f;
		++jump_count;
	}
}

void cEnemyWiremanManager::Wireman::MoveByAutomation(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch)
{
	// *EnemyAnchorStretch�̏�����
	//if (*EnemyAnchorStretch != (0 | 1 | 2)){*EnemyAnchorStretch = 0;}
	if (hp <= 0) {
		*EnemyAnchorStretch == 3;
	}

	if (*EnemyAnchorStretch == 0) {
		enemy_count++;
		if (direction == true)
			pos.x += 4;
		else pos.x -= 4;
		if (enemy_count >= 100) {
			*EnemyAnchorStretch = 1;
			*WirePos = pos;
			enemy_count = 0;
			image_change = 116;
		}
	}
	else if (*EnemyAnchorStretch == 1) {
		if (direction == true) {
			WirePos->x += 5;
			WirePos->y -= 5;
		}
		else {
			WirePos->x -= 5;
			WirePos->y -= 5;
		}
		enemy_count++;

		if (enemy_count >= 60) {
			*EnemyAnchorStretch = 0;
			enemy_count = 0;
			wire_radian = atan2(WirePos->y - pos.y, WirePos->x - pos.x);
			if (direction == false)
				direction = true;
			else direction = false;
		}
	}
	else if (*EnemyAnchorStretch == 2) {
		pos.x += anchor_speed * cos(wire_radian);
		pos.y += anchor_speed * sin(wire_radian);
		if (ceiling == true) {
			*EnemyAnchorStretch = 0;
			bottomhit = false;
			ceiling = false;
			if (direction == false)
				direction = true;
			else direction = false;
		}
	}
	else {
		*EnemyAnchorStretch = 0;
	}
}

void cEnemyWiremanManager::Wireman::Render(int image[], int *AnchorStretch, int *EnemyAnchorStretch)
{
	if (possess == false) {
		switch (*EnemyAnchorStretch) {
		case 0:
			if (image_change > 38)
				image_change = 0;
			image_change++;
			if (image_change == 38)
				image_change = 0;
			break;
		case 1:
			if (image_change <= 133)
				image_change++;
			break;
		case 2:

			break;
		case 3:
			if (image_change < 40 || image_change > 77)
				image_change = 40;
			image_change++;
			if (image_change == 77)
				image_change = 77;
			break;
		}
	}
	else {
		switch (*AnchorStretch) {
		case 0:
			if (key[KEY_INPUT_SPACE] != 0) {

			}
			else if (key[KEY_INPUT_LEFT] != 0) {
				direction = false;
				image_change++;
				if (image_change >= 40) {
					image_change = 0;
				}
			}
			else if (key[KEY_INPUT_RIGHT] != 0) {
				direction = true;
				image_change++;
				if (image_change >= 40) {
					image_change = 0;
				}
			}
			else if (mouse_state & (MOUSE_INPUT_LEFT)) {
				image_change = 116;
			}
			else image_change = 0;
			break;
		case 1:
			if (image_change < 120)
				image_change = 120;
			if (image_change <= 132)
				image_change++;
			break;
		case 2:
			if (image_change <= 119)
				image_change = 120;
			if (image_change != 146)
				image_change++;
			break;
		}
	}
	if (direction == false)
		DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, FALSE);

	else if (direction == true)
		DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, TRUE);
	DrawCircle(pos.x - 40, pos.y - 60, 10, 0xfffff, true);
	DrawCircle(pos.x + 40, pos.y + 60, 10, 0xfffff, true);


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
	// �t�@���l���͔��ł���̂ŃW�����v�͕K�v�Ȃ����Ǝv���܂�
}

void cEnemyFryingman::MoveByAutomation()
{
	if (possess == false) {
		switch (move_flow) {
		case -1:
			if (FocusPos.x + 500 >= pos.x) {
				move_flow = 0;
			}
			break;
		case 0:
			pos.x--;
			if (FocusPos.x + 100 >= pos.x) {
				move_flow = 1;
				image_change = 40;
				bulletpos = pos;
				lockon = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x);
				bullet.Shot(bulletpos, bulletsize, 10, lockon, EnemyBullet);
			}
			break;
		case 1:
			pos.x--;
			pos.y--;
			break;
		case 2:
			break;
		}
	}
}

void cEnemyFryingman::Render(int image[])
{
	switch (move_flow) {
	case 0:
		image_change = 0;
		break;
	case 1:
		if (image_change != 59)
			image_change++;
		break;
	case 2:
		break;
	case 3:
		break;
	}
	DrawGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], TRUE);
}

/*------------------------------------------------------------------------------*
| <<< cEnemyBossmiddle >>>
*------------------------------------------------------------------------------*/
void cEnemyBossmiddle::Update() 
{
	MoveByAutomation();
	Physical();
}

void cEnemyBossmiddle::MoveByAutomation() 
{
	switch(move_pattern) {
		// �����𑪂�
	case 0:
		// �����ċ߂Â�
		if (pos.x < FocusPos.x + 300 && pos.x > FocusPos.x - 300) {
			move_pattern = 1;
			image_change = 160;
		}
		// �W�����v���ċ߂Â�
		else if (pos.x < FocusPos.x + 500 && pos.x > FocusPos.x - 500) {
			move_pattern = 2;
			Player_old = FocusPos;
			image_change = 0;
		}
		break;
		// �v���C���[�ɋ߂Â�
	case 1:
		image_change++;
		
		if (image_change == 189) {
			image_change = 160;
			move_time++;
		}
		if (pos.x < FocusPos.x) {
			pos.x += move_speed;
			direction = false;
		}
		else {
			pos.x -= move_speed;
			direction = true;
		}
		if (move_time == 3) {
			move_time = 0;
			move_pattern = 3;
			image_change = 50;
		}
		break;
		// �W�����v���Ȃ���߂Â�
	case 2:
		if (move_time == 0) {
			jump = 20.f;
			move_time++;
		}
		else if (image_change != 24) {
			image_change++;
			if (pos.x < FocusPos.x) {
				direction = false;
				pos.x+=10;
			}
			else {
				direction = true;
				pos.x-=10;
			}
		}
		else if (landing == true) {
			move_pattern = 3;
			move_time = 0;
			image_change = 50;
		}
		if (move_time != 0) {
			if (pos.x < FocusPos.x)
				pos.x++;
			else pos.x--;
		}
		break;
		// �U��
	case 3:
		if (move_time == 0) {
			if (image_change != 100) {
				image_change++;
			}
			else move_time = 1;
		}
		else {
			move_time++;
			if (move_time == 100) {
				move_time = 0;
				move_pattern = 0;
			}
		}
		break;
		// ���ꂽ�Ƃ�
	case 4:
		image_change++;
		break;

	}
} 

void cEnemyBossmiddle::Render(int image[])
{
	if (direction == true)
		DrawGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], TRUE);
	else if (direction == false)
		DrawTurnGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], TRUE);
}

/*------------------------------------------------------------------------------*
| <<< cEnemyCircularSaw >>>
*------------------------------------------------------------------------------*/

void cEnemyCircularSaw::Update() {
	MoveByAutomation();
}

void cEnemyCircularSaw::MoveByAutomation() {
	switch (move_pattern) {
	case 0:
		attack_count++;
		pos.x += 10;
		if (attack_count == 100) {
			move_pattern = 1;
			attack_count = 0;
		}
		break;
	case 1:
		attack_count++;
		pos.x -= 10;
		if (attack_count == 100) {
			move_pattern = 0;
			attack_count = 0;
		}
		break;
	}
}

void cEnemyCircularSaw::Render(int image[]) {
	image_change++;
	if (image_change == 5)
		image_change = 0;
	if (direction == false)
		DrawTurnGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], true);
	else 	DrawGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], true);
}

/*------------------------------------------------------------------------------*
| <<< cEnemyCannon >>>
*------------------------------------------------------------------------------*/

void cEnemyCannon::Update() 
{
	MoveByAutomation();
	Physical();
}

void cEnemyCannon::Render(int image[]) 
{

	if (image_change > 19)
		image_change = 19;
	else if (image_change <= -1)
		image_change = 0;
	if (direction == false)
		DrawTurnGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], true);
	else 	DrawGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], true);
	DrawFormatString(pos.x, pos.y, 0xffffff, "%d", image_change);
	DrawFormatString(FocusPos.x, FocusPos.y - 100 , 0xfffff, "%f", -angle * 180 / PI);
	DrawFormatString(FocusPos.x, FocusPos.y - 110, 0xfffff, "%f", angle);
}

void cEnemyCannon::MoveByAutomation() 
{
	switch (move_pattern) {
	case 0:
		attack_count++;
		if (pos.x > FocusPos.x) {
			angle = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x);
			image_change = (-angle * 180 / PI) / 4.5 - 19;
			int dainyu = image_change;

			image_change = (dainyu - 19) * -1;

			direction = true;
		}
		else {
			angle = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x);
			image_change = (-angle * 180 / PI) / 4.5;
			direction = false;
		}
		if (attack_count == 300) {
			bulletpos = pos;
			bulletpos.y += sin(angle) *110;
			bulletpos.x += cos(angle) *110;
			bullet.Shot(bulletpos, bulletsize, 10, angle, EnemyBullet);
			move_pattern = 1;
		}
		break;
	case 1:
		
		attack_count = 0;
		move_pattern = 0;
		break;
	case 2:
		break;
	}

}
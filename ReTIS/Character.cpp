
#include "Main.h"
VECTOR FocusPos, FocusOld, FocusCam,MouseAdd;
/*------------------------------------------------------------------------------*
| <<< cCharacterBase >>>
*------------------------------------------------------------------------------*/
void	cCharacterBase::MoveByAutomation() {
	old = pos;
}
void	cCharacterBase::MoveByPlayer() {
	old = pos;	// 過去座標

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// 移動量θを減少
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// 移動量θを増加
		}
	}
	else {
		// キー押し下げ時以外は収束する
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

	if (inertia >  90) inertia = 90;	// はみ出しリミッタ
	if (inertia < -90) inertia = -90;

	pos.x += sin((float)d2r(inertia)) * speed;

	pos.y -= jump;
	jump -= gravity;
}

void	cCharacterBase::Update() {
	// 乗り移る
	if (possess) {
		MoveByPlayer();		// 乗り移っていたら手動操作
		FocusOld = FocusPos;
		FocusPos = pos;
	}
	else {
		MoveByAutomation();	// その他は自動
	}
	// 無敵時間
	if (invincible) {
		const int invicible_time_max = 200;
		++invincible_time;
		if (invincible_time >= invicible_time_max)
			invincible = false;
	}
	// 重力
	Physical();
}

void	cCharacterBase::Render() {
	DrawBoxAA(pos.x - GetSize().x / 2.f, pos.y - GetSize().y / 2.f,
		pos.x + GetSize().x / 2.f, pos.y + GetSize().y / 2.f,
		0xFFFFFF, false);
}

void	cCharacterBase::HitAction(cObject *hit) {

	switch (hit->GetType()) {
	// ----マップタイルとの当たり判定-----------------------------------------------------------
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
	// プレイヤーがチップからみてどの方向にいるのかを過去の座標の差分から求める
	float rad = col_CheckRadian(hit->GetSize(), GetSize());
	int	  dir = col_HitRadian(GetPos(), hit->GetPos(), rad);
	//int   o_dir = col_HitRadian(GetOldPos(), hit->GetPos(), rad);
	// 方向毎に処理
	landing = false;
	ceiling = false;
	// *引っかかる不具合がある
	switch (dir) {
	case 1: // right
		pos.x = hit->GetPos().x + hit->GetSize().x / 2.f + GetSize().x / 2.f + 1.f;
		inertia = 0;

		// 衝突判定---------------------
		ceiling = true;
		break;
	case 2: // bottom
		pos.y = hit->GetPos().y + hit->GetSize().y / 2.f + GetSize().y / 2.f + 1.f;
		jump = 0.f;

		// 衝突判定---------------------
		ceiling = true;
		break;
	case 3: // left
		pos.x = hit->GetPos().x - hit->GetSize().x / 2.f - GetSize().x / 2.f - 1.f;
		inertia = 0;

		// 衝突判定---------------------
		ceiling = true;
		break;
	case 4: // top
		pos.y = hit->GetPos().y - hit->GetSize().y / 2.f - GetSize().y / 2.f - 1.f;
		jump = 0.f;

		// 着地判定---------------------
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
		if (wireman[i]   != nullptr) wireman[i]   ->WireRender(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch);
		if (wireanchor[i] != nullptr) wireanchor[i]->Render();
	}
	if (possess_time != 0) {
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "%d", (600 - possess_time) / 60);
		DrawFormatStringToHandle(FocusPos.x - w / 2, FocusPos.y - 120, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "%d", (600 - possess_time) / 60);
	}
}
void	cCharacterManager::Update() {
	player->Update();
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]   != nullptr) jumpman[i]   ->Update();
		if (hardbody[i]  != nullptr) hardbody[i]  ->Update();
		if (wireman[i]   != nullptr) wireman[i]   ->Update(&wmanager[i]->WirePos,&wmanager[i]->AnchorStretch);
		if (fryingman[i] != nullptr) fryingman[i] ->Update();
		if (wireanchor[i] != nullptr) wireanchor[i]->Update(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch);
	}
	PossessListener();
}
void	cCharacterManager::PossessListener() {
	bool IsEnemyHavePossess = false;
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]   != nullptr) if (jumpman[i]  ->possess == true) IsEnemyHavePossess = true;
		if (hardbody[i]  != nullptr) if (hardbody[i] ->possess == true) IsEnemyHavePossess = true;
		if (wireman[i]   != nullptr) if (wireman[i]  ->possess == true) IsEnemyHavePossess = true;
		if (fryingman[i] != nullptr) if (fryingman[i]->possess == true) IsEnemyHavePossess = true;
	}
	if (IsEnemyHavePossess) {
		player->possess = false;
		possess_time++;
		const int possess_time_max = 60 * 10; // 10秒
		if (possess_time >= possess_time_max) {
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (jumpman[i]   != nullptr) jumpman[i]  ->possess = false;
				if (hardbody[i]  != nullptr) hardbody[i] ->possess = false;
				if (wireman[i]   != nullptr) wireman[i]  ->possess = false;
				if (fryingman[i] != nullptr) fryingman[i]->possess = false;
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
		MoveByPlayer();		// 乗り移っていたら手動操作
		FocusOld = FocusPos;
		FocusPos = pos;
	}
	else {
		MoveByAutomation();	// その他は自動
	}
	Physical();
}

void	cEnemyJumpman::MoveByPlayer() {
	old = pos;	// 過去座標

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 6;				// 移動量θを減少
										// 速度が速い敵なので少し上げています
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 6;				// 移動量θを増加
		}
	}
	else {
		// キー押し下げ時以外は収束する
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
	if (landing == true)
		jump_count = 0;
	if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
		jump = 30.f;					// *ジャンプ力を高くするとブロックの衝突判定が正常にされなくなります
		++jump_count;
	}
}

void cEnemyJumpman::MoveByAutomation()
{
	if (possess == false) {
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
			jump = 20.f;
			jump_count = 0;
			move_dir *= -1;
		}
	}
	if (possess == true) {
		jump_count = 0;
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
	old = pos;	// 過去座標

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// 移動量θを減少
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// 移動量θを増加
		}
	}
	else {
		// キー押し下げ時以外は収束する
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
		default:
			break;
		}
	}
}

void cEnemyWiremanManager::Wireman::MouseStateGet()
{
	SetMouseDispFlag(TRUE);
	GetMousePoint(&mouse_posx, &mouse_posy);
	/*mouse_changeposx = FocusCam.x + mouse_posx - WINDOW_SIZE_X / 2;
	mouse_changeposy = FocusCam.y + mouse_posy - WINDOW_SIZE_Y / 2;*/

	/*mouse_changeposx = FocusPos.x + mouse_posx - WINDOW_SIZE_X / 2;
	mouse_changeposy = FocusPos.y + mouse_posy - WINDOW_SIZE_Y / 2;*/
	mouse_changeposx = mouse_posx + MouseAdd.x - WINDOW_SIZE_X / 2;
	mouse_changeposy = mouse_posy + MouseAdd.y - WINDOW_SIZE_Y / 2;

}

void cEnemyWiremanManager::Anchor::Update(VECTOR *WirePos, int *AnchorStretch)
{
	if(*AnchorStretch == 0) {
		pos.x = -100;
		pos.y = -100;
	}
	else pos = *WirePos;
	if (ceiling == true && *AnchorStretch == 1) {
		*AnchorStretch = 2;
		ceiling = false;
	}

}

void cEnemyWiremanManager::Anchor::MoveByAutomation(VECTOR *WirePos, int *AnchorStretch)
{

}



void cEnemyWiremanManager::Wireman::Update(VECTOR *WirePos, int *AnchorStretch )
{
	if (possess) {
		MoveByPlayer(WirePos,AnchorStretch);
		FocusOld = FocusPos;
		FocusPos = pos;
	}
	else {
		MoveByAutomation(WirePos,AnchorStretch);
		*AnchorStretch = 0;
	}
	if (*AnchorStretch != 2)
		Physical();
	MouseStateGet();

}

void cEnemyWiremanManager::Wireman::WireRender(VECTOR *WirePos, int *AnchorStretch)
{
	if (*AnchorStretch != 0) {
		DrawLine(pos.x, pos.y, WirePos->x, WirePos->y, 0xffffff);
	}
	DrawFormatString(FocusCam.x, FocusCam.y, 0xfffff, "%d %d", mouse_changeposx, mouse_changeposy);
	DrawFormatString(FocusCam.x, FocusCam.y + 10, 0xfffff, "%f",wire_radian);
	DrawFormatString(FocusCam.x, FocusCam.y + 20, 0xfffff, "%f %f", FocusPos.x, FocusPos.y);
	DrawFormatString(FocusCam.x, FocusCam.y + 30, 0xfffff, "%f %f", WirePos->x, WirePos->y);
	DrawCircle(mouse_changeposx, mouse_changeposy, 30, 0xfffff, true);

}

void	cEnemyWiremanManager::Wireman::MoveByPlayer(VECTOR *WirePos, int *AnchorStretch)
{
	mouse_state = GetMouseInput();
	if (AnchorStretch == 0 ) {
		

	}
	if (*AnchorStretch == 0 && (mouse_state & MOUSE_INPUT_LEFT)) {
		
		*AnchorStretch = 1;
		*WirePos = pos;
		wire_radian = atan2(mouse_changeposy - pos.y, mouse_changeposx -pos.x);
	}

	else if (*AnchorStretch == 1 && (mouse_state & MOUSE_INPUT_LEFT)) {
		WirePos->x += anchor_speed * cos(wire_radian);
		WirePos->y += anchor_speed * sin(wire_radian);
	}
	else if (*AnchorStretch == 2 && (mouse_state & MOUSE_INPUT_LEFT)) {
		if (WirePos->x < pos.x) {
			if (WirePos->x >= pos.x) {
				move_speedx -= 2.f;
			}
			else {
				move_speedx += 2.f;
			}
			if (WirePos->y <= pos.y) {
				move_speedy -= 3.f;
			}
			else {
				move_speedy += 3.f;
			}

			//wire_radian = atan2(mouse_changeposy - pos.y, mouse_changeposx - pos.x);

			if (ceiling == true) {
				ceiling = false;
				move_speedx = 0.f;
			}
			if (landing == true) {
				move_speedy = 0.f;
				landing = false;
			}

			pos.x += move_speedx / 3 * cos(wire_radian);
			pos.y += move_speedy * sin(wire_radian);
		}
		else {
			if (WirePos->x >= pos.x) {
				move_speedx += 2.f;
			}
			else {
				move_speedx -= 2.f;
			}
			if (WirePos->y <= pos.y) {
				move_speedy += 3.f;
			}
			else {
				move_speedy -= 3.f;
			}

			//wire_radian = atan2(mouse_changeposy - pos.y, mouse_changeposx - pos.x);

			if (ceiling == true) {
				ceiling = false;
				move_speedx = 0.f;
			}
			if (landing == true) {
				move_speedy = 0.f;
				landing = false;
			}

			pos.x += move_speedx / 3 * cos(wire_radian);
			pos.y += move_speedy * sin(wire_radian);
		}
	}
	else {
		*AnchorStretch = 0;
		wire_radian = 0.f;
		move_speedx = 0.f;
		move_speedy = 0.f;
	}

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// 移動量θを減少
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// 移動量θを増加
		}
	}
	else {
		// キー押し下げ時以外は収束する
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

void cEnemyWiremanManager::Wireman::MoveByAutomation(VECTOR *WirePos, int *AnchorStretch)
{

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
	old = pos;	// 過去座標

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			inertia -= 4;				// 移動量θを減少
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			inertia += 4;				// 移動量θを増加
		}
	}
	else {
		// キー押し下げ時以外は収束する
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
		// ファンネルは飛んでいるのでジャンプは必要ないかと思われます
}

void cEnemyFryingman::MoveByAutomation()
{
	if (possess == false) {
		// プレイヤーがファンネルのセンサーに引っかかったとき
		if (move_flow == -1) {
			if (FocusPos.x >= pos.x) {
				move_flow = 0;
			}
		}
		// 降下開始
		else if (move_flow == 0) {
			pos.x = FocusPos.x + length;
			pos.y += 5;
			if (pos.y >= FocusPos.y) {
				move_flow = 1;
				std::random_device rnd;
				std::mt19937 mt(rnd());
				std::uniform_int_distribution<int> rand5(0, 36);
				rotation_time -= rand5(mt);
			}
		}
		// 回転開始
		else if (move_flow == 1) {
			rotation_time--;
			angle += 10;
			pos.x = FocusPos.x + cos(angle * PI / 180) * length;
			pos.y = FocusPos.y + sin(angle * PI / 180) * length;
			if (rotation_time <= 0) {
				lockon = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x );
				move_flow = 2;
				rotation_time = 100;	// クールタイムに使用
				bulletpos = pos;
				bullet.Shot(bulletpos, bulletsize, 10, cos(lockon * PI * 2 / 360) , EnemyBullet);

			}
		}
		// 弾発射
		else if (move_flow == 2) {
			firing = true;

			bulletpos = pos;

			rotation_time--;
			if (rotation_time <= 0)
				move_flow = 3;

		}
		// ファンネル撤退
		else if (move_flow == 3) {
			pos.y -= 5;
			firing = false;
		}

	}
}
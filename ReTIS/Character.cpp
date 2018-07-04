
#include "Main.h"
VECTOR FocusPos, FocusOld;
/*------------------------------------------------------------------------------*
| <<< cCharacterBase >>>
*------------------------------------------------------------------------------*/
void	cCharacterBase::MoveByAutomation() {
	old = pos;
}
void	cCharacterBase::MoveByPlayer() {
	old = pos;	// 過去座標

	if (key[KEY_INPUT_A] == 2 || key[KEY_INPUT_D] == 2) {
		if (key[KEY_INPUT_A] == 2) {
			inertia -= 4;				// 移動量θを減少
		}
		if (key[KEY_INPUT_D] == 2) {
			inertia += 4;				// 移動量θを増加
		}
	}
	else {
		// キー押し下げ時以外は収束する
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}
	if (key[KEY_INPUT_SPACE] == 1) jump = 20.f;
}

void	cCharacterBase::Physical() {

	if (inertia >  90) inertia = 90;	// はみ出しリミッタ
	if (inertia < -90) inertia = -90;

	pos.x += sin((float)d2r(inertia)) * speed;

	pos.y -= jump;
	jump -= gravity;
}

void	cCharacterBase::Update() {
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

void	cCharacterBase::Render() {
	DrawBoxAA(pos.x - GetSize().x / 2.f, pos.y - GetSize().y / 2.f,
		pos.x + GetSize().x / 2.f, pos.y + GetSize().y / 2.f,
		0xFFFFFF, false);
}

void	cCharacterBase::HitAction(cObject *hit) {

	switch (hit->GetType()) {
	// ----マップタイルとの当たり判定-----------------------------------------------------------
	case MapTile:
		// プレイヤーがチップからみてどの方向にいるのかを過去の座標の差分から求める
		float rad = col_CheckRadian(hit->GetSize(), GetSize());
		int	  dir = col_HitRadian(GetPos(), hit->GetPos(), rad);
		//int   o_dir = col_HitRadian(GetOldPos(), hit->GetPos(), rad);

		// 方向毎に処理
		// *引っかかる不具合がある
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

			// 着地判定---------------------
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
}
void	cCharacterManager::Update() {
	player->Update();
	jumpman->Update();
	gunman->Update();
	hardbody->Update();
}


void cEnemyJumpman::MoveByAutomation()
{
	if (move_dir > 0) {
		pos.x++;
	}
	else {
		pos.x--;
	}

	if (landing == true && jump_count != 3) {
		jump = 20.f;
		jump_count++;
		landing = false;
	}
	if (landing == true &&jump_count == 3) {
		jump = 40.f;
		jump_count = 0;
		move_dir *= -1;
	}
	if (CheckHitKey(KEY_INPUT_X) == 1) {
		bullet->Render();
	}
					// 重力計算の書き方の例
}


// ガンマンの処理

void cEnemyGunman::MoveByAutomation()
{
	pos.x++;
}

VECTOR	GetObjectPos(cCharacterBase *character) {
	return character->GetPos();
}
}


void cEnemyHardBody::Update()
{
	if (possess) {
		MoveByPlayer();		// 乗り移っていたら手動操作
	}
	else {
		MoveByAutomation();	// その他は自動
	}
	Physical();
	Physical();
}
void cEnemyHardBody::MoveByAutomation()
{
	if (landing == false) {
		stop_time+=2;
	}
	else if (landing == true && stop_time != 0) {
		stop_time--;
	}
	else {
		pos.x += move_speed;
	}
}

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
	case MoveFloor:
		Collision(hit);
		if (landing) {
			pos.x += hit->GetPos().x - hit->GetOld().x;
			jump = -4.f;
		}
		break;
	case MapTile:
		Collision(hit);
		break;
	}
}
void	cCharacterBase::Collision(cObject *hit) {
	// プレイヤーがチップからみてどの方向にいるのかを過去の座標の差分から求める
	float rad = col_CheckRadian(hit->GetSize(), GetSize());
	int	  dir = col_HitRadian(GetPos(), hit->GetPos(), rad);
	//int   o_dir = col_HitRadian(GetOldPos(), hit->GetPos(), rad);
	// 方向毎に処理
	landing = false;
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

	// 重力計算の書き方の例
}

void cEnemyJumpman::MoveByAutomation()
{

}


// ガンマンの処理

void cEnemyGunman::MoveByAutomation()
{
	
}

void cEnemyHardBody::Update()
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
	Physical();
}

void cEnemyHardBody::MoveByAutomation()
{


}


void cEnemyWireman::Update()
{
	if (action_count >= 0 && start_wire == false) {
		if (dir == -1)
			pos.x += 10;
		else if (dir == 1)
			pos.x -= 10;
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
		DrawLine(pos.x, pos.y, wirepos.x, wirepos.y, 0xffffff);
		action_count--;
		if (action_count <= -10) {
			wirepos.y -= 100;
			start_wire = true;
			action_count = 100;
		}
	}
	if (start_wire == true) {
		float rad = rot * PI / 180;
		float px = wirepos.x + cos(rad) * wire_length;			// 振り子本体の座標
		float py = wirepos.y + sin(rad) * wire_length;			// 

																// 重力移動量を反映した重りの位置
		float vx = px - wirepos.x;								// 支点から重りまでのベクトルを出す
		float vy = py - wirepos.y;								//
		float t = -(vy * wire_gravity) / (vx * vx + vy * vy);	//  
		float gx = px + t * vx;
		float gy = py + wire_gravity + t * vy;

		// ２つの重りの位置の角度差
		float r = atan2(gy - wirepos.y, gx - wirepos.x) * 180 / PI;
		
		
		rad = rot * PI / 180;

			pos.x = wirepos.x + cos(rad) * wire_length;
			pos.y = wirepos.y + sin(rad) * wire_length;

		// 角度差を角速度に加算
		float sub = r - rot;

		sub -= floor(sub / 360.0) * 360.0;
		if (sub < 0.0) {
			sub += 360.0;
			
		}
		else if (sub > 180.0) {
			sub -= 360.0;
		}

		move_speed += sub;

		// 角度に角速度を加算
		rot += move_speed;

		if (dir == 1 && rot > 170)
			start_wire = false;
		if (dir == -1 && rot < 10)
			start_wire = false;

		// 新しい重りの位置
		// 重りの座標
		vx = px;
		vy = py;
		if (start_wire == false) {
			jump = 20.f;
			dir *= -1;
			move_speed = -4;
			if (dir == -1)
				move_speed = 4;
			rot = 45.f;
			if (dir == -1)
				rot = 135.f;
		}
	}
	else {
		Physical();
	}
}

void cEnemyWireman::MoveByAutomation() 
{

}


void cEnemyFryingman::Update()
{
	// プレイヤーがファンネルのセンサーに引っかかったとき
	if (move_flow == -1) {
		if (FocusPos.x >= pos.x) {
			move_flow = 0;
		}
	}
	// 降下開始
	else if (move_flow == 0) {
		pos.x = FocusPos.x + length;
		pos.y+=5;
		if (pos.y >= FocusPos.y) {
			move_flow = 1;
		}
	}
	//回転開始
	else if (move_flow == 1){
		rotation_time--;
		angle+=10;
		pos.x = FocusPos.x + cos(angle * PI / 180) * length;
		pos.y = FocusPos.y + sin(angle * PI / 180) * length;
		if (rotation_time <= 0) {
			move_flow = 2;
			rotation_time = 100;	// クールタイムに使用
		}
	}
	// 弾発射
	else if (move_flow == 2) {
		// 弾発射の処理
		rotation_time--;
		if (rotation_time <= 0)
			move_flow = 3;
	}
	// ファンネル撤退
	else if (move_flow == 3) {
		pos.y -= 5;
	}

}
void cEnemyFryingman::MoveByAutomation()
{

}
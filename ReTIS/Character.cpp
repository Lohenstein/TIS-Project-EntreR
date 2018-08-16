
#include "Main.h"
VECTOR FocusPos, FocusOld, WirePos ,FocusCam,MouseAdd;
bool	AnchorStretch = true;
bool	IsClearFlag, IsOverFlag;
int		coin, ecoin, rcoin;
int		mp;

using namespace std;

/*------------------------------------------------------------------------------*
| <<< cCharacterBase >>>
*------------------------------------------------------------------------------*/
void	cCharacterBase::MoveByAutomation() {
	old = pos;
	if (inertia > 0) inertia -= 2;
	if (inertia < 0) inertia += 2;
}
void	cCharacterBase::MoveByPlayer() {
	old = pos;	// 過去座標

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
		if (key[KEY_INPUT_LEFT] == 2) {
			rect = true;
			inertia -= 4;				// 移動量θを減少
		}
		if (key[KEY_INPUT_RIGHT] == 2) {
			rect = false;
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
	if (key[KEY_INPUT_C] == 1 && mp >= 300) {
		mp = 0;
		bullet.Shot(pos, { 3.f, 3.f, 0.f }, 20.f, 0, PlayerBullet);
	}
	// 穴に落っこちた
	if (pos.y >= 3520) {
		IsOverFlag = true;
	}
}

void	cCharacterBase::Physical() {

	if (inertia >  90) inertia = 90;	// はみ出しリミッタ
	if (inertia < -90) inertia = -90;

	if (landing == true) {
		if (inertia == 0) {
			animmode = 1;
		}
		else {
			animmode = 0;
		}
		anim++;
		if (anim >= 30) anim = 0;
		animjump = 0;
	}
	else {
		if (animjump == 0) anim = 0;
		animmode = 2;
		anim++;
		animjump++;
		if (anim >= 30) anim = 30;
	}


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
	case DropFloor:
		Collision(hit);
		break;
	case Enemy:
		Damaged();
		break;
	case EnemyBullet:
		Damaged();
		break;
	case PlayerBullet:
		possess = true;
		break;
	case PlayerAttack:
		Damaged();
		break;
	case MapTile:
		Collision(hit);
		break;
	case Clear:
		if (this->GetType() == Player) {
			IsClearFlag = true;
		}
		break; 
	case NormalCoin:
		if (possess) coin++;
		break;
	case EneCoin:
		if (possess) ecoin++;
		break;
	case RareCoin:
		if (possess) rcoin++;
		break;
	}
}
void	cCharacterBase::Damaged() {
	if (invincible == false) {
		invincible = true;
		invincible_time = 0;
		hp--;
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
	bottomhit = false;
	// *引っかかる不具合がある
	switch (dir) {
	case 1: // right
		pos.x = hit->GetPos().x + hit->GetSize().x / 2.f + GetSize().x / 2.f + 1.f;
		inertia = 0;

		// 衝突判定---------------------
		ceiling = true;
		righthit = true;
		break;
	case 2: // bottom
		pos.y = hit->GetPos().y + hit->GetSize().y / 2.f + GetSize().y / 2.f + 1.f;
		jump = 0.f;

		// 衝突判定---------------------
		ceiling = true;
		bottomhit = true;
		break;
	case 3: // left
		pos.x = hit->GetPos().x - hit->GetSize().x / 2.f - GetSize().x / 2.f - 1.f;
		inertia = 0;

		// 衝突判定---------------------
		ceiling = true;
		lefthit = true;
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

	int halfw = 70;
	int halfh = 65;

	if (invincible) {
		if (invincible_time % 3 == 0) {
			DrawRotaGraph(pos.x, pos.y - 5.f, 0.28, 0.0, img[animmode][anim], true, rect);
		}
	}
	else {
		DrawRotaGraph(pos.x, pos.y - 5.f, 0.28, 0.0, img[animmode][anim], true, rect);
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
	clear->DebugRender();
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]		!= nullptr) jumpman[i]->Render(jumpman_img);
		if (hardbody[i]		!= nullptr) hardbody[i]->Render();
		if (wireman[i]		!= nullptr) wireman[i]->Render(wireman_img,&wmanager[i]->AnchorStretch,&wmanager[i]->EnemyAnchorStretch);
		if (fryingman[i]	!= nullptr) fryingman[i]->Render(fryingman_img);
		if (wireman[i]		!= nullptr) wireman[i]->WireRender(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (wireanchor[i]	!= nullptr) wireanchor[i]->Render();
		if (gunman[i]		!= nullptr)	gunman[i]->Render(gunman_img);
		if (bossmiddle[i]	!= nullptr) bossmiddle[i]->Render(bossmiddle_img);
		if (circularsaw[i]	!= nullptr) circularsaw[i]->Render(circularsaw_img);
		if (cannon[i]		!= nullptr) cannon[i]->Render(cannon_img);
		if (movefloor[i]	!= nullptr) movefloor[i]->Render();
		if (dropfloor[i]	!= nullptr) dropfloor[i]->Render();
		if (coin[i]			!= nullptr) coin[i]->Render(coin_img);
	}
	if (possess_time != 0) {
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "%d", (600 - possess_time) / 60);
		DrawFormatStringToHandle(FocusPos.x - w / 2, FocusPos.y - 120, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "%d", (600 - possess_time) / 60);
	}
}
void	cCharacterManager::Update() {
	player->Update();
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]		!= nullptr) jumpman[i]->Update();
		if (hardbody[i]		!= nullptr) hardbody[i]->Update();
		if (wireman[i]		!= nullptr) wireman[i]->Update(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (fryingman[i]	!= nullptr) fryingman[i]->Update();
		if (wireanchor[i]	!= nullptr) wireanchor[i]->Update(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (gunman[i]		!= nullptr)	gunman[i]->Update();
		if (bossmiddle[i]	!= nullptr) bossmiddle[i]->Update();
		if (circularsaw[i]	!= nullptr) circularsaw[i]->Update(5.f, 0, 1);
		if (cannon[i]		!= nullptr) cannon[i]->Update();
		if (movefloor[i]	!= nullptr) movefloor[i]->Update(3.f, 0, 1);
		if (dropfloor[i]	!= nullptr) dropfloor[i]->Update();
		if (coin[i]			!= nullptr) coin[i]->Update();
	}
	PossessListener();
	DeleteDeathCharacters();
	if (mp > 300) mp = 300;
}
void	cCharacterManager::PossessListener() {
	bool IsEnemyHavePossess = false;
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]   != nullptr) if (jumpman[i]->possess   == true) IsEnemyHavePossess = true;
		if (hardbody[i]  != nullptr) if (hardbody[i]->possess  == true) IsEnemyHavePossess = true;
		if (wireman[i]   != nullptr) if (wireman[i]->possess   == true) IsEnemyHavePossess = true;
		if (fryingman[i] != nullptr) if (fryingman[i]->possess == true) IsEnemyHavePossess = true;
		if (gunman[i]    != nullptr) if (gunman[i]->possess    == true) IsEnemyHavePossess = true;
	}
	if (IsEnemyHavePossess) {
		player->possess = false;
		possess_time++;
		const int possess_time_max = 60 * 10; // 10秒
		if (possess_time >= possess_time_max) {
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (jumpman[i]   != nullptr) jumpman[i]->possess   = false;
				if (hardbody[i]  != nullptr) hardbody[i]->possess  = false;
				if (wireman[i]   != nullptr) wireman[i]->possess   = false;
				if (fryingman[i] != nullptr) fryingman[i]->possess = false;
				if (gunman[i]    != nullptr) gunman[i]->possess    = false;
			}
			player->possess = true;
			possess_time = 0;
		}
	}
}

void	cCharacterManager::DeleteCharacters() {

	delete player;
	player = nullptr;
	for (int i = 0; i < ENEMY_MAX; i++) {

		delete jumpman[i];
		delete hardbody[i];
		delete wireman[i];
		delete fryingman[i];
		delete gunman[i];
		delete bossmiddle[i];
		delete dropfloor[i];
		delete movefloor[i];

		jumpman[i] = nullptr;
		hardbody[i] = nullptr;
		wireman[i] = nullptr;
		fryingman[i] = nullptr;
		gunman[i] = nullptr;
		bossmiddle[i] = nullptr;
		dropfloor[i] = nullptr;
		movefloor[i] = nullptr;

	}
}


void	cCharacterManager::DeleteDeathCharacters() {
	if (player->GetHp() <= 0) {
		IsOverFlag = true;
	}
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i] != nullptr) {
			if (jumpman[i]->GetHp() <= 0) {
				if (jumpman[i]->possess) {
					player->possess = true;
					possess_time = 0;
				}
				delete jumpman[i];
				jumpman[i] = nullptr;
			}
		}
		if (hardbody[i] != nullptr) {
			if (hardbody[i]->GetHp() <= 0) {
				if (hardbody[i]->possess) {
					player->possess = true;
					possess_time = 0;
				}
				delete hardbody[i];
				hardbody[i] = nullptr;
			}
		}
		if (fryingman[i] != nullptr) {
			if (fryingman[i]->GetHp() <= 0) {
				if (fryingman[i]->possess) {
					player->possess = true;
					possess_time = 0;
				}
				delete fryingman[i];
				fryingman[i] = nullptr;
			}
		}
		if (gunman[i] != nullptr) {
			if (gunman[i]->GetHp() <= 0) {
				if (gunman[i]->possess) {
					player->possess = true;
					possess_time = 0;
				}
				delete gunman[i];
				gunman[i] = nullptr;
			}
		}
		if (bossmiddle[i] != nullptr) {
			if (bossmiddle[i]->GetHp() <= 0) {
				if (bossmiddle[i]->possess) {
					player->possess = true;
					possess_time = 0;
				}
				delete bossmiddle[i];
				bossmiddle[i] = nullptr;
			}
		}
		if (coin[i] != nullptr) {
			if (coin[i]->GetHp() <= 0) {
				delete coin[i];
				coin[i] = nullptr;
			}
		}
	}
}
void	cCharacterManager::LoadCharacters(string name) {

	// 複数のcsv読み込むきたない機構
	string enemy = name + "mapdata_enemy.csv";
	ifstream ifs; ifs.open(enemy.c_str());
	string line;

	int count = 0;
	while (getline(ifs, line)) {

		vector<string> str = split(line, ',');

		switch (stoi(str.at(0))) {
		case ePlayer:
			player = new cPlayer(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
			break;
		case eClear:
			clear = new cClearCollision(stoi(str.at(1)), stoi(str.at(2)));
			break;
		case eJumpman:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (jumpman[i] == nullptr) {
					jumpman[i] = new cEnemyJumpman(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eHardbody:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (hardbody[i] == nullptr) {
					hardbody[i] = new cEnemyHardBody(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eFryingman:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (fryingman[i] == nullptr) {
					fryingman[i] = new cEnemyFryingman(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eGunman:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (gunman[i] == nullptr) {
					gunman[i] = new cEnemyGunman(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eWireman:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (wireman[i] == nullptr) {
					wireman[i] = new cEnemyWiremanManager::Wireman(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eBossmiddle:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (bossmiddle[i] == nullptr) {
					bossmiddle[i] = new cEnemyBossmiddle(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eCircularsaw:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (circularsaw[i] == nullptr) {
					circularsaw[i] = new cEnemyCircularSaw(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)));
					break;
				}
			}
			break;
		case eCannon:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (cannon[i] == nullptr) {
					cannon[i] = new cEnemyCannon(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eMoveFloor:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (movefloor[i] == nullptr) {
					movefloor[i] = new cMoveFloor(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)));
					break;
				}
			}
			break;
		case eDropFloor:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (dropfloor[i] == nullptr) {
					dropfloor[i] = new cDropFloor(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)));
					break;
				}
			}
			break;
		case eCoin:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (coin[i] == nullptr) {
					coin[i] = new cCoin(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)));
					break;
				}
			}
			break;
		default:
			MessageBox(NULL, "キャラクターシート読み込み時に\n存在しないパラメータが読み込まれました。", "Debug - Error", MB_OK);
			break;
		}
		count++;
	}
}
/*------------------------------------------------------------------------------*
| <<< cEnemyJumpman >>>
*------------------------------------------------------------------------------*/
void cEnemyJumpman::Update()
{
	if (hp > 0) {
		if (possess) {
			MoveByPlayer();		// 乗り移っていたら手動操作
			FocusOld = FocusPos;
			FocusPos = pos;
		}
		else {
			MoveByAutomation();	// その他は自動
			attack_flag = false;
		}
	}
	if (image_change == 89)
		hp = 0;
//	if (hp == 0)
	//	possess = false;
	Physical();
}

void cEnemyJumpman::MoveByPlayer() {

	if (hp > 0) {
		if (key[KEY_INPUT_C] == 1 && attack_flag == false) {
			attack_flag = true;
		}

		if (attack_flag == false) {
			if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
				if (key[KEY_INPUT_LEFT] == 2) {
					inertia -=4;			 	// 移動量θを減少
												// 速度が速い敵なので少し上げています
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
			if (inertia >  90) inertia = 90;	// はみ出しリミッタ
			if (inertia < -90) inertia = -90;

			if (key[KEY_INPUT_SPACE] == 1) {
				jump = 30.f;					// *ジャンプ力を高くするとブロックの衝突判定が正常にされなくなります
				++jump_count;
				landing = false;
			}
		}
	}
}

void cEnemyJumpman::MoveByAutomation()
{
	if (hp > 0) {
		inertia = 0;
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
	if (hp >= 1) {
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
				if (key[KEY_INPUT_LEFT] != 0 && attack_flag == false) {
					if (image_change < 90)
						image_change = 90;
					image_change++;
					if (image_change > 119)
						image_change = 90;
					direction = false;
				}
				else if (key[KEY_INPUT_RIGHT] != 0 && attack_flag == false) {
					if (image_change < 90)
						image_change = 90;
					image_change++;
					if (image_change > 119)
						image_change = 90;
					direction = true;
				}
				else if (attack_flag == true) {
					inertia = 0;
					player_attack = pos;
					if (direction == true) {
						player_attack.x += 100;
						bullet.Shot(player_attack, { 10,10,0 }, 0, 180, EnemyBullet);
					}
					else {
						player_attack.x -= 100;
						bullet.Shot(player_attack, { 10,10,0 }, 0, 180, EnemyBullet);
					}
					if (image_change < 30 || image_change > 59)
						image_change = 30;
					image_change++;
					if (image_change == 59) {
						attack_flag = false;
					}
				}
				else image_change = 0;
			}
		}
	}
	else if (hp == 1){
		// 死んだ処理
		if (image_change < 60 || image_change > 90)
			image_change = 60;
		if (image_change != 89)
			image_change++;
	}
	
	if (direction == true)
		DrawRotaGraph(pos.x, pos.y, 0.7, 0, image[image_change], TRUE, TRUE);
	else if (direction == false)
		DrawRotaGraph(pos.x, pos.y, 0.7, 0, image[image_change], TRUE, FALSE);
}

/*------------------------------------------------------------------------------*
| <<< cEnemyGunman >>>
*------------------------------------------------------------------------------*/
void cEnemyGunman::MoveByAutomation()
{
	if (hp > 0) {
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
			}
			break;
		case 1:
			attack_count++;
			if (attack_count == 1) {
				pos.x < FocusPos.x ? direction = false : direction = true;
				
			}
			if (attack_flag) {
				if (image_change == 54) {
					attack_flag = false;
					bulletpos = pos;
					if (direction) {
						bulletpos.y -= 40;
						bulletpos.x -= 40;
					}
					else {

						bulletpos.y -= 40;
						bulletpos.x += 40;
					}
					direction ? bullet.Shot(bulletpos, bulletsize, 10, PI, EnemyBullet): bullet.Shot(bulletpos, bulletsize, -10, PI, EnemyBullet);
				}
			}
			if (attack_count == 100) {
				attack_count = 0;
				move_pattern = 2;
				image_change = 53;
			}
			break;
		case 2:
			attack_count++;
			if (image_change != 40) image_change--;
			if (attack_count == 100) {
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
}

void cEnemyGunman::MoveByPlayer()
{
	if (hp > 0) {
		switch (player_move_pattern) {
		case 0:
			if (key[KEY_INPUT_C] == 1) {
				player_move_pattern = 1;
				image_change = 40;
				attack_count = 0;
				//bulletpos = pos;
				inertia = 0;
			}

			if (landing)
				jump_count = 0;
			if (key[KEY_INPUT_SPACE] == 1 && jump_count < 2) {
				jump = 20.f;
				++jump_count;
			}
			if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2) {
				if (key[KEY_INPUT_LEFT] == 2) {
					inertia -= 4;			 	// 移動量θを減少
					direction = true;			// 速度が速い敵なので少し上げています
				}
				if (key[KEY_INPUT_RIGHT] == 2) {
					inertia += 4;				// 移動量θを増加
					direction = false;
				}
			}

			if (inertia >  90) inertia = 90;	// はみ出しリミッタ
			if (inertia < -90) inertia = -90;
			break;
		case 1:
			attack_count++;
			if (image_change == 54) {
				bulletpos = pos;
				// 玉の初期座標
				direction ? bulletpos.x -= 40:bulletpos.x += 40;
				bulletpos.y -= 40;
				// 玉の方向
				attack_count ? bullet.Shot(bulletpos, bulletsize, 10, PI, PlayerAttack) : bullet.Shot(bulletpos, bulletsize, -10, PI, PlayerAttack);
			}
			if (attack_count == 65) {
				image_change = 53;
				player_move_pattern = 2;
				attack_count = 0;
			}
			break;
		case 2:
			if (image_change != 40)
				image_change--;
			else {
				player_move_pattern = 0;
			}
			break;
		case 3:
			break;
		}
	}
	if (inertia > 0) inertia -= 2;
	if (inertia < 0) inertia += 2;
}

void cEnemyGunman::Update()
{
	if (possess) {
		MoveByPlayer();		// 乗り移っていたら手動操作
		FocusOld = FocusPos;
		FocusPos = pos;
		move_pattern = 0;
	}
	else {
		MoveByAutomation();	// その他は自動
		player_move_pattern = 0;
	}
	if (image_change == 179)
		hp = 0;
	Physical();
}

void cEnemyGunman::Render(int image[])
{
	if (hp > 1) {
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
					image_change++;
					if (image_change >= 233) {
						image_change = 195;
					}
				}
				else if (key[KEY_INPUT_RIGHT] != 0) {
					if (image_change < 194)
						image_change = 195;
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
				break;
			case 2:

				break;
			}
		}
	}
	else if (hp == 1){
		if (image_change < 156 || image_change > 181)
			image_change = 156;

		if (image_change == 180)
			image_change = 180;
		else		image_change++;
	}

	if (direction == true)
		DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, FALSE);
	else if (direction == false)
		DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, TRUE);
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
	/*f (possess == false) {
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
	}*/
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
	Physical();
	MouseStateGet();
}

void cEnemyWiremanManager::Wireman::WireRender(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch)
{
	if (*AnchorStretch != 0 || *EnemyAnchorStretch != 0) {
		DrawLineAA(pos.x, pos.y, WirePos->x, WirePos->y, 0xffffff);
	}
	/*DrawFormatString(FocusCam.x, FocusCam.y + 20, 0xfffff, "%f %f", MouseAdd.x, MouseAdd.y);
	DrawFormatString(FocusCam.x, FocusCam.y + 30, 0xfffff, "%f %f", WirePos->x, WirePos->y);
	DrawCircle(mouse_changeposx, mouse_changeposy, 30, 0xfffff, true);
	DrawFormatString(FocusPos.x, FocusPos.y + 60, 0xfffff, "%d", *AnchorStretch);*/
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

void cEnemyWiremanManager::Wireman::MoveByAutomation(VECTOR *WirePos, int *AnchorStretch, int *EnemyAnchorStretch)
{
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
	if (hp > 1) {
		MoveByAutomation();
	}
	if (image_change == 149) {
		hp = 0;
	}
	Physical();
}

void cEnemyBossmiddle::MoveByAutomation() 
{
	if (hp > 1) {
		switch (move_pattern) {
			// 距離を測る
		case 0:
			// 歩いて近づく
			if (pos.x < FocusPos.x + 300 && pos.x > FocusPos.x - 300) {
				move_pattern = 1;
				image_change = 160;
			}
			// ジャンプして近づく
			else if (pos.x < FocusPos.x + 400 && pos.x > FocusPos.x - 400) {
				move_pattern = 2;
				Player_old = FocusPos;
				image_change = 0;
			}
			break;
			// プレイヤーに近づく
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
			// ジャンプしながら近づく
		case 2:
			if (move_time == 0) {
				jump = 20.f;
				move_time++;
			}
			else if (image_change != 24) {
				image_change++;
				if (pos.x < FocusPos.x) {
					direction = false;
					pos.x += 10;
				}
				else {
					direction = true;
					pos.x -= 10;
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
			// 攻撃
		case 3:
			if (move_time == 0) {
				if (image_change != 100) {
					Attack_pos = pos;
					if (direction == false && (image_change == 66 || image_change == 86)) {
						Attack_pos.x += 150;
						bullet.Shot(Attack_pos, { 10,10,0 }, 0, 0, EnemyBullet);
					}
					else if (direction == true && (image_change == 66 || image_change == 86)){
						Attack_pos.x -= 150;
						bullet.Shot(Attack_pos, { 10,10,0 }, 0, 0, EnemyBullet);
					}
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
		}
	}
} 

void cEnemyBossmiddle::Render(int image[])
{
	if (hp < 1) {
		if (image_change < 100 || image_change > 149)
			image_change = 100;
		if (image_change != 149)
			image_change++;
	}

	else if (hp == 1) {
		int change = 0;
		switch (change) {
		case 0:
			image_change = 101;
			change = 0;
			break;
		case 1:
			image_change++;
			break;
		}
		image_change = 100;

	}
	
	if (direction == true)
		DrawGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], TRUE);
	else if (direction == false)
		DrawTurnGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], TRUE);
}

/*------------------------------------------------------------------------------*
| <<< cEnemyCircularSaw >>>
*------------------------------------------------------------------------------*/

void cEnemyCircularSaw::Update(float s,int p1,int p2) {
	// 移動する角度と距離を求める
	float rad = atan2f(sy[1] - sy[0], sx[1] - sx[0]);
	float dis = sqrtf((sx[1] - sx[0]) * (sx[1] - sx[0]) + (sy[1] - sy[0]) * (sy[1] - sy[0]));

	// 過去の移動を保存
	old = pos;

	// 移動する
	pos.x = sx[0] - (cosf(rad + PI) * p);
	pos.y = sy[0] - (sinf(rad + PI) * p);

	// 移動の方向に向かって動かす
	if (flag)
		p += s;
	else
		p -= s;

	// 移動の方向の切り替え
	if (p >= dis || p <= 0.f) {
		if (flag)
			flag = false;
		else
			flag = true;
	}

}

void cEnemyCircularSaw::SetPoint(int num, float x, float y) {
	sx[num] = x;
	sy[num] = y;
}

void cEnemyCircularSaw::Render(int image[]) {
	image_change++;
	if (image_change == 5)
		image_change = 0;
	DrawGraph(pos.x - 150.f, pos.y - 150.f, image[image_change], true);
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

/*------------------------------------------------------------------------------*
| <<< cCoin >>>
*------------------------------------------------------------------------------*/

void cCoin::Update() 
{
	
}

void cCoin::Render(int image[]) 
{
	DrawRotaGraph(pos.x, pos.y, 0.7, 10 ,image[cointype], TRUE , FALSE);
}
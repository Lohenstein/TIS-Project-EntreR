
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
	FocusOld = FocusPos;
	FocusPos = pos;

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
		if (this->GetType() == Player) Damaged();
		break;
	case EnemyBullet:
		if (this->GetType() == Player) Damaged();
		break;
	case PlayerBullet:
		if (this->GetType() != Player) Damaged();
		break;
	case MapTile:
		Collision(hit);
		break;
	case Clear:
		if (this->GetType() == Player) IsClearFlag = true;
		break; 
	case NormalCoin:
		if (this->GetType() == Player) coin++;
		break;
	case EneCoin:
		if (this->GetType() == Player) ecoin++;
		break;
	case RareCoin:
		if (this->GetType() == Player) rcoin++;
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
	if (anchor != nullptr) anchor->Render(pos);
}

void	cPlayer::UpdateAnchor() {
	if (anchor != nullptr) {
		// 飛んでるとき
		if (anchor->GetFlag()) {
			anchor->Update();
			if (sqrtf((anchor->GetPos().x - pos.x) * (anchor->GetPos().x - pos.x) + (anchor->GetPos().y - pos.y) * (anchor->GetPos().y - pos.y)) >= 400.f) {
				delete anchor;
				anchor = nullptr;
				IsAnchored = false;
			}
		}
		else {
			// くっついたとき
			if (!IsAnchored) {
				savepos = pos;
				rad2anchor = (DX_PI_F / 2) + atan2f(anchor->GetPos().y - pos.y, anchor->GetPos().x - pos.x);
				dis2anchor = sqrtf((anchor->GetPos().x - pos.x) * (anchor->GetPos().x - pos.x) + (anchor->GetPos().y - pos.y) * (anchor->GetPos().y - pos.y));
				IsAnchored = true;
				swing = 0.f;
				jump_count = 0;
			}
			else {
				wrad = cos(swing) * (rad2anchor / (PI / 2.f));
				pos.x = anchor->GetPos().x + cos(wrad + DX_PI_F / 2.f) * dis2anchor;
				pos.y = anchor->GetPos().y + sin(wrad + DX_PI_F / 2.f) * dis2anchor;
				swing += DX_PI_F / 90;
				jump = 0.f;
			}
		}
	}
}

void	cPlayer::Update() {
	old = pos;	// 過去座標

	// 乗り移る
	FocusOld = FocusPos;
	FocusPos = pos;

	if (key[KEY_INPUT_LEFT] == 2 || key[KEY_INPUT_RIGHT] == 2 || stick_lx >= 100 || stick_lx <= -100) {
		if (key[KEY_INPUT_LEFT] == 2 || stick_lx <= -100) {
			rect = true;
			inertia -= 4;				// 移動量θを減少
		}
		if (key[KEY_INPUT_RIGHT] == 2 || stick_lx >= 100) {
			rect = false;
			inertia += 4;				// 移動量θを増加
		}
	}
	else {
		// キー押し下げ時以外は収束する
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
	}

	if ((key[KEY_INPUT_SPACE] == 1 || pad_b[XINPUT_BUTTON_A] == 1) && jump_count < 2) {
		jump = 20.f;
		++jump_count;
	}
	if ((key[KEY_INPUT_C] == 1 || pad_b[XINPUT_BUTTON_X] == 1) && mp >= 10) {
		mp -= 10;
		bullet.Shot(pos, { 3.f, 3.f, 0.f }, 20.f, PI * rect, PlayerBullet);
	}
	if (pad_b[XINPUT_BUTTON_Y] == 1) {
		delete anchor;
		IsAnchored = false;
		anchor = nullptr;
		anchor = new cAnchor(pos, { 10.f, 10.f, 10.f }, 20.f, -stick_rad, WireAnchor);
	}
	// 穴に落っこちた
	if (pos.y >= 3520) {
		IsOverFlag = true;
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
	UpdateAnchor();
}
void	cPlayer::HitAction(cObject *hit) {

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
		if (this->GetType() == Player) Damaged();
		break;
	case EnemyBullet:
		if (this->GetType() == Player) Damaged();
		break;
	case PlayerBullet:
		if (this->GetType() != Player) Damaged();
		break;
	case MapTile:
		Collision(hit);
		if (anchor != nullptr) {
			IsAnchored = false;
			delete anchor;
			anchor = nullptr;
			pos = old;
		}
		break;
	case Clear:
		if (this->GetType() == Player) IsClearFlag = true;
		break;
	case NormalCoin:
		if (this->GetType() == Player) coin++;
		break;
	case EneCoin:
		if (this->GetType() == Player) ecoin++;
		break;
	case RareCoin:
		if (this->GetType() == Player) rcoin++;
		break;
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
		if (jumpman[i]		!= nullptr) jumpman[i]		->Render(jumpman_img);
		if (hardbody[i]		!= nullptr) hardbody[i]		->Render();
		if (wireman[i]		!= nullptr) wireman[i]		->Render(wireman_img,&wmanager[i]->AnchorStretch,&wmanager[i]->EnemyAnchorStretch);
		if (fryingman[i]	!= nullptr) fryingman[i]	->Render(fryingman_img);
		if (wireman[i]		!= nullptr) wireman[i]		->WireRender(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (wireanchor[i]	!= nullptr) wireanchor[i]	->Render();
		if (gunman[i]		!= nullptr)	gunman[i]		->Render(gunman_img);
		if (bossmiddle[i]	!= nullptr) bossmiddle[i]	->Render(bossmiddle_img);
		if (circularsaw[i]	!= nullptr) circularsaw[i]	->Render(circularsaw_img);
		if (cannon[i]		!= nullptr) cannon[i]		->Render(cannon_img);
		if (movefloor[i]	!= nullptr) movefloor[i]	->Render();
		if (dropfloor[i]	!= nullptr) dropfloor[i]	->Render();
		if (coin[i]			!= nullptr) coin[i]			->Render(coin_img);
	}
}
void	cCharacterManager::Update() {
	player->Update();
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]		!= nullptr) jumpman[i]		->Update();
		if (hardbody[i]		!= nullptr) hardbody[i]		->Update();
		if (wireman[i]		!= nullptr) wireman[i]		->Update(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (fryingman[i]	!= nullptr) fryingman[i]	->Update();
		if (wireanchor[i]	!= nullptr) wireanchor[i]	->Update(&wmanager[i]->WirePos, &wmanager[i]->AnchorStretch, &wmanager[i]->EnemyAnchorStretch);
		if (gunman[i]		!= nullptr)	gunman[i]		->Update();
		if (bossmiddle[i]	!= nullptr) bossmiddle[i]	->Update();
		if (circularsaw[i]	!= nullptr) circularsaw[i]	->Update(5.f, 0, 1);
		if (cannon[i]		!= nullptr) cannon[i]		->Update();
		if (movefloor[i]	!= nullptr) movefloor[i]	->Update(3.f, 0, 1);
		if (dropfloor[i]	!= nullptr) dropfloor[i]	->Update();
		if (coin[i]			!= nullptr) coin[i]			->Update();
	}
	DeleteDeathCharacters();
	if (mp > 300) mp = 300;
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

		jumpman[i]		= nullptr;
		hardbody[i]		= nullptr;
		wireman[i]		= nullptr;
		fryingman[i]	= nullptr;
		gunman[i]		= nullptr;
		bossmiddle[i]	= nullptr;
		dropfloor[i]	= nullptr;
		movefloor[i]	= nullptr;

	}
}


void	cCharacterManager::DeleteDeathCharacters() {
	if (player->GetHp() <= 0) {
		IsOverFlag = true;
	}
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i] != nullptr) {
			if (jumpman[i]->GetHp() <= 0) {
				delete jumpman[i];
				jumpman[i] = nullptr;
			}
		}
		if (hardbody[i] != nullptr) {
			if (hardbody[i]->GetHp() <= 0) {
				delete hardbody[i];
				hardbody[i] = nullptr;
			}
		}
		if (fryingman[i] != nullptr) {
			if (fryingman[i]->GetHp() <= 0) {
				delete fryingman[i];
				fryingman[i] = nullptr;
			}
		}
		if (gunman[i] != nullptr) {
			if (gunman[i]->GetHp() <= 0) {
				delete gunman[i];
				gunman[i] = nullptr;
			}
		}
		if (bossmiddle[i] != nullptr) {
			if (bossmiddle[i]->GetHp() <= 0) {
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
		/*case eWireman:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (wireman[i] == nullptr) {　ここにアンカーの処理かきたいけど無理かも
					wireman[i] = new cEnemyWiremanManager::Wireman(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;*/
		case eBossmiddle:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (bossmiddle[i] == nullptr) {
					bossmiddle[i] = new cEnemyBossmiddle(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
					break;
				}
			}
			break;
		case eBoss:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (boss[i] == nullptr) {
					boss[i] = new cEnemyBoss(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)) == 1 ? true : false);
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
		MoveByAutomation();	// その他は自動
		attack_flag = false;
	}
	if (image_change == 89)
		hp = 0;
//	if (hp == 0)
	//	possess = false;
	Physical();
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
		if (landing == false) {
			if (image_change > 28)
				image_change = 0;
			if (image_change != 28)
				image_change++;
		}
		if (landing == true)
			image_change = 0;
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


void cEnemyGunman::Update()
{

	MoveByAutomation();	// その他は自動
	player_move_pattern = 0;

	if (image_change == 179)
		hp = 0;
	Physical();
}

void cEnemyGunman::Render(int image[])
{
	if (hp > 1) {
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
	MoveByAutomation();
	Physical();
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
	MoveByAutomation(WirePos, AnchorStretch, EnemyAnchorStretch);
	*AnchorStretch = 0;
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
	MoveByAutomation();
}

void cEnemyFryingman::MoveByAutomation()
{
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
				pos.x < FocusPos.x ? direction = false : direction = true;
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
			// 敵の向きと移動の処理
			if (pos.x < FocusPos.x && image_change == 187) {
				direction = false;
			}
			else if (pos.x >= FocusPos.x && image_change == 187) {
				direction = true;
			}
			//pos.x < FocusPos.x & image_change == 189 ? direction = false : direction = true;
			direction ? pos.x -= move_speed : pos.x += move_speed;
			
			image_change++;

			if (image_change == 189) {
				image_change = 160;
				move_time++;
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
				pos.x < FocusPos.x ? direction = false : direction = true;
			}
			else if (image_change != 24) {
				image_change++;
				direction ? pos.x -= 10 : pos.x += 10;
			}
			else if (landing) {
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
	if (hp == 1);
} 

void cEnemyBossmiddle::Render(int image[])
{
	if (hp == 1) {
		if (image_change < 100 || image_change > 149)
			image_change = 100;
		if (image_change != 149)
			image_change++;
	}

	if (direction == true)
		DrawGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], TRUE);
	else if (direction == false)
		DrawTurnGraph(pos.x - 300 / 2, pos.y - 300 / 2, image[image_change], TRUE);
}

/*------------------------------------------------------------------------------*
| <<< cEnemyBoss >>>
*------------------------------------------------------------------------------*/

void cEnemyBoss::Update()
{
	if (hp > 1) {
		MoveByAutomation();
	}
	if (image_change == 149) {
		hp = 0;
	}
	Physical();

}

void cEnemyBoss::MoveByAutomation()
{
	switch (enemy_move)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

void cEnemyBoss::Render(int image[])
{
	switch (enemy_move)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
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
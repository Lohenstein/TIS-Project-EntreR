
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
	case Spring:
		jump = 40.f;
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
	DrawFormatString(pos.x, pos.y, 0xFFFFFF, "%f, %d", wrad, anchor_dir);
}

void	cPlayer::UpdateAnchor() {
	if (anchor != nullptr) {
		// 飛んでるとき
		if (anchor->GetFlag()) {
			anchor->Update();
			if (sqrtf((anchor->GetPos().x - pos.x) * (anchor->GetPos().x - pos.x) + (anchor->GetPos().y - pos.y) * (anchor->GetPos().y - pos.y)) >= 600.f) {
				delete anchor;
				anchor = nullptr;
				IsAnchored = false;
			}
		}
		else {
			// くっついたとき
			if (!IsAnchored) {
				savepos = pos;
				rad2anchor = atan2f(pos.y - anchor->GetPos().y, pos.x - anchor->GetPos().x) - (DX_PI_F / 2.f);
				dis2anchor = sqrtf((anchor->GetPos().x - pos.x) * (anchor->GetPos().x - pos.x) + (anchor->GetPos().y - pos.y) * (anchor->GetPos().y - pos.y));
				IsAnchored = true;
				swing = rad2anchor;
				jump_count = 0;
				wrad_old = 0;
				wrad = cos(rad2anchor);// * (rad2anchor / (DX_PI_F / 2.f));
				if (wrad > wrad_old) anchor_dir = 1;
				if (wrad < wrad_old) anchor_dir = -1;
			}
			else {
				// ぶら下がっているとき
				if (trigger_r > 55) dis2anchor -= trigger_r / 32.f;
				if (trigger_l > 55) dis2anchor += trigger_l / 32.f;

				wrad_old = wrad;
				wrad = cos(swing) * (rad2anchor / (DX_PI_F / 2.f));

				if (wrad > wrad_old) anchor_dir = 1;
				if (wrad < wrad_old) anchor_dir = -1;

				pos.x = anchor->GetPos().x + cos(wrad + DX_PI_F / 2.f) * dis2anchor;
				pos.y = anchor->GetPos().y + sin(wrad + DX_PI_F / 2.f) * dis2anchor;

				swing += DX_PI_F / 45.f;
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
			if (IsAnchored) {
				if (anchor_dir ==  1) rad2anchor += DX_PI_F / 360.f;
				//if (anchor_dir == -1) rad2anchor -= DX_PI_F / 360.f;
			}
		}
		if (key[KEY_INPUT_RIGHT] == 2 || stick_lx >= 100) {
			rect = false;
			inertia += 4;				// 移動量θを増加
			if (IsAnchored) {
				//if (anchor_dir ==  1) rad2anchor -= DX_PI_F / 360.f;
				if (anchor_dir == -1) rad2anchor += DX_PI_F / 360.f;
			}
		}
	}
	else {
		// キー押し下げ時以外は収束する
		if (inertia > 0) inertia -= 2;
		if (inertia < 0) inertia += 2;
		if (IsAnchored) {
			if (rad2anchor > 0.f) rad2anchor -= DX_PI_F / 1440.f;
		}
	}

	if ((key[KEY_INPUT_SPACE] == 1 || pad_b[XINPUT_BUTTON_A] == 1) && jump_count < 2) {
		jump = 20.f;
		++jump_count;
		if (anchor != nullptr) {
			delete anchor;
			IsAnchored = false;
			anchor = nullptr;
		}
	}
	if ((key[KEY_INPUT_C] == 1 || pad_b[XINPUT_BUTTON_X] == 1) && mp >= 10) {
		mp -= 10;
		bullet.Shot(pos, { 3.f, 3.f, 0.f }, 20.f, PI * rect, PlayerBullet);
	}
	if (pad_b[XINPUT_BUTTON_Y] == 1) {
		delete anchor;
		IsAnchored = false;
		IsFall = false;
		anchor = nullptr;
		anchor = new cAnchor(pos, { 10.f, 10.f, 10.f }, 30.f, -stick_rad, WireAnchor);
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
		if (fryingman[i]	!= nullptr) fryingman[i]	->Render(fryingman_img);
		if (gunman[i]		!= nullptr)	gunman[i]		->Render(gunman_img,gunman_imgdead);
		if (bossmiddle[i]	!= nullptr) bossmiddle[i]	->Render(bossmiddle_img);
		if (circularsaw[i]	!= nullptr) circularsaw[i]	->Render(circularsaw_img);
		if (cannon[i]		!= nullptr) cannon[i]		->Render(cannon_img);
		if (movefloor[i]	!= nullptr) movefloor[i]	->Render();
		if (dropfloor[i]	!= nullptr) dropfloor[i]	->Render();
		if (coin[i]			!= nullptr) coin[i]			->Render(coin_img);
		if (spring[i]		!= nullptr) spring[i]		->Render(spring_img);
	}
}
void	cCharacterManager::Update() {
	player->Update();
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (jumpman[i]		!= nullptr) jumpman[i]		->Update();
		if (hardbody[i]		!= nullptr) hardbody[i]		->Update();
		if (fryingman[i]	!= nullptr) fryingman[i]	->Update();
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
		delete fryingman[i];
		delete gunman[i];
		delete bossmiddle[i];
		delete dropfloor[i];
		delete movefloor[i];

		jumpman[i]		= nullptr;
		hardbody[i]		= nullptr;
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
		case eSpring:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (spring[i] == nullptr) {
					spring[i] = new cSpring(stoi(str.at(1)), stoi(str.at(2)));
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
			}
			break;
		case 1:
			attack_count++;
			if (attack_count == 1) {
				pos.x < FocusPos.x ? direction = false : direction = true;
				
			}
			if (attack_flag) {
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
				direction ? bullet.Shot(bulletpos, bulletsize, 10, lockon, EnemyBullet) : bullet.Shot(bulletpos, bulletsize, -10, PI, EnemyBullet);
			}
			if (attack_count == 100) {
				attack_count = 0;
				move_pattern = 2;
			}
			break;
		case 2:
			attack_count++;
			if (attack_count == 100) {
				move_pattern = 0;
				attack_count = 0;
			}
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

void cEnemyGunman::Render(int image[],int imagedead[])
{
	if (hp > 1) {
		switch (move_pattern) {
		case 0:
			image_change = 0;
			break;
		case 1:
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
	if (hp != 1) {
		if (direction == true) {
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, FALSE);
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, lockon, image[37], TRUE, FALSE);
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, lockon, image[74], TRUE, FALSE);
		}
		else if (direction == false) {
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, image[image_change], TRUE, TRUE);
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, lockon, image[37], TRUE, TRUE);
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, lockon, image[74], TRUE, TRUE);
		}
	}
	else if (hp == 1) {
		if (direction == true) {
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, imagedead[image_change], TRUE, FALSE);
		}
		else if (direction == false) {
			DrawRotaGraph(pos.x, pos.y - 15, 0.7, 0, imagedead[image_change], TRUE, TRUE);
		}
	}
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
	pos.x -= speed / 2;
	switch (move_flow) {
		// 移動　攻撃
		
	case 0:

		if (cooltime <= 100) cooltime++;
		
		if (cooltime > 100) {
			bulletpos = pos;
			lockon = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x);
			bullet.Shot(bulletpos, bulletsize, 10, lockon, EnemyBullet);
			cooltime = 0;
		}
		// 左から近づくとき
		if (FocusPos.x < pos.x && FocusPos.x + 100 > pos.x) {
			lockon = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x);
			move_flow = 1;
			image_change = 41;
		}
		break;
	case 1:
		image_change != 61 ? image_change++ : image_change = 61;
		pos.y -= cos(lockon) * speed;
		if (landing == true || bottomhit == true || lefthit == true || righthit == true) {
			hp = 0;
		}
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
| <<< cEnemyJugem >>>
*------------------------------------------------------------------------------*/

void cEnemyJugem::Update()
{
	if (hp > 1) {
		MoveByAutomation();
	}
	/*if (image_change == 149) {
		hp = 0;
	}*/
	if (hp == 1)move_pattern = 4;
}

void cEnemyJugem::MoveByAutomation()
{
	switch (move_pattern) {
		// 移動
	case 0:
		if (count = 500) {

			move_pattern = 1;
		}
		break;
		// 攻撃
	case 1:
		image_change++;
		if (count == 100) {

			move_pattern = 2;
		}
		break;
		// 強攻撃
	case 2:
		image_change++;
		if (count == 200) {

			move_pattern = 0;
		}
		break;
		// 要らんかも
	case 3:
		break;
	case 4:
		// 倒れた時の処理
		image_change++;
		if (image_change) {
			hp = 0;
		}
		break;
	}
}

void cEnemyJugem::Render(int img[])
{
	direction ? DrawTurnGraph(pos.x - 300 / 2, pos.y - 300 / 2, img[image_change], TRUE): DrawTurnGraph(pos.x - 300 / 2, pos.y - 300 / 2, img[image_change],FALSE);
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
	// レーザー
	case 0:
		if (attack_count == 20) {

			enemy_move = 1;
		}
		break;
	// 銃
	case 1:
		if (attack_count == 10) {

		}
		enemy_move = 2;
		break;
	// 掴みかかる
	case 2:
		if (attack_count == 50) {
			lockon = atan2(FocusPos.y - pos.y, FocusPos.x - pos.x);
			for (int i = 0; i < 4; i++) {
				attackpos[i] = {pos.x + (cos(lockon) * speed * i),pos.y + (sin(lockon) * speed * i),0};
			}
			enemy_move = 3;
		}
		break;
	// 色々
	case 3:
		if (attack_count == 10) {

			enemy_move = 4;
		}
		break;
	// ダウン
	case 4:
		// 休憩タイム
		if (attack_count == 20) {

			attack_count = 0;
			enemy_move = 0;
		}
		break;
	}
}

void cEnemyBoss::Render(int image[])
{
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

/*------------------------------------------------------------------------------*
| <<< cEventsSwitch >>>
*------------------------------------------------------------------------------*/

void cEventsSwitch::Update()
{
	Physical();

	if (hp == 0) hp= 1;
}

void cEventsSwitch::MoveByAutomation()
{

}

void cEventsSwitch::Render(int image[])
{

}

void	cSpring::HitAction(cObject *hit) {
	if (hit->GetType() == Player) {
		flag = true;
	}
}

void	cSpring::Update() {
	if (flag) {
		num++;
		if (num >= 30) {
			num = 0;
			flag = false;
		}
	}
}

void	cSpring::Render(int image[30]) {
	DrawGraph(pos.x - size.x / 2.f, pos.y - size.y / 2.f, image[num], true);
}


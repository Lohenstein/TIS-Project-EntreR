#include "Main.h"

std::unique_ptr<cGame> scene;
std::unique_ptr<cTitle> title;

MenuMode	menumode;

void	LoadStage(string str, bool reload);

/*------------------------------------------------------------------------------*
| <<< ゲーム >>>
*------------------------------------------------------------------------------*/
void	cGame::Init() {

}

void	cGame::CollisionAroundMaptile(cObject *hit) {

	VECTOR pos = hit->GetPos();

	int csize = 20;
	int x = (int)pos.x / (int)bsize, y = (int)pos.y / (int)bsize;
	int startx = x - csize / 2, starty = y - csize / 2;
	int endx = x + csize / 2, endy = y + csize / 2;

	if (startx < 0) startx = 0;
	if (endx > stage->GetStageSizeX()) endx = stage->GetStageSizeX();
	if (starty < 0) starty = 0;
	if (endy > stage->GetStageSizeY()) endy = stage->GetStageSizeY();

	for (int i = startx; i < endx; i++) {
		for (int j = starty; j < endy; j++) {
			if (stage->GetTile(i, j) != -1) {
				CheckHitRectAndRect(hit, stage->GetMapTile(i, j));
			}
		}
	}
}

void	cGame::Collision() {

	CheckHitRectAndRect(character->GetPlayer(), character->GetClear());

	CollisionAroundMaptile(character->GetPlayer());
	if (character->GetAnchor() != nullptr) CollisionAroundMaptile(character->GetAnchor());
	for (int k = 0; k < ENEMY_MAX; k++) {
		if (character->GetEnemyJumpman(k)		!= nullptr) CollisionAroundMaptile(character->GetEnemyJumpman(k));
		if (character->GetEnemyHardBody(k)		!= nullptr) CollisionAroundMaptile(character->GetEnemyHardBody(k));
		if (character->GetEnemyGunman(k)		!= nullptr)	CollisionAroundMaptile(character->GetEnemyGunman(k));
		if (character->GetEnemyBossmiddle(k)	!= nullptr) CollisionAroundMaptile(character->GetEnemyBossmiddle(k));
		if (character->GetCannon(k)				!= nullptr) CollisionAroundMaptile(character->GetCannon(k));
		if (character->GetEnemyFryingman(k)		!= nullptr) CollisionAroundMaptile(character->GetEnemyFryingman(k));
		if (character->GetEnemyJugem(k)			!= nullptr) CollisionAroundMaptile(character->GetEnemyJugem(k));
		if (character->GetCrumbleWall(k)		!= nullptr) CollisionAroundMaptile(character->GetCrumbleWall(k));
		if (character->GetMoveWall(k)			!= nullptr) CollisionAroundMaptile(character->GetMoveWall(k));
	}
	// Wire
	for (int i = 0; i < 120; i++) {
		if (character->GetAnchorWire(i) != nullptr) {
			CollisionAroundMaptile(character->GetAnchorWire(i));
		}
	}
	for (int k = 0; k < BULLET_MAX; k++) {
		if (bullet.GetBullet(k) != nullptr) {
			CollisionAroundMaptile(bullet.GetBullet(k));
		}
	}
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (character->GetMoveFloor(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetMoveFloor(i));
			/*for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyWireman(j));
				if (character->GetEnemyWireAnchor(j) != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyWireAnchor(j));
				if (character->GetEnemyGunman(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j) != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyBossmiddle(j));
			}*/
		}
		if (character->GetDropFloor(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetDropFloor(i));
			/*for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyWireman(j));
				if (character->GetEnemyWireAnchor(j) != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyWireAnchor(j));
				if (character->GetEnemyGunman(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j) != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyBossmiddle(j));
			}*/
		}
		//---------------------------------------------------------------------------
		// 壊れるオブジェクトはすべての敵に当たり判定を付けています
		if (character->GetCrumbleWall(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetCrumbleWall(i));
		 	for (int j = 0; j < ENEMY_MAX; j++) {
			if (character->GetEnemyJumpman(j)	 != nullptr) CheckHitRectAndRect(character->GetCrumbleWall(i), character->GetEnemyJumpman(j));
			if (character->GetEnemyHardBody(j)	 != nullptr) CheckHitRectAndRect(character->GetCrumbleWall(i), character->GetEnemyHardBody(j));
			if (character->GetEnemyGunman(j)	 != nullptr) CheckHitRectAndRect(character->GetCrumbleWall(i), character->GetEnemyGunman(j));
			if (character->GetEnemyBossmiddle(j) != nullptr) CheckHitRectAndRect(character->GetCrumbleWall(i), character->GetEnemyBossmiddle(j));
			}
		}
		if (character->GetCoin(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetCoin(i));
		}
		if (character->GetSpring(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetSpring(i));
		}
		if (character->GetMoveWall(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetMoveWall(i));
		}
	}

	// 弾とキャラクタ
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet.GetBullet(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), bullet.GetBullet(i));
			if (character->GetEnemyBoss() != nullptr)	CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyBoss());
			for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)	!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j)	!= nullptr) CheckHitRectAndRollingRect(bullet.GetBullet(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyGunman(j)	!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j)!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyBossmiddle(j));
				if (character->GetEnemyFryingman(j) != nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyFryingman(j));
				if (character->GetEnemyJugem(j)		!= nullptr)	CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyJugem(j));
				if (character->GetCrumbleWall(j)	!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetCrumbleWall(j));
				if (character->GetMoveWall(j)		!= nullptr && character->GetSwitchHp(j) != 1) CheckHitRectAndRect(bullet.GetBullet(i), character->GetMoveWall(j));
			}
		}
	}
	// キャラクタ同士
	if (character->GetEnemyBoss() != nullptr)	CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyBoss());
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (character->GetEnemyJumpman(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyJumpman(i));
		if (character->GetEnemyHardBody(i)	!= nullptr) CheckHitRectAndRollingRect(character->GetPlayer(), character->GetEnemyHardBody(i));
		if (character->GetEnemyGunman(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyGunman(i));
		if (character->GetEnemyBossmiddle(i)!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyBossmiddle(i));
		if (character->GetCircularSaw(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetCircularSaw(i));
		if (character->GetEnemyFryingman(i) != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyFryingman(i));
		if (character->GetEnemyJugem(i)	    != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyJugem(i));
		if (character->GetCrumbleWall(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetCrumbleWall(i));
	}
}	

void	cGame::Update() {
	if (IsClearFlag) {
		UpdateResult();
	}
	else if (IsOverFlag) {
		UpdateOver();
	}
	else {
		// 時間UP処理
		if (character->GetAddSwitch() == true) {
			sec += 10;

			character->GetAddSwitchChange();
		}
		character->Update(GetTime());
		Collision();
		bullet.Update();
		camera->Update(FocusPos);
		gui->SetHp(character->GetPlayerHp());
		dialog->Update();
		UpdateGui();
		effect.Update();
	}
}

void	cGame::Render() {
	// 描画先ウィンドウを変更
	SetDrawScreen(bghandle);
	ClearDrawScreen();
	// 描画
	stage->RenderBG();
	character->Render();
	stage->Render();
	bullet.Render();
	// 戻す
	SetDrawScreen(DX_SCREEN_BACK);

	camera->Render(bghandle,stage->GetStageSizeX(),stage->GetStageSizeY());
	dialog->Render();
	gui->Render();
	RenderGui();

	// クリア時
	if (IsClearFlag) {
		DrawResult();
		trans++;
	}
	// ゲームオーバー時
	if (IsOverFlag) {
		DrawOver();
		trans += 2;
	}

	//DrawFormatString(10, 10, 0xFFFFFF, "操作キャラの座標:x=%d, y=%d", (int)FocusPos.x, (int)FocusPos.y);
}

void	cGame::DrawOver() {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans >= 255 ? 255 : trans);

	DrawGraph(0, 0, imghandle[3], false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (trans > 235 && trans % 30 != 0) {
		int w =  GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "R key: Retry / X key : Lab", time);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 550, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "R key: Retry / X key : Lab", time);
	}
}

void	cGame::UpdateOver() {
	if ((key[KEY_INPUT_R] == 1 || pad_b[XINPUT_BUTTON_START] == 1) && trans > 235) {
		LoadStage("this is reload", true);
	}
	if (pad_b[XINPUT_BUTTON_A] == 1 && trans > 235) {
		title.reset(new cTitle);
		gamemode = Game::mode_title;
		scene.reset();
	}
}

void	cGame::UpdateResult() {
	if ((key[KEY_INPUT_R] == 1 || pad_b[XINPUT_BUTTON_START] == 1) && trans > 335) {
		LoadStage("this is reload", true);
	}
	if (pad_b[XINPUT_BUTTON_A] == 1 && trans > 235) {
		title.reset(new cTitle);
		gamemode = Game::mode_title;
		scene.reset();
	}
}

void	cGame::DrawResult() {
	// 薄暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans >= 255 ? 255 : trans);

	int w;
	DrawGraph(0, 0, imghandle[4], false);
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (trans > 255) {
		w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "Score:%dsec", time);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 300, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "Score:%dsec", time);
	}
	if (trans > 275) {
		w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "Time:%dsec", rectime);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 350, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "Time:%dsec", time);
	}
	if (trans > 295) {
		w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "Coin:%dsec", time);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 400, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "Coin:%dsec", time);
	}
	if (trans > 315) {
		w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "RareCoin:%d/5", time);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 450, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "RareCoin:%d/5", time);
	}
	if (trans > 335 && trans % 30 != 0) {
		w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "R key: Retry / X key : Lab", time);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 550, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "R key: Retry / X key : Lab", time);
	}
}

void	cGame::UpdateGui() {
	time++;
}

void	cGame::RenderGui() {
	if (time >= 60) {
		time = 0;
		sec--;
		mp += 5;
		if (sec < 0) {
			mp += 100;
			rectime++;
			sec = 59;
			min--;
		}
	}
	if (sec >= 60) {
		sec -= 60;
		min += 1;
	}
	if (sec < 10) {
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_TIME], "0%d:0%d", min, sec);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 20, 0xFFFFFF, font_handle[FONT_TIME], "0%d:0%d", min, sec);
	}
	else {
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_TIME], "0%d:%d", min, sec);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 20, 0xFFFFFF, font_handle[FONT_TIME], "0%d:%d", min, sec);
	}

	DrawGraph(10, 70, imghandle[2], true);
	if (mp >= 300) {
		if (time % 3 != 0) DrawRectGraph(10, 70, 0, 0, mp, 16, imghandle[1], false, true);
	}
	else {
		DrawRectGraph(10, 70, 0, 0, mp, 16, imghandle[1], false, true);
	}
	if (min <= 0 && sec <= 0) IsOverFlag = true;
}

void	cGame::DrawPauseMenu() {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans > 155 ? 155 : trans);
	DrawBox(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// もっかいスタートボタン押したらポーズ解除
	if (pad_b[XINPUT_BUTTON_START] == 1 && trans != 0) IsPaused = false;
	trans += 2;

	// 各メニューへ移動
	switch (pause.draw(550, 280, 3, pause_str)) {
	case PAUSE_CONTINUE:
		IsPaused = false;
		trans    = 0;
		break;
	case PAUSE_RESTART:
		LoadStage("this is reload", true);
		trans = 0;
		break;
	case PAUSE_BACK2TITLE:

		trans = 0;
		// タイトルに戻る
		title.reset(new cTitle);
		gamemode = Game::mode_title;
		scene.reset();

		menumode = MENUMODE_MAINMENU;
		IsPaused = false;
		break;
	default:
		break;
	}
}
/*------------------------------------------------------------------------------*
| <<< メニュー >>>
*------------------------------------------------------------------------------*/
void	cTitle::Init() {

}
void	cTitle::Update() {
	//SetDrawScreen(DX_SCREEN_BACK);
}
void	cTitle::Render() {
	DrawGraph(0, 0, titlebg, false);
	switch (menumode) {
	case MENUMODE_MAINMENU:
		DrawTitle();
		break;
	case MENUMODE_STAGESELECTMENU:
		DrawStageSelect();
		break;
	case MENUMODE_OPTIONMENU:
		DrawOption();
		break;
	}
}
/*------------------------------------------------------------------------------*
| <<< タイトルメニュー >>>
*------------------------------------------------------------------------------*/
void	cTitle::DrawTitle() {

	// 各メニューへ移動
	switch (menu.draw(550, 280, 4, title_str)) {
		// GAME START
	case TITLE_GAMESTART:
		break;
		// STAGE EDITOR
	case TITLE_STAGESELECT:
		menumode = MENUMODE_STAGESELECTMENU;
		break;
		// OPTION
	case TITLE_OPTION:
		menumode = MENUMODE_OPTIONMENU;
		break;
		// QUIT GAME
	case TITLE_QUITGAME:
		IsQuit = true;
		break;
	default:
		break;
	}
}

/*------------------------------------------------------------------------------*
| <<< おぷちょんメニュー >>>
*------------------------------------------------------------------------------*/
void	cTitle::DrawOption() {
	switch (optionselect.draw(550, 280, 2, option_str)) {
	case OPTION_FULLSCREEN:
		if (IsWindowed) {

			int DesktopSizeX;	// デスクトップの大きさ
			int DesktopSizeY;

			GetDefaultState(&DesktopSizeX, &DesktopSizeY, NULL);
			SetWindowStyleMode(4);						// ウインドウスタイルを枠なしに変更
			SetWindowSize(DesktopSizeX, DesktopSizeY);	// ウインドウのサイズをデスクトップと同じにする
			SetWindowPosition(0, 0);					// ウインドウの位置を画面左上に変更
			SetWindowZOrder(DX_WIN_ZTYPE_TOPMOST, TRUE);
		}
		else {
			SetWindowStyleMode(0);						// ウインドウスタイルを標準に変更
			SetWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);// ウインドウサイズを 1280x720 に変更
			SetWindowZOrder(DX_WIN_ZTYPE_NORMAL, FALSE);
		}
		IsWindowed = !IsWindowed;
		break;
	case OPTION_OPTIONBACK:
		menumode = MENUMODE_MAINMENU;
		break;
	}
}

/*------------------------------------------------------------------------------*
| <<< ステージセレクトメニュー >>>
*------------------------------------------------------------------------------*/
void	cTitle::DrawStageSelect() {
	int w;
	// 各メニューへ移動
	switch (stageselect.draw(550, 280, 4, stage_str)) {
		// GAME START
	case 0:
		LoadStage("data/map/stage1/", false);
		break;
		// STAGE EDITOR
	case 1:
		LoadStage("data/map/stage2/", false);
		break;
		// QUIT GAME
	case 2:
		LoadStage("data/map/stage3/", false);
		break;
	case 3:
		menumode = MENUMODE_MAINMENU;
		break;
	}
}

void	LoadStage(string str, bool reload) {

	// リロードならファイルパスは同じなので変えない
	if (!reload) stagepath = str; // ステージのファイルパス

	// ステージ初期化
	scene.reset();

	SetUseASyncLoadFlag(true);

	scene.reset(new cGame);

	SetUseASyncLoadFlag(false);

	while (GetASyncLoadNum() != 0 && !ScreenFlip() && !ProcessMessage() && !ClearDrawScreen()) {

		while (GetNowCount() - FrameStartTime < 1000 / 60) {}
		FrameStartTime = GetNowCount();

		DrawBox(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0x000000, true);
		// Now Loading描画
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "Now Loading...");
		DrawFormatStringToHandle(WINDOW_SIZE_X - (w + 40), 660, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "Now Loading... %d", GetASyncLoadNum());
	}

	// リロードの場合はタイトルの初期化をしない
	if (!reload) title.reset();

	// ゲームモードをゲームにする
	gamemode = Game::mode_game;
}
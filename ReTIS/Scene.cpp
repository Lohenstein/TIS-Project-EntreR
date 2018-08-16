#include "Main.h"

std::unique_ptr<cBase> scene;

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
	for (int k = 0; k < ENEMY_MAX; k++) {
		if (character->GetEnemyJumpman(k)		!= nullptr) CollisionAroundMaptile(character->GetEnemyJumpman(k));
		if (character->GetEnemyHardBody(k)		!= nullptr) CollisionAroundMaptile(character->GetEnemyHardBody(k));
		if (character->GetEnemyWireman(k)		!= nullptr) CollisionAroundMaptile(character->GetEnemyWireman(k));
		if (character->GetEnemyWireAnchor(k)	!= nullptr) CollisionAroundMaptile(character->WireAnchor(k));
		if (character->GetEnemyGunman(k)		!= nullptr)	CollisionAroundMaptile(character->GetEnemyGunman(k));
		if (character->GetEnemyBossmiddle(k)	!= nullptr) CollisionAroundMaptile(character->GetEnemyBossmiddle(k));
		if (character->GetCannon(k)				!= nullptr) CollisionAroundMaptile(character->GetCannon(k));
	}
	for (int k = 0; k < BULLET_MAX; k++) {
		if (bullet.GetBullet(k) != nullptr) {
			CollisionAroundMaptile(bullet.GetBullet(k));
		}
	}
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (character->GetCoin(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetCoin(i));
			for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)	 != nullptr) CheckHitRectAndRect(character->GetCoin(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j)	 != nullptr) CheckHitRectAndRect(character->GetCoin(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j)	 != nullptr) CheckHitRectAndRect(character->GetCoin(i), character->GetEnemyWireman(j));
				if (character->GetEnemyWireAnchor(j) != nullptr) CheckHitRectAndRect(character->GetCoin(i), character->GetEnemyWireAnchor(j));
				if (character->GetEnemyGunman(j)	 != nullptr) CheckHitRectAndRect(character->GetCoin(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j) != nullptr) CheckHitRectAndRect(character->GetCoin(i), character->GetEnemyBossmiddle(j));
			}
		}
	}
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (character->GetMoveFloor(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetMoveFloor(i));
			for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyWireman(j));
				if (character->GetEnemyWireAnchor(j) != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyWireAnchor(j));
				if (character->GetEnemyGunman(j)	 != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j) != nullptr) CheckHitRectAndRect(character->GetMoveFloor(i), character->GetEnemyBossmiddle(j));
			}
		}
		if (character->GetDropFloor(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), character->GetDropFloor(i));
			for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyWireman(j));
				if (character->GetEnemyWireAnchor(j) != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyWireAnchor(j));
				if (character->GetEnemyGunman(j)	 != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j) != nullptr) CheckHitRectAndRect(character->GetDropFloor(i), character->GetEnemyBossmiddle(j));
			}
		}
	}

	// 弾とキャラクタ
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet.GetBullet(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), bullet.GetBullet(i));
			for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)	!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j)	!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j)	!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyWireman(j));
				if (character->GetEnemyGunman(j)	!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j)!= nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyBossmiddle(j));
			}
		}
	}
	// キャラクタ同士
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (character->GetEnemyJumpman(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyJumpman(i));
		if (character->GetEnemyHardBody(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyHardBody(i));
		if (character->GetEnemyWireman(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyWireman(i));
		if (character->GetEnemyGunman(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyGunman(i));
		if (character->GetEnemyBossmiddle(i)!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyBossmiddle(i));
		if (character->GetCircularSaw(i)	!= nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetCircularSaw(i));
	}
}

void	cGame::Update() {
	input();
	if (IsClearFlag) {
		UpdateResult();
	}
	else if (IsOverFlag) {
		UpdateOver();
	}
	else {
		character->Update();
		Collision();
		bullet.Update();
		camera->Update(FocusPos);
		gui->SetHp(character->GetPlayerHp());
		UpdateGui();
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

	camera->Render(bghandle);
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

	DrawGraph(0, 0, imghandle[0], false);
	int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_TIME], "Game Over!!");
	DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 150, 0xFFFFFF, font_handle[FONT_TIME], "Game Over!!");

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (trans > 235 && trans % 30 != 0) {
		w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "R key: Retry / X key : Lab", time);
		DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 550, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "R key: Retry / X key : Lab", time);
	}
}

void	cGame::UpdateOver() {
	if (key[KEY_INPUT_R] == 1 && trans > 235) {
		// なうろ
		DrawGraph(0, 0, imghandle[0], false);
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "Now Loading...");
		DrawFormatStringToHandle(WINDOW_SIZE_X - (w + 20), 660, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "Now Loading...");
		ScreenFlip();
		// 初期化
		scene.reset(new cGame);
	}
}

void	cGame::UpdateResult() {
	if (key[KEY_INPUT_R] == 1 && trans > 335) {
		// なうろ
		DrawGraph(0, 0, imghandle[0], false);
		int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "Now Loading...");
		DrawFormatStringToHandle(WINDOW_SIZE_X - (w + 20), 660, 0xFFFFFF, font_handle[FONT_POSSESSTIME], "Now Loading...");
		ScreenFlip();
		// 初期化
		scene.reset(new cGame);
	}
}

void	cGame::DrawResult() {
	// 薄暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, trans >= 255 ? 255 : trans);

	DrawGraph(0, 0, imghandle[0], false);
	int w = GetDrawFormatStringWidthToHandle(font_handle[FONT_TIME], "Stage Clear!!");
	DrawFormatStringToHandle(WINDOW_SIZE_X / 2 - w / 2, 150, 0xFFFFFF, font_handle[FONT_TIME], "Stage Clear!!");

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
		mp += 20;
		if (sec < 0) {
			mp += 100;
			rectime++;
			sec = 59;
			min--;
			if (min <= 0 && sec <= 0) IsOverFlag = true;
		}
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
}
#include "Main.h"

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
		if (character->GetEnemyJumpman(k) != nullptr) CollisionAroundMaptile(character->GetEnemyJumpman(k));
		if (character->GetEnemyHardBody(k) != nullptr) CollisionAroundMaptile(character->GetEnemyHardBody(k));
		if (character->GetEnemyWireman(k) != nullptr) CollisionAroundMaptile(character->GetEnemyWireman(k));
		if (character->GetEnemyWireAnchor(k) != nullptr) CollisionAroundMaptile(character->WireAnchor(k));
		if (character->GetEnemyGunman(k) != nullptr)	CollisionAroundMaptile(character->GetEnemyGunman(k));
		if (character->GetEnemyBossmiddle(k) != nullptr) CollisionAroundMaptile(character->GetEnemyBossmiddle(k));
		if (character->GetCannon(k) != nullptr)CollisionAroundMaptile(character->GetCannon(k));
	}
	for (int k = 0; k < BULLET_MAX; k++) {
		if (bullet.GetBullet(k) != nullptr) {
			CollisionAroundMaptile(bullet.GetBullet(k));
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
				if (character->GetEnemyJumpman(j) != nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j) != nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j) != nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyWireman(j));
				if (character->GetEnemyGunman(j) != nullptr)CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyGunman(j));
				if (character->GetEnemyBossmiddle(j) != nullptr)CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyBossmiddle(j));
			}
		}
	}
	// キャラクタ同士
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (character->GetEnemyJumpman(i) != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyJumpman(i));
		if (character->GetEnemyHardBody(i) != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyHardBody(i));
		if (character->GetEnemyWireman(i) != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyWireman(i));
		if (character->GetEnemyGunman(i) != nullptr)CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyGunman(i));
		if (character->GetEnemyBossmiddle(i) != nullptr)CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyBossmiddle(i));
		if (character->GetCircularSaw(i) != nullptr)CheckHitRectAndRect(character->GetPlayer(), character->GetCircularSaw(i));
	}
}

void	cGame::Update() {
	input();
	if (IsClearFlag) {

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
	stage->Render();
	character->Render();
	bullet.Render();
	// 戻す
	SetDrawScreen(DX_SCREEN_BACK);

	camera->Render(bghandle);
	gui->Render();
	RenderGui();

	if (IsClearFlag) {
		DrawResult();
		trans++;
	}

	//DrawFormatString(10, 10, 0xFFFFFF, "操作キャラの座標:x=%d, y=%d", (int)FocusPos.x, (int)FocusPos.y);
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
		w = GetDrawFormatStringWidthToHandle(font_handle[FONT_POSSESSTIME], "Time:%dsec", time);
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
		if (sec < 0) {
			sec = 59;
			min--;
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
}
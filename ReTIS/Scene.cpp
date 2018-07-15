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
			if (stage->GetTile(i, j) != 0) {
				CheckHitRectAndRect(hit, stage->GetMapTile(i, j));
			}
		}
	}
}
void	cGame::Collision() {
	/*/ マップチップとキャラクタ
	for (int i = 0; i < stage->GetStageSizeX(); i++) {
		for (int j = 0; j < stage->GetStageSizeY(); j++) {
			if (i * bsize > FocusPos.x - WINDOW_SIZE_X / 2 && i * bsize < FocusPos.x + WINDOW_SIZE_X / 2 &&
				j * bsize > FocusPos.y - WINDOW_SIZE_Y / 2 && j * bsize < FocusPos.y + WINDOW_SIZE_Y / 2) {
				if (stage->GetTile(i, j) != 0) {
					CheckHitRectAndRect(character->GetPlayer(), stage->GetMapTile(i, j));
					for (int k = 0; k < ENEMY_MAX; k++) {
						if (character->GetEnemyJumpman(k)  != nullptr) CheckHitRectAndRect(character->GetEnemyJumpman(k), stage->GetMapTile(i, j));
						if (character->GetEnemyHardBody(k) != nullptr) CheckHitRectAndRect(character->GetEnemyHardBody(k), stage->GetMapTile(i, j));
						if (character->GetEnemyWireman(k)  != nullptr) CheckHitRectAndRect(character->GetEnemyWireman(k), stage->GetMapTile(i, j));
					}
					for (int k = 0; k < BULLET_MAX; k++) {
						if (bullet.GetBullet(k) != nullptr) {
							CheckHitRectAndRect(stage->GetMapTile(i, j), bullet.GetBullet(k));
						}
					}
				}
			}
		}
	}*/
	
	CollisionAroundMaptile(character->GetPlayer());
	for (int k = 0; k < ENEMY_MAX; k++) {
		if (character->GetEnemyJumpman(k)  != nullptr) CollisionAroundMaptile(character->GetEnemyJumpman(k));
		if (character->GetEnemyHardBody(k) != nullptr) CollisionAroundMaptile(character->GetEnemyHardBody(k));
		if (character->GetEnemyWireman(k)  != nullptr) CollisionAroundMaptile(character->GetEnemyWireman(k));
		if (character->GetEnemyWireAnchor(k) != nullptr) CollisionAroundMaptile(character->WireAnchor(k));
	}
	for (int k = 0; k < BULLET_MAX; k++) {
		if (bullet.GetBullet(k) != nullptr) {
			CollisionAroundMaptile(bullet.GetBullet(k));
		}
	}


	// 弾とキャラクタ
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet.GetBullet(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), bullet.GetBullet(i));
			for (int j = 0; j < ENEMY_MAX; j++) {
				if (character->GetEnemyJumpman(j)  != nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyJumpman(j));
				if (character->GetEnemyHardBody(j) != nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyHardBody(j));
				if (character->GetEnemyWireman(j)  != nullptr) CheckHitRectAndRect(bullet.GetBullet(i), character->GetEnemyWireman(j));
			}
		}
	}
	// キャラクタ同士
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (character->GetEnemyJumpman(i)  != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyJumpman(i));
		if (character->GetEnemyHardBody(i) != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyHardBody(i));
		if (character->GetEnemyWireman(i)  != nullptr) CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyWireman(i));
	}
}

void	cGame::Update() {
	input();
	character->Update();
	Collision();
	bullet.Update();
	camera->Update(FocusPos);
	gui->SetHp(character->GetPlayerHp());
	UpdateGui();
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

	//DrawFormatString(10, 10, 0xFFFFFF, "操作キャラの座標:x=%d, y=%d", (int)FocusPos.x, (int)FocusPos.y);
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
#include "Main.h"

/*------------------------------------------------------------------------------*
| <<< ゲーム >>>
*------------------------------------------------------------------------------*/
void	cGame::Init() {

}

void	cGame::Collision() {
	// マップチップとキャラクタ
	for (int i = 0; i < stage->GetStageSizeX(); i++) {
		for (int j = 0; j < stage->GetStageSizeY(); j++) {
			if (stage->GetTile(i, j) != 0) {
				CheckHitRectAndRect(character->GetPlayer(),        stage->GetMapTile(i, j));
				for (int k = 0; k < ENEMY_MAX; k++) {
					if (character->GetEnemyJumpman(k)  != nullptr) CheckHitRectAndRect(character->GetEnemyJumpman(k),  stage->GetMapTile(i, j));
					if (character->GetEnemyHardBody(k) != nullptr) CheckHitRectAndRect(character->GetEnemyHardBody(k), stage->GetMapTile(i, j));
					if (character->GetEnemyWireman(k)  != nullptr) CheckHitRectAndRect(character->GetEnemyWireman(k),  stage->GetMapTile(i, j));
				}
				for (int k = 0; k < BULLET_MAX; k++) {
					if (bullet.GetBullet(k) != nullptr) {
						CheckHitRectAndRect(stage->GetMapTile(i, j), bullet.GetBullet(k));
					}
				}
			}
		}
	}
	// 弾とキャラクタ
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet.GetBullet(i) != nullptr) {
			CheckHitRectAndRect(character->GetPlayer(), bullet.GetBullet(i));
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

	//DrawFormatString(10, 10, 0xFFFFFF, "操作キャラの座標:x=%d, y=%d", (int)FocusPos.x, (int)FocusPos.y);
}

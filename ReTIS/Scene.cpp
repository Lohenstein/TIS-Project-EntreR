#include "Main.h"

/*------------------------------------------------------------------------------*
| <<< ゲーム >>>
*------------------------------------------------------------------------------*/
void	cGame::Init() {

}

void	cGame::Collision() {
	for (int i = 0; i < stage->GetStageSizeX(); i++) {
		for (int j = 0; j < stage->GetStageSizeY(); j++) {
			if (stage->GetTile(i, j) != 0) {
				CheckHitRectAndRect(character->GetPlayer(),        stage->GetMapTile(i, j));
				CheckHitRectAndRect(character->GetEnemyJumpman(),  stage->GetMapTile(i, j));
				CheckHitRectAndRect(character->GetEnemyHardBody(), stage->GetMapTile(i, j));
				CheckHitRectAndRect(character->GetEnemyWireman(),  stage->GetMapTile(i, j));
			}
		}
	}
	CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyJumpman());
	CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyHardBody());
	CheckHitRectAndRect(character->GetPlayer(), character->GetEnemyWireman());
}

void	cGame::Update() {
	input();
	character->Update();
	Collision();
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
	// 戻す
	SetDrawScreen(DX_SCREEN_BACK);

	camera->Render(bghandle);
	gui->Render();

	//DrawFormatString(10, 10, 0xFFFFFF, "操作キャラの座標:x=%d, y=%d", (int)FocusPos.x, (int)FocusPos.y);
}

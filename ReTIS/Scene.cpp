#include "Main.h"

/*------------------------------------------------------------------------------*
| <<< ÉQÅ[ÉÄ >>>
*------------------------------------------------------------------------------*/
void	cGame::Init() {

}
void	cGame::Collision() {
	for (int i = 0; i < stage->GetStageSizeX(); i++) {
		for (int j = 0; j < stage->GetStageSizeY(); j++) {
			if (stage->GetTile(i, j) != 0) {
				CheckHitRectAndRect(character->GetPlayer(), stage->GetMapTile(i, j));
				CheckHitRectAndRect(character->GetEnemyJumpman(), stage->GetMapTile(i, j));
				CheckHitRectAndRect(character->GetEnemyGunman(), stage->GetMapTile(i, j));
				CheckHitRectAndRect(character->GetEnemyHardBody(), stage->GetMapTile(i, j));
			}
		}
	}
}
void	cGame::Update() {
	input();
	character->Update();
	Collision();
}
void	cGame::Render() {
	stage->Render();
	character->Render();
	DrawFormatString(10, 10, 0xFFFFFF, "ëÄçÏÉLÉÉÉâÇÃç¿ïW:x=%d, y=%d", (int)FocusPos.x, (int)FocusPos.y);
}

#include "Main.h"

/*------------------------------------------------------------------------------*
| <<< �Q�[�� >>>
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
	camera->Update(FocusPos);
}
void	cGame::Render() {
	// �`���E�B���h�E��ύX
	SetDrawScreen(bghandle);
	ClearDrawScreen();
	// �`��
	stage->Render();
	character->Render();
	// �߂�
	SetDrawScreen(DX_SCREEN_BACK);

	camera->Render(bghandle);

	DrawFormatString(10, 10, 0xFFFFFF, "����L�����̍��W:x=%d, y=%d", (int)FocusPos.x, (int)FocusPos.y);
}

#include "Main.h"

/*------------------------------------------------------------------------------*
| <<< ƒQ[ƒ€ >>>
*------------------------------------------------------------------------------*/
void	cGame::Init() {

}
void	cGame::Collision() {
	for (int i = 0; i < stage->GetStageSizeX(); i++) {
		for (int j = 0; j < stage->GetStageSizeY(); j++) {
			if (stage->GetTile(i, j) != 0) {
				CheckHitRectAndRect(character->GetPlayer(), stage->GetMapTile(i, j));
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
}
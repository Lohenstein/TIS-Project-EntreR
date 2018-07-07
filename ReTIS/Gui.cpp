
#include "Main.h"

void	cGuiGame::Update() {

}
void	cGuiGame::Render() {

	const int hp_margin_x = 10;
	const int hp_margin_y = 10;
	const int hp_margin   = 60;

	for (int i = 0; i < hp; i++) {
		DrawGraph(hp_margin_x + (i * hp_margin), hp_margin_y, imgh_hp, true);
	}
}
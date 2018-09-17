#include "Main.h"

int		effects[10];

cEffectManager effect;

void	LoadEffects() {

	effects[0] = LoadEffekseerEffect("data/effect/laser.efk");
	effects[1] = LoadEffekseerEffect("data/effect/dead.efk");
	effects[2] = LoadEffekseerEffect("data/effect/smog.efk");
	effects[3] = LoadEffekseerEffect("data/effect/PlayerBullet.efk");
	effects[4] = LoadEffekseerEffect("data/effect/EnemyBullet.efk");
	effects[5] = LoadEffekseerEffect("data/effect/noumison_Laser.efk");
}
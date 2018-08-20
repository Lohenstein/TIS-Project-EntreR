#include "Main.h"

int		effects[10];

cEffectManager effect;

void	LoadEffects() {

	effects[0] = LoadEffekseerEffect("data/effect/laser.efk");
	effects[1] = LoadEffekseerEffect("data/effect/dead.efk");
}



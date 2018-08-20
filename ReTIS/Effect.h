#pragma once

extern	int		effects[10];
extern	void	LoadEffects();

class cEffect {
public:
	VECTOR	save_pos;
	float	x, y;
	int		effect_hnd;
	int		length = 0;

	cEffect(VECTOR pos, float zoom, int effect_num) {

		effect_hnd = PlayEffekseer2DEffect(effects[effect_num]);
		save_pos = pos;

		float x = pos.x - (FocusPos.x - (WINDOW_SIZE_X / 2.f));
		float y = pos.y - (FocusPos.y - (WINDOW_SIZE_Y / 2.f));

		SetScalePlayingEffekseer2DEffect(effect_hnd, zoom, zoom, zoom);
		SetPosPlayingEffekseer2DEffect(effect_hnd, x, y, 0);
	}
	void	UpdateEffect() {

		float x = save_pos.x - (FocusPos.x - (WINDOW_SIZE_X / 2.f));
		float y = save_pos.y - (FocusPos.y - (WINDOW_SIZE_Y / 2.f));

		SetPosPlayingEffekseer2DEffect(effect_hnd, x, y, 0);
		length++;
	}
	int		GetLength() { return length; }
};

class cEffectManager {
public:
	cEffect *effect[20];

	void	DeleteEffect() {
		for (int i = 0; i < 20; i++) {
			if (effect[i] != nullptr) {
				if (effect[i]->GetLength() > 2000){
					delete effect[i];
					effect[i] = nullptr;
				}
			}
		}
	}

	void	Shot(VECTOR pos, float zoom, int effect_num) {
		for (int i = 0; i < 20; i++) {
			if (effect[i] == nullptr) {
				effect[i] = new cEffect(pos, zoom, effect_num);
				break;
			}
		}
	}

	void	Update() {
		for (int i = 0; i < 20; i++) {
			if (effect[i] != nullptr) {
				effect[i]->UpdateEffect();
			}
		}
		DeleteEffect();
	}
};

extern cEffectManager effect;
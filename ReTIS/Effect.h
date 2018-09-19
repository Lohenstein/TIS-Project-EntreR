#pragma once

extern	int		effects[10];
extern	void	LoadEffects();

class cEffect {
public:
	VECTOR	save_pos;
	float	x, y;
	int		effect_hnd;
	int		length = 0;
	int		max;

	cEffect(VECTOR pos, float zoom, int effect_num, int maxlength = 2000) {

		effect_hnd	= PlayEffekseer2DEffect(effects[effect_num]);
		save_pos	= pos;
		max			= maxlength;

		x = pos.x - (camera_px);
		y = pos.y - (camera_py);

		SetScalePlayingEffekseer2DEffect(effect_hnd, zoom, zoom, zoom);
		SetPosPlayingEffekseer2DEffect(effect_hnd, x, y, 0);
	}
	void	UpdateEffect() {

		x = save_pos.x - (camera_px);
		y = save_pos.y - (camera_py);

		SetPosPlayingEffekseer2DEffect(effect_hnd, x, y, 0);
		length++;
	}
	int		GetLength() { return length; }
};

class cEffectManager {
public:
	cEffect *effect[64];

	void	DeleteEffect() {
		for (int i = 0; i < 64; i++) {
			if (effect[i] != nullptr) {
				if (effect[i]->GetLength() > effect[i]->max){
					delete effect[i];
					effect[i] = nullptr;
				}
			}
		}
	}

	void	Shot(VECTOR pos, float zoom, int effect_num, int max) {
		for (int i = 0; i < 64; i++) {
			if (effect[i] == nullptr) {
				effect[i] = new cEffect(pos, zoom, effect_num, max);
				break;
			}
		}
	}

	void	Update() {
		for (int i = 0; i < 64; i++) {
			if (effect[i] != nullptr) {
				effect[i]->UpdateEffect();
			}
		}
		DeleteEffect();
	}
};

extern cEffectManager effect;
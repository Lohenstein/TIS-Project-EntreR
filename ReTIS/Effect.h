#pragma once

extern	int		effects[10];
extern	void	LoadEffects();

class cEffect {
public:
	VECTOR	save_pos;
	float	x, y;
	int		effect_hnd;
	int		length;
	int		max;

	cEffect(const float px, const float py, float zoom, int effect_num, int maxlength = 2000) {

		effect_hnd	= PlayEffekseer2DEffect(effects[effect_num]);
		
		save_pos	= { px, py, 0.f };
		max			= maxlength;
		length		= 0;

		x = save_pos.x - camera_px;
		y = save_pos.y - camera_py;

		printf("%d,%d\n", (int)save_pos.x, (int)save_pos.y);

		SetScalePlayingEffekseer2DEffect(effect_hnd, zoom, zoom, zoom);
		SetPosPlayingEffekseer2DEffect(effect_hnd, x, y, 0);
	}
	void	UpdateEffect() {

		x = save_pos.x - camera_px;
		y = save_pos.y - camera_py;

		printf("%d,%d\n", (int)save_pos.x, (int)save_pos.y);

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
				if (effect[i]->GetLength() > 200){
					delete effect[i];
					effect[i] = nullptr;
				}
			}
		}
	}

	void	Shot(const float x, const float y, float zoom, int effect_num, int max) {
		for (int i = 0; i < 64; i++) {
			if (effect[i] == nullptr) {
				effect[i] = new cEffect(x, y, zoom, effect_num, max);
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

#include "Main.h"
/*------------------------------------------------------------------------------*
| <<< cMoveFloor >>>
*------------------------------------------------------------------------------*/
void	cMoveFloor::Update(float s, int p1, int p2) {

	// ˆÚ“®‚·‚éŠp“x‚Æ‹——£‚ð‹‚ß‚é
	float rad = atan2f(sy[p2] - sy[p1], sx[p2] - sx[p1]);
	float dis = sqrtf((sx[p2] - sx[p1]) * (sx[p2] - sx[p1]) + (sy[p2] - sy[p1]) * (sy[p2] - sy[p1]));

	// ‰ß‹Ž‚ÌˆÚ“®‚ð•Û‘¶
	old = pos;

	// ˆÚ“®‚·‚é
	pos.x = sx[p1] + (sinf(rad + PI) * p);
	pos.y = sy[p1] + (cosf(rad + PI) * p);

	// ˆÚ“®‚Ì•ûŒü‚ÉŒü‚©‚Á‚Ä“®‚©‚·
	if (flag)
		p += s;
	else
		p -= s;

	// ˆÚ“®‚Ì•ûŒü‚ÌØ‚è‘Ö‚¦
	if (p >= dis || p <= 0.f) {
		if (flag)
			flag = false;
		else
			flag = true;
	}
}

void	 cMoveFloor::Render() {
	DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
		GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
		0x0000FF, true);
}

void	 cMoveFloor::SetPoint(int num, float x, float y) {
	sx[num] = x;
	sy[num] = y;
}
/*------------------------------------------------------------------------------*
| <<< cDropFloor >>>
*------------------------------------------------------------------------------*/
void	cDropFloor::Update() {
	// —Ž‰ºˆ—
	if (flag) {
		time++;
		// °‚Éæ‚Á‚Ä0.5•bŒo‚Á‚½‚ç(60fps)
		const int fps = 60;
		if (time > fps / 2) {
			pos.y -= drop_p;
			drop_p -= gravity;
			const int prev_time = 5;
			if (time > fps * prev_time) {
				drop_p = 0.f;
				flag = false;
				time = 0;
				pos.y = sy;
			}
		}
	}
}

void	 cDropFloor::Render() {
	DrawBoxAA(GetPos().x - GetSize().x / 2.f, GetPos().y - GetSize().y / 2.f,
		GetPos().x + GetSize().x / 2.f, GetPos().y + GetSize().y / 2.f,
		0x0000FF, true);
}

void	cDropFloor::HitAction(cObject *hit) {
	flag = true;
}

void	cDropFloor::SetPoint(float x, float y) {
	pos = { x, y, 0.f };
	sx = x, sy = y;
}
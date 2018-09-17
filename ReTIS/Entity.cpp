
#include "Main.h"

using namespace std;

int floorimg;

/*------------------------------------------------------------------------------*
| <<< cMoveFloor >>>
*------------------------------------------------------------------------------*/
void	cMoveFloor::Update(float s, int p1, int p2) {

	// �ړ�����p�x�Ƌ��������߂�
	float rad = atan2f(sy[1] - sy[0], sx[1] - sx[0]);
	float dis = sqrtf((sx[1] - sx[0]) * (sx[1] - sx[0]) + (sy[1] - sy[0]) * (sy[1] - sy[0]));

	// �ߋ��̈ړ���ۑ�
	old = pos;

	// �ړ�����
	pos.x = sx[0] - (cosf(rad + DX_PI_F) * p);
	pos.y = sy[0] - (sinf(rad + DX_PI_F) * p);

	// �ړ��̕����Ɍ������ē�����
	if (flag)
		p += s;
	else
		p -= s;

	// �ړ��̕����̐؂�ւ�
	if (p >= dis || p <= 0.f) {
		if (flag)
			flag = false;
		else
			flag = true;
	}
}

void	 cMoveFloor::Render() {
	DrawRectGraph((int)GetPos().x - (int)GetSize().x / 2, (int)GetPos().y - (int)GetSize().y / 2, 576, 82, 128, 32, floorimg, true, false);
}

void	 cMoveFloor::SetPoint(int num, float x, float y) {
	sx[num] = x;
	sy[num] = y;
	floorimg = LoadGraph("data/img/block/block.png");
}
/*------------------------------------------------------------------------------*
| <<< cDropFloor >>>
*------------------------------------------------------------------------------*/
void	cDropFloor::Update() {
	// ��������
	if (flag) {
		time++;
		// ���ɏ����0.5�b�o������(60fps)
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
	DrawRectGraph((int)GetPos().x - (int)GetSize().x / 2, (int)GetPos().y - (int)GetSize().y / 2, 576, 32, 128, 32, floorimg, true, false);
}

void	cDropFloor::HitAction(cObject *hit) {
	flag = true;
}

void	cDropFloor::SetPoint(float x, float y) {
	pos = { x, y, 0.f };
	sx = x, sy = y;
}

/*------------------------------------------------------------------------------*
| <<< cEntityManager >>>
*------------------------------------------------------------------------------*/
/*
void	cEntityManager::LoadEntities(string name) {

	// ������csv�ǂݍ��ނ����Ȃ��@�\
	string entity = name + "mapdata_entity.csv";
	ifstream ifs; ifs.open(entity.c_str());
	string line;

	int count = 0;
	while (getline(ifs, line)) {

		vector<string> str = split(line, ',');

		switch (stoi(str.at(0))) {
		case 5:
			MessageBox(NULL, "c+�B", "Debug - Error", MB_OK);
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (movefloor[i] == nullptr) {
					movefloor[i] = new cMoveFloor(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)), stoi(str.at(5)), stoi(str.at(6)));
					break;
				}
			}
			break;
		case 6:
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (dropfloor[i] == nullptr) {
					dropfloor[i] = new cDropFloor(stoi(str.at(1)), stoi(str.at(2)), stoi(str.at(3)), stoi(str.at(4)));
					break;
				}
			}
			break;
		default:
			MessageBox(NULL, "�G���e�B�e�B�[�V�[�g�ǂݍ��ݎ���\n���݂��Ȃ��p�����[�^���ǂݍ��܂�܂����B", "Debug - Error", MB_OK);
			break;
		}
		count++;
	}
}

void	cEntityManager::Render() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (movefloor[i] != nullptr) movefloor[i]->Render();
		if (dropfloor[i] != nullptr) dropfloor[i]->Render();
	}
}

void	cEntityManager::Update() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		//if (movefloor[i] != nullptr) movefloor[i]->Update(5.f, 0, 1);
		//if (dropfloor[i] != nullptr) dropfloor[i]->Update();
	}
}
*/
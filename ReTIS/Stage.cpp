
#include "Main.h"

using namespace std;

vector<string> split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}
/*------------------------------------------------------------------------------*
| <<< cStageLoader >>>
*------------------------------------------------------------------------------*/
void	cStageManager::LoadStageData(char name[]) {

	ifstream ifs; ifs.open(name);
	string line;

	int count = 0;
	while (getline(ifs, line)) {

		vector<string> str = split(line, ',');

		// 最初の一行目はマップの大きさ情報
		if (count == 0) {
			// 大きさを格納
			stage_x = stoi(str.at(0));
			stage_y = stoi(str.at(1));
			// マップの大きさ分クラスの配列を確保
			stage = new cStageMapTile*[stage_x];
			for (int i = 0; i < stage_x; i++) {
				stage[i] = new cStageMapTile[stage_y];
			}
		}
		else {
			for (unsigned int i = 0; i < str.size(); i++) {
				// マップタイル情報を書き込む
				VECTOR pos  = { (float)i * bsize + bsize / 2.f, (float)(count - 1) * bsize + bsize / 2.f, NULL };
				VECTOR size = { bsize, bsize, NULL };
				stage[i][count - 1].SetData(pos, size, stoi(str.at(i)));
			}
		}
		count++;
	}
}
void	cStageManager::Render() {
	for (int i = 0; i < GetStageSizeX(); i++) {
		for (int j = 0; j < GetStageSizeY(); j++) {
			if (GetTile(i, j) != 0) {
				VECTOR pos  = GetPos(i, j);
				VECTOR size = GetSize(i, j);
				DrawBoxAA(pos.x - size.x / 2.f,
						pos.y - size.y / 2.f,
						pos.x + size.x / 2.f,
						pos.y + size.y / 2.f,
						0x555555, true);
			}
		}
	}
}
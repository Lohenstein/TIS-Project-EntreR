
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
void	cStageManager::LoadStageData(string name) {

	// 複数のcsv読み込むきたない機構
	string main  = name + ".csv";
	string bg1   = name + "_bg1.csv";
	string bg2   = name + "_bg2.csv";
	string light = name + "_light.csv";

	ifstream ifs_mapdata; ifs_mapdata.open(main.c_str());
	ifstream ifs_bg1;     ifs_bg1.open(bg1.c_str());
	ifstream ifs_bg2;     ifs_bg2.open(bg2.c_str());
	ifstream ifs_light;   ifs_light.open(light.c_str());

	string line_mapdata;
	string line_bg1;
	string line_bg2;
	string line_light;

	int count = 0;
	while ( getline(ifs_mapdata, line_mapdata) && getline(ifs_light, line_light) &&
			getline(ifs_bg1, line_bg1) && getline(ifs_bg2, line_bg2)) {

		vector<string> str_mapdata  = split(line_mapdata, ',');
		vector<string> str_bg1		= split(line_bg1, ',');
		vector<string> str_bg2		= split(line_bg2, ',');
		vector<string> str_light	= split(line_light, ',');

		// 最初の一行目はマップの大きさ情報
		if (count == 0) {
			// 大きさを格納
			stage_x = stoi(str_mapdata.at(0));
			stage_y = stoi(str_mapdata.at(1));
			// マップの大きさ分クラスの配列を確保
			stage = new cStageMapTile*[stage_x];
			for (int i = 0; i < stage_x; i++) {
				stage[i] = new cStageMapTile[stage_y];
			}
		}
		else {
			for (unsigned int i = 0; i < str_mapdata.size(); i++) {
				// マップタイル情報を書き込む
				VECTOR pos  = { (float)i * bsize + bsize / 2.f, (float)(count - 1) * bsize + bsize / 2.f, NULL };
				VECTOR size = { bsize, bsize, NULL };
				stage[i][count - 1].SetData(pos, size, stoi(str_mapdata.at(i)));
				stage[i][count - 1].SetBg(stoi(str_bg1.at(i)), stoi(str_bg2.at(i)), stoi(str_light.at(i)));
			}
		}
		count++;
	}
}

void	cStageManager::Render() {
	for (int i = 0; i < GetStageSizeX(); i++) {
		for (int j = 0; j < GetStageSizeY(); j++) {
			if (i * bsize > FocusCam.x - WINDOW_SIZE_X && i * bsize < FocusCam.x + WINDOW_SIZE_X &&
				j * bsize > FocusCam.y - WINDOW_SIZE_Y && j * bsize < FocusCam.y + WINDOW_SIZE_Y) {
				VECTOR pos  = GetPos(i, j);
				VECTOR size = GetSize(i, j);
				if (GetBg1(i, j) != -1)   DrawGraph(pos.x - size.x / 2.f, pos.y - size.y / 2.f, image[GetBg1(i, j)], true);
				if (GetBg2(i, j) != -1)   DrawGraph(pos.x - size.x / 2.f, pos.y - size.y / 2.f, image[GetBg2(i, j)], true);
				if (GetTile(i, j) != -1)  DrawGraph(pos.x - size.x / 2.f, pos.y - size.y / 2.f, image[GetTile(i, j)], true);
			}
		}
	}
}
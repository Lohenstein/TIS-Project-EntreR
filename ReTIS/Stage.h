#pragma once

const float bsize = 32.f;	// マップタイルのサイズ
using namespace std;

/*------------------------------------------------------------------------------*
| <<< マップタイルクラス >>>
*------------------------------------------------------------------------------*/
class cStageMapTile : public cObject {
protected:
	int		tile;		// タイル
	int		bg1;
	int		bg2;
	int		light;
public:
	cStageMapTile() { type = MapTile; };
	~cStageMapTile() {};
	int		GetTile()  { return tile; }
	int		GetBg1()   { return bg1; }
	int		GetBg2()   { return bg2; }
	int		GetLight() { return light; }
	void	HitAction(cObject *hit) {}
	void	SetBg(int ibg1, int ibg2) {
		bg1 = ibg1;
		bg2 = ibg2;
	}
	void	SetData(VECTOR setPos, VECTOR setSize, int setTile) {
		pos  = setPos;
		size = setSize;
		tile = setTile;
	}
};

/*------------------------------------------------------------------------------*
| <<< マップ読み込み兼ヒープ領域管理クラス >>>
*------------------------------------------------------------------------------*/
class cStageManager {
protected:
	cStageMapTile	**stage;
	int		stage_x;
	int		stage_y;
	int		image[3200];
public:
	cStageManager(string name) { 
		LoadStageData(name); 
		LoadDivGraph("data/img/block/block.png", 3200, 50, 64, bsize, bsize, image);
	};
	~cStageManager() {
		for (int i = 0; i < stage_x; i++) { delete[] stage[i]; }
		delete[] stage;
		stage = nullptr;
	}
	void	LoadStageData(string name);	// 最初に必ず通すこと！
	//VECTOR	*GetPos(int x, int y) { return &stage[x][y].GetPos(); }
	VECTOR	GetPos(int x, int y)   { return stage[x][y].GetPos(); }
	VECTOR	GetSize(int x, int y)  { return stage[x][y].GetSize(); }
	int		GetTile(int x, int y)  { return stage[x][y].GetTile(); }
	int		GetBg1(int x, int y)   { return stage[x][y].GetBg1(); }
	int		GetBg2(int x, int y)   { return stage[x][y].GetBg2(); }
	int		GetLight(int x, int y) { return stage[x][y].GetLight(); }
	int		GetStageSizeX() { return stage_x; }
	int		GetStageSizeY() { return stage_y; }
	void	Update();
	void	Render();
	void	RenderBG();
	cObject *GetMapTile(int x, int y) { return reinterpret_cast<cObject*>(&stage[x][y]); }
};

extern vector<string> split(string& input, char delimiter);
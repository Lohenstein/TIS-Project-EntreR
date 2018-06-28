#pragma once

const float bsize = 32.f;	// �}�b�v�^�C���̃T�C�Y

/*------------------------------------------------------------------------------*
| <<< �}�b�v�^�C���N���X >>>
*------------------------------------------------------------------------------*/
class cStageMapTile : public cObject {
protected:
	int		tile;		// �^�C��
public:
	cStageMapTile() { type = MapTile; };
	~cStageMapTile() {};
	int		GetTile() { return tile; }
	void	HitAction(cObject *hit) {}
	void	SetData(VECTOR setPos, VECTOR setSize, int setTile) {
		pos  = setPos;
		size = setSize;
		tile = setTile;
	}
};

/*------------------------------------------------------------------------------*
| <<< �}�b�v�ǂݍ��݌��q�[�v�̈�Ǘ��N���X >>>
*------------------------------------------------------------------------------*/
class cStageManager {
protected:
	cStageMapTile	**stage;
	int		stage_x;
	int		stage_y;
public:
	cStageManager(char name[]) { LoadStageData(name); };
	~cStageManager() {
		for (int i = 0; i < stage_x; i++) { delete[] stage[i]; }
		delete[] stage;
		stage = nullptr;
	}
	void	LoadStageData(char name[]);	// �ŏ��ɕK���ʂ����ƁI
	//VECTOR	*GetPos(int x, int y) { return &stage[x][y].GetPos(); }
	VECTOR	GetPos(int x, int y)  { return stage[x][y].GetPos(); }
	VECTOR	GetSize(int x, int y) { return stage[x][y].GetSize(); }
	int		GetTile(int x, int y) { return stage[x][y].GetTile(); }
	int		GetStageSizeX() { return stage_x; }
	int		GetStageSizeY() { return stage_y; }
	void	Update();
	void	Render();
	cObject *GetMapTile(int x, int y) { return reinterpret_cast<cObject*>(&stage[x][y]); }
};

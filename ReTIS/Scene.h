#pragma once

using namespace std;

extern	int menumode;

class cBase
{
public:
	cBase() {}
	~cBase() {}
	virtual void Init() {}
	virtual void Update() {}
	virtual void Render() {}
	void Show()
	{
		Init();
		Update();
		Render();
	}
};

class cLobby : public cBase
{
private:
	cStageManager *stage;
	cCharacterManager *character;
	cCamera *camera;
	cGuiGame *gui;
protected:

};

class cTitle : public cBase 
{
public:
	int		titlebg;
	int		menu_mode;

	char *title_str[4] = { "GAME START", "STAGE SELECT", "OPTION", "QUIT GAME" };
	char *stage_str[4] = { "������n", "���ъ�n", "�����n", "�߂�" };

	cTitle() {
		titlebg = LoadGraph("data/img/wall/titlebg.png");
		menumode = 0;
	}

	void	Init();
	void	Update();
	void	Render();
	void	DrawTitle();
	void	DrawStageSelect();
};

class cGame : public cBase
{
private:

	cStageManager *stage;
	cCharacterManager *character;
	cCamera *camera;
	cGuiGame *gui;
	cDialog *dialog;

	int imghandle[10];
	int bghandle;

protected:
	// �������ԂƂ�
	int min  = 5;
	int sec  = 0;
	int time = 0;
	int rectime = 0;
	int trans = 0;
public:
	cGame() { 
		//stagepath = "data/map/stage3/";
		stage	  = new cStageManager(stagepath);
		character = new cCharacterManager(stagepath);
		camera	  = new cCamera();
		gui       = new cGuiGame();
		dialog		= new cDialog("Test Message Dayo!! HogeHoge FugaFuga...!");
		bghandle  = MakeScreen(stage->GetStageSizeX()*(int)bsize, stage->GetStageSizeY()*(int)bsize, true);
		camera->SetStageSize(stage->GetStageSizeX(), stage->GetStageSizeY());

		// �摜
		imghandle[0] = LoadGraph("data/img/wall/wallbk.png");
		imghandle[1] = LoadGraph("data/img/hp/mpbar.png");
		imghandle[2] = LoadGraph("data/img/hp/mpbarbg.png");
		imghandle[3] = LoadGraph("data/img/wall/overbg.png");
		imghandle[4] = LoadGraph("data/img/wall/resultbg.png");
	}
	~cGame() { 
		delete stage;
		delete character;
		//delete entity;
		delete camera;
		delete gui;
		stage     = nullptr;
		character = nullptr;
		//entity	  = nullptr;
		camera	  = nullptr;
		gui		  = nullptr;
		for (int i = 0; i < 10; i++) { DeleteGraph(imghandle[i]); }
		DeleteGraph(bghandle);
	}
	void	Init();
	void	Collision();
	void	CollisionAroundMaptile(cObject *hit);
	void	Update();
	void	Render();

	void	UpdateGui();
	void	RenderGui();

	void	DrawResult();
	void	UpdateResult();
	void	DrawOver();
	void	UpdateOver();

	int		GetTime() { return time; }

	int		StageSizeX() { return stage->GetStageSizeX(); }
	int		StageSizeY() { return stage->GetStageSizeY(); }
};

extern std::unique_ptr<cGame> scene;
extern std::unique_ptr<cTitle> title;
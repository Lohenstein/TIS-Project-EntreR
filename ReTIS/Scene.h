#pragma once

using namespace std;
extern int totalranking[10];

enum TitleMenu {
	TITLE_GAMESTART,
	TITLE_STAGESELECT,
	TITLE_OPTION,
	TITLE_QUITGAME
};

enum MenuMode {
	MENUMODE_MAINMENU,
	MENUMODE_STAGESELECTMENU,
	MENUMODE_OPTIONMENU
};

enum OptionMenu {
	OPTION_FULLSCREEN,
	OPTION_OPTIONBACK
};

enum PauseMenu {
	PAUSE_CONTINUE,
	PAUSE_RESTART,
	PAUSE_BACK2TITLE
};

extern	MenuMode menumode;

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
	char *stage_str[4] = { "SATO FACTORY", "SMALL FOREST FACTORY", "FUJI FACTORY", "BACK" };
	char *option_str[2] = { "FULL/WINDOW", "BACK" };

	cTitle() {
		titlebg = LoadGraph("data/img/wall/titlebg.png");
		menumode = MENUMODE_MAINMENU;
	}

	void	Init();
	void	Update();
	void	Render();
	void	DrawTitle();
	void	DrawStageSelect();
	void	DrawOption();
};

class cGame : public cBase
{
private:

	cStageManager *stage;
	cCharacterManager *character;
	cCamera *camera;
	cGuiGame *gui;

	int imghandle[10];
	int bghandle;
	int time = 0;
	int totalscore = 0;
	int storagebox = 0;
	
protected:
	// §ŒÀŽžŠÔ‚Æ‚©
	int min  = 5;
	int sec  = 0;
	int rectime = 0;
	int trans = 0;
	bool hantei = false;
public:

	bool IsPaused = false;
	char *pause_str[3] = { "CONTINUE", "RESTART", "BACK TO TITLE" };

	cGame() { 
		//stagepath = "data/map/stage3/";
		stage	  = new cStageManager(stagepath);
		character = new cCharacterManager(stagepath);
		camera	  = new cCamera(stagepath);
		
		gui       = new cGuiGame();
		bghandle  = MakeScreen(stage->GetStageSizeX()*(int)bsize, stage->GetStageSizeY()*(int)bsize, true);
		camera->SetStageSize(stage->GetStageSizeX(), stage->GetStageSizeY());

		// ‰æ‘œ
		imghandle[0] = LoadGraph("data/img/wall/wallbk.png");
		imghandle[1] = LoadGraph("data/img/hp/mpbar.png");
		imghandle[2] = LoadGraph("data/img/hp/mpbarbg.png");
		imghandle[3] = LoadGraph("data/img/wall/overbg.png");
		imghandle[4] = LoadGraph("data/img/wall/resultbg.png");
		imghandle[5] = LoadGraph("data/img/wall/choco_bg.png");
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
		score = 0;
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
	void	RankingUpdate();
	void	RankingRender();

	void	DrawPauseMenu();

	int		GetTime() { return time; }

	int		StageSizeX() { return stage->GetStageSizeX(); }
	int		StageSizeY() { return stage->GetStageSizeY(); }
};

extern std::unique_ptr<cGame> scene;
extern std::unique_ptr<cTitle> title;

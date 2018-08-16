#pragma once

using namespace std;

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

class cGame : public cBase
{
private:

	cStageManager *stage;
	cCharacterManager *character;
	cCamera *camera;
	cGuiGame *gui;

	int imghandle[10];

	int bghandle;
	string stagepath;

protected:
	// §ŒÀŽžŠÔ‚Æ‚©
	int min  = 5;
	int sec  = 0;
	int time = 0;
	int rectime = 0;
	int trans = 0;
public:
	cGame() { 
		stagepath = "data/map/tenji/";
		stage	  = new cStageManager(stagepath);
		character = new cCharacterManager(stagepath);
		camera	  = new cCamera();
		gui       = new cGuiGame();
		bghandle  = MakeScreen(stage->GetStageSizeX()*(int)bsize, stage->GetStageSizeY()*(int)bsize, true);
		camera->SetStageSize(stage->GetStageSizeX(), stage->GetStageSizeY());

		// ‰æ‘œ
		imghandle[0] = LoadGraph("data/img/wall/wallbk.png");
		imghandle[1] = LoadGraph("data/img/hp/mpbar.png");
		imghandle[2] = LoadGraph("data/img/hp/mpbarbg.png");
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

	int		StageSizeX() { return stage->GetStageSizeX(); }
	int		StageSizeY() { return stage->GetStageSizeY(); }
};

extern std::unique_ptr<cBase> scene;
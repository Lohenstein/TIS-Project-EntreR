#pragma once
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

class cGame : public cBase
{
private:
	cStageManager *stage;
	cCharacterManager *character;
	cCamera *camera;
	cGuiGame *gui;
	int bghandle;
public:
	cGame() { 
		stage	  = new cStageManager("mapdata.csv"); 
		character = new cCharacterManager();
		camera	  = new cCamera();
		gui       = new cGuiGame();
		bghandle  = MakeScreen(stage->GetStageSizeX()*(int)bsize, stage->GetStageSizeY()*(int)bsize, false);
		camera->SetStageSize(stage->GetStageSizeX(), stage->GetStageSizeY());
	}
	~cGame() { 
		delete stage;
		delete character;
		delete camera;
		delete gui;
		stage     = nullptr;
		character = nullptr;
		camera	  = nullptr;
		gui		  = nullptr;
		DeleteGraph(bghandle);
	}
	void	Init();
	void	Collision();
	void	Update();
	void	Render();
	int		StageSizeX() { return stage->GetStageSizeX(); }
	int		StageSizeY() { return stage->GetStageSizeY(); }
};
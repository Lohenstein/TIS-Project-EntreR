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
protected:
	// §ŒÀŽžŠÔ‚Æ‚©
	int min  = 5;
	int sec  = 0;
	int time = 0;
public:
	cGame() { 
		stage	  = new cStageManager("data/map/test/mapdata"); 
		character = new cCharacterManager();
		camera	  = new cCamera();
		gui       = new cGuiGame();
		bghandle  = MakeScreen(stage->GetStageSizeX()*(int)bsize, stage->GetStageSizeY()*(int)bsize, true);
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
	void	CollisionAroundMaptile(cObject *hit);
	void	Update();
	void	Render();
	void	UpdateGui();
	void	RenderGui();
	int		StageSizeX() { return stage->GetStageSizeX(); }
	int		StageSizeY() { return stage->GetStageSizeY(); }
};
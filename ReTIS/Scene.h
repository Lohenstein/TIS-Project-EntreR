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
	int bghandle;
public:
	cGame() { 
		stage	  = new cStageManager("mapdata.csv"); 
		character = new cCharacterManager();
		camera	  = new cCamera();
		bghandle  = MakeScreen(stage->GetStageSizeX()*bsize, stage->GetStageSizeY()*bsize, false);
	}
	~cGame() { 
		delete stage;
		delete character;
		delete camera;
		stage     = nullptr;
		character = nullptr;
		camera	  = nullptr;
		DeleteGraph(bghandle);
	}
	void	Init();
	void	Collision();
	void	Update();
	void	Render();
	VECTOR	GetFocusPos();
};
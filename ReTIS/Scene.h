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
public:
	cGame() { 
		stage	  = new cStageManager("mapdata.csv"); 
		character = new cCharacterManager();
	}
	~cGame() { 
		delete stage;
		stage = nullptr;
		delete character;
		character = nullptr;
	}
	void	Init();
	void	Collision();
	void	Update();
	void	Render();
	VECTOR	GetFocusPos();
};
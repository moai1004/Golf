#pragma once
#include"TitleScene.h"
#include"SelectScene.h"
#include"PlayScene.h"
#include"ResultScene.h"
#include"HowToScene.h"
#include"SceneType.h"
#include"GameMode.h"
#include"NameInputScene.h"
#include"Network.h"
#include"MultiMenuScene.h"
#include"MultiLobbyScene.h"




class Scene {
private:
	
	int selectCourse = 0;

	bool isHost = false;
	bool isOnline = false;

	SceneType currentScene;
	GameMode currentMode;
	TitleScene titleScene;
	SelectScene selectScene;
	PlayScene playScene;
	ResultScene resultScene;
	HowToScene howToScene;
	NameInput nameInput;
	Network netWork;
	MultiMenuScene multiMenuScene;
	MultiLobbyScene multiLobbyScene;


	 char playerName[16] = "PLAYER";

public:

	Scene();

	void Update();
	void Draw();

	void ChangeScene(SceneType next);

	const char* GetPlayerName()const { return playerName; }
	char* GetPlayerNameBuffer() { return playerName; }

	

	bool GetIsHost()const {
		return isHost;
	}

	Network& GetNetWork() {
		return netWork;
	}
};
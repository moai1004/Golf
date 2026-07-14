#pragma once
#include"SceneType.h"
#include"Network.h"
#include"GameMode.h"



class MultiLobbyScene {
private:
	GameMode mode=ONLINE_TURN;

	int page;

	bool isOnline = false;
	bool isPlayerJoined = false;
	bool isConnected = false;

public:
	void Update(SceneType& scene, bool isHost,Network& network);
	void Draw(bool isHost);
	bool GetIsOnLine()const;

	GameMode GetMode()const;
};
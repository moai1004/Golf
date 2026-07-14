#pragma once
#include"SceneType.h"
#include"GameMode.h"
#include"RankingManager.h"

class TitleScene {
private:
	
	bool isLode = false;

	RankingManager rankingManager;


public:

	void Draw(char* name);
	void Update(SceneType& scene,GameMode& mode);
};
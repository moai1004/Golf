#pragma once
#include"SceneType.h"
#include"GameMode.h"
#include"RankingManager.h"
#include"GhostManager.h"
#include"Network.h"


class ResultScene {
private:
	
	RankingManager rankingManager;
	Network* netWork = nullptr;

	int shot = 0;

	int course = 0;

	int totalShot = 0;
	int totalShotP1 = 0;
	int totalShotP2 = 0;

	int holeScore[4];
	int holeScoreP1[4];
	int holeScoreP2[4];

	int player1Shot = 0;
	int player2Shot = 0;

	int myShot = 0;
	int enemyShot = 0;

	int result;

	GameMode gameMode = SINGLE;

	bool isGrandPrix = false;

	bool isSave = false;

	bool isNewRecode = false;

	bool myRematch = false;
	bool enemyRematch = false;
	bool enemyExit = false;

	char rank = 'C';

	const char* playerName="PLAYER";
	

public:
	void Update(SceneType& scene);
	void Draw();
	void SetPlayerName(const char* name) {
		playerName = name;
	}
	void SetResult(int nextShot, int nextCourse,int total,int totalP1,int totalP2,bool grandPrix,
		const int score[],const int scoreP1[],const int scoreP2[], int p1shot, int p2shot, GameMode gmode,int mShot,int eShot);

	void SetNetWork(Network* net);

};
#pragma once
#include"GameMode.h"
#include"SwingInput.h"
#include"Stage.h"
#include"SceneType.h"
#include"Star.h"
#include"ShotType.h"
#include"GolfPlayer.h"
#include"Network.h"




enum WEATHER {
	SUNNY,
	SUNSET,
	RAIN,
	NIGHT,
	SNOW
};

enum EFFECT_TYPE {
	EFFECT_GRASS,
	EFFECT_BUNKER,
	EFFECT_WATER,
	EFFECT_GOAL
};

enum ClassicState {
	CLASSIC_POWER,
	CLASSIC_ANGLE,
	CLASSIC_SHOT
};

struct Effect {
	float x;
	float y;

	float vx;
	float vy;

	float size;

	int life;
	int color;
	int type;
	
	bool use;


};


class PlayScene {
private:
	
	SwingInput sp;
	Stage stage;

	Network* netWork = nullptr;
	
	Ball* cameraTarget=nullptr;

	GolfPlayer player[2];
	int currentPlayer;
	
	GameMode gameMode = SINGLE;
	ShotMode shotMode = SWING;
	ClassicState classicState;
	WEATHER weather = SUNNY;


	static const int STAR_MAX = 100;
	star stars[STAR_MAX];

	static const int EFFECT_MAX = 100;
	Effect effects[EFFECT_MAX];

	static const int SCORE_MAX = 4;
	int holeScore[SCORE_MAX];
	int holeScoreP1[SCORE_MAX];
	int holeScoreP2[SCORE_MAX];

	int currentCourse = 0;
	int goalX=2000;
	int goalY=400;
	int goalRadius=20;
	int shotCount = 0;
	int sandX = 0;
	int sandZ = 0;
	int holeIndex ;
	int selectNumber;
	int dragStratX=0;
	int dragStratY=0;
	int starTimer = 0;
	int grandCourse = 0;
	int totalShot = 0;
	int totalShotP1 = 0;
	int totalShotP2 = 0;

	int myPlayer = 0;
	int enemyPlayer = 1;

	int hitSE;
	int capInSE;

	int goalTime = 0;
	int par = 2;

	

	bool isGoal = false;
	bool isHit = false;
	bool isResult = false;
	bool isDrag = false;
	bool powerUp = true;
	bool angleUp = true;
	bool isGhostMove = false;

	bool enemyTurnEnd = false;

	bool isGrandPrix = false;

	bool isHost = true;

	bool isMyTurn = false;

	bool enemyWatching = false;

	bool goalEffectPlayer[2] = { false,false };

	// CLASSIC—p
	float classicPower = 0.0f;
	float classicAngle = 0.0f;
	float windPower = 0.0f;


	const int NORMAL_COURSE_MAX = 4;

	const char* playerName = "PLAYER";

	



public:
	
	void Init(int course);
	void Update(SceneType& scene);
	void Draw();
	void CheckHitGoal();
	void ShotCount();
	void BackGround();
	void SetWeather(WEATHER next);
	void SetGameMode(GameMode mode);
	void LodeCourse(int course);
	void UpdateDrag(int cameraX);
	void UpdateClassic();
	void CreatEffect(float x, float y, int color, int count, int type);

	void ChangePlayer();

	void SetNetWork(Network* net);
	void SetIsHost(bool host);


	int GetShotCount()const;
	int GetTotalShot()const;
	int GetTotalShotP1()const;
	int GetTotalShotP2()const;
	int GetCurrentCount(int index)const;
	int GetMyShot();
	int GetEnemyShot();

	GameMode GetGameMode()const;
	const int* GetHoleScore()const;
	const int* GetHoleScoreP1()const;
	const int* GetHoleScoreP2()const;
	
	Ball& GetActiveBall();

	bool GetIsGrandPrix()const;

	void SetPlayerName(const char* name) {
		playerName = name;
	}


};
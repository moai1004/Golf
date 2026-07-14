#pragma once
#include"SceneType.h"
#include"Network.h"

class SelectScene {

	/*struct HolePos {
		int x;
		int y;
	};*/



private:

	/*HolePos holePos[5];

	float playerX;
	float playerY;

	int nearHole = 0;*/

	int select = 0;
	int selectCourse;
	const int COURSE_MAX = 5;

	bool isGrandPrix = false;

public:

	//void Into();
	void Update(SceneType& scene,int& course,bool isOnline,bool isHost,Network& netWork);
	void Draw();
};
#pragma once
#include"Ball.h"

 class GolfPlayer {

public:

	Ball ball;

	int shotCount = 0;

	const char* name = "";

	bool finished = false;

	bool isLocalPlayer = false;

};
#pragma once
#include"MousePoint.h"
#include"Ball.h"
#include<vector>


class SwingInput {
private:

	std::vector<MousePoint> trail;

	int mx=0;
	int my=0;
	int maxTrajectory = 50;
	bool isHit =false;
	bool alreadyHit=false;

public:

	void Update(int cameraX);
	void Draw(int cameraX)const;
	void Reset();

	bool LineCheckHit(Vec3 A,Vec3 B,Vec3 C,float radius,Vec3& hitpos);
	bool CheckHitBall(const Ball& ball, Vec3& hitpos);
	Vec3 SetTrail()const;
	
	float GetPower()const;
};
#pragma once
#include"Vec3.h"
#include"Stage.h"
#include"ShotType.h"



class Ball {
private:

	Stage stage;

	Vec3 position;
	Vec3 velocity;
	Vec3 shotPos ;


	ShotMode lastShotMode;

	int radius;
	int size = 64;
	int landType;

	float gravity = -0.35f;
	float friction = 0.02f;
	float ar = 0.99f;
	float hitoffsetY = 0;
	float prevGround = 0;
	float debugSlope = 0;
	float debugGround = 0;

	bool justLand;
	bool wasFly = false;
	bool hitSwing = false;
	bool isFlying = false;
	bool isBunker = false;

	//int mode;

public:
	
	void Update(Stage& stage,float winds);
	void Reset();
	void Draw(int cameraX)const;

	

	Vec3 GetPosition() const;
	int GetRadius()const;
	int GetSpeedX()const;
	int GetLandType()const;

	
	void HitMove(ShotMode mode,float power, float angle,float hight,float offsetY);
	bool IsMove()const;
	bool GetJustLand()const;
	bool IsBunker() const;
};
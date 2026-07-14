#pragma once
#include"SceneType.h"
#include"PlayScene.h"

class HowToScene {
private:
	
	int page = 0;

	int selectWeather = 0;


public:
	void Update(SceneType& scene,SceneType& oldScene);
	void Draw();

	int GetWeather()const;
}; 
#pragma once
#include"SceneType.h"

class NameInput {
private:

	char* playerName = nullptr;
	char temp[17] = "";
	int inputHandle=-1;

public:
	//NameInput();
	void Init();
	void SetNameBuffer(char* name);
	void Update(SceneType& scene);
	void Draw();

};
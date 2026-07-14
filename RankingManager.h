#pragma once
#include"RankingData.h"

class RankingManager {
private:

	bool newRecode = false;

	RankingData course1[5];
	RankingData course2[5];
	RankingData course3[5];
	RankingData course4[5];
	RankingData grandprix[5];


	RankingData* GetArray(int course);
public:
	void Lode(int course);
	void Save(int course);

	void AddRanking(int course,const char* name, int score);

	bool GetNewRecode()const;

	const RankingData* GetRanking(int course)const;

};
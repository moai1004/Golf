#include<fstream>
#include"RankingManager.h"
#include"DxLib.h"


void RankingManager::Lode(int course) {

	const char* fileName[5]{
		"result1.txt",
		"result2.txt",
		"result3.txt",
		"result4.txt",
		"grandprix.txt"
	};

	std::ifstream file(fileName[course]);

	RankingData* ranking = GetArray(course);

	if (!file) {
		for (int i = 0;i < 5;i++) {

			strcpy_s(ranking[i].name, "NONE");
			ranking[i].score = 999;

		}

		Save(course);
		return;
		
	}

	for (int i = 0;i < 5;i++) {

		file >> ranking[i].name;
		file >> ranking[i].score;
	}

}

void RankingManager::Save(int course) {

	const char* fileName[5]{
		"result1.txt",
		"result2.txt",
		"result3.txt",
		"result4.txt",
		"grandprix.txt"
	};

	std::ofstream file(fileName[course]);

	RankingData* ranking = GetArray(course);

	for (int i = 0;i < 5;i++) {

		file << ranking[i].name << " " << ranking[i].score << std::endl;

	}

}

void RankingManager::AddRanking(int course,const char* name, int score) {

	RankingData* ranking = GetArray(course);

	if (score >= ranking[4].score) {
		newRecode = false;
		return;
	}
	

	ranking[4].score = score;
	strcpy_s(ranking[4].name, name);

	

	for (int i = 4;i > 0;i--) {
		if (ranking[i].score < ranking[i - 1].score) {

			RankingData temp = ranking[i];

			ranking[i] = ranking[i - 1];

			ranking[i - 1] = temp;

		}
		else {
			break;
		}
	}
	newRecode = true;

	Save(course);


}



const RankingData* RankingManager::GetRanking(int course)const {
	switch (course) {
	case 0:return course1;
	case 1:return course2;
	case 2:return course3;
	case 3:return course4;
	case 4:return grandprix;
	}
	return course1;
}

 RankingData* RankingManager::GetArray(int course) {

	switch(course) {
	case 0:return course1;
	case 1:return course2;
	case 2:return course3;
	case 3:return course4;
	case 4:return grandprix;
	}
	return course1;
}

 bool RankingManager::GetNewRecode()const {
	 return newRecode;
 }
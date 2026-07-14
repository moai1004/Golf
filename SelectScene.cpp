#include<DxLib.h>
#include"SelectScene.h"
#include"Input.h"

//void SelectScene::Into() {
//
//	holePos[0] = { 250,180 };
//	holePos[1] = { 450,180 };
//	holePos[2] = { 650,180 };
//	holePos[3] = { 850,180 };
//	holePos[4] = { 550,330 };
//
//	playerX = 250;
//	playerY = 260;
//
//}



void SelectScene::Update(SceneType& scene,int& course,bool isOnline,bool isHost,Network& netWork) {
	//--------------
	//コース選択
	//--------------

	if(!isOnline||isHost)
	{
		if (IsPushKey(KEY_INPUT_UP)) {
			select--;
			if (select < 0) {
				select = COURSE_MAX - 1;
			}
			if(isOnline){
				courseSelectDate date;
				date.course = select;
				date.start = false;
				netWork.Send(&date, sizeof(date));
			}
		}
		if (IsPushKey(KEY_INPUT_DOWN)) {
			select++;
			if (select >= COURSE_MAX) {
				select = 0;
			}
			if(isOnline){
				courseSelectDate date;
				date.course = select;
				date.start = false;
				netWork.Send(&date, sizeof(date));
			}
			
		}

		//if (IsPushKey(KEY_INPUT_LEFT)) {
		//	select--;

		//	if (select < 0) {
		//		select = COURSE_MAX - 1;
		//	}

		//}

		//if (IsPushKey(KEY_INPUT_RIGHT)) {
		//	select++;

		//	if (select >= COURSE_MAX) {
		//		select = 0;
		//	}
		//}

		//playerX = holePos[select].x;
		////playerX+=(targetx-playerx)*0.1f



		//選んだコースの情報を取得
		if (IsPushKey(KEY_INPUT_RETURN)) {

			selectCourse = select;

			course = select;
			//選択したのがグランプリか判断
			if (select == 4) {
				isGrandPrix = true;
			}
			else {
				isGrandPrix = false;
			}
			courseSelectDate date;
			date.course = course;
			date.start = true;
			netWork.Send(&date, sizeof(date));


			//後でコース番号を渡す
			scene = GAME;
		}
		if (IsPushKey(KEY_INPUT_ESCAPE)) {
			scene = TITLE;
		}
	}else{
		courseSelectDate date;
		if (netWork.Receive(&date, sizeof(date)) > 0) {
			select =date.course;
			if (date.start) {
				course = date.course;
				scene = GAME;
			}

		}
	}
}

void SelectScene::Draw() {

	//------------------
	//背景
	//------------------

	//夜空
			//空
	DrawBox(0, 0, 1280, 720, GetColor(10, 10, 40), TRUE);
	//星
	for (int i = 0; i < 100; i++) {

		DrawPixel(GetRand(1280), GetRand(300), GetColor(255, 255, 255));

	}
	//月
	DrawCircle(100, 100, 50, GetColor(255, 255, 200), TRUE);
	DrawCircle(120, 90, 50, GetColor(10, 10, 40));
	
	


	DrawString(240, 80, "COURSE SELECT", GetColor(255, 255, 255));

	const char* courseNum[5] = {
		"course 1",
		"course 2",
		"course 3",
		"course 4",
		/*"course 5",
		"course 6",
		"course 7",
		"course 8",
		"course 9",*/
		"Grand Prix"
	};

	for (int i = 0;i < COURSE_MAX;i++) {

		int color = GetColor(255, 255, 255);

		if (i == select) {

			color = GetColor(255, 255, 0);

			DrawString(190, 150 + i * 40, ">", color);

		}

		DrawString(260, 150 + i * 40, courseNum[i], color);

	}

	/*for (int i = 0;i < COURSE_MAX;i++) {
		DrawCircle(holePos[i].x, holePos[i].y, 20, GetColor(255, 255, 255), TRUE);
		DrawCircle(holePos[i].x, holePos[i].y, 5, GetColor(0, 0, 0), FALSE);
	}1*/


	DrawString(180, 400, "[ENTER] Start      [ESC] Back", GetColor(200, 200, 200));

}
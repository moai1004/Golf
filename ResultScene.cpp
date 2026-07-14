#include<DxLib.h>
#include"ResultScene.h"
#include"Input.h"


void ResultScene::Update(SceneType& scene) {

	if (!isSave) {

		rankingManager.Lode(course);

		if (gameMode == SINGLE) {

			if (isGrandPrix) {

				rankingManager.AddRanking(4,playerName, totalShot);

				isNewRecode = rankingManager.GetNewRecode();

			}
			else {

				rankingManager.AddRanking(course,playerName, shot);
				
				

				isNewRecode = rankingManager.GetNewRecode();
				if (isNewRecode) {
					
				}
			}
		}

		if (gameMode == ONLINE_TURN) {

			ResultSelectDate date;

			date.shot = myShot;
			date.rematch = false;
			date.exit = false;

			netWork->Send(&date, sizeof(date));

		}

		isSave = true;

	}

	if (gameMode == ONLINE_TURN) {
		
		ResultSelectDate date;

		if (netWork->Receive(&date, sizeof(date)) > 0) {
			enemyShot = date.shot;
			enemyRematch = date.rematch;
			enemyExit = date.exit;
		}

		if (myRematch && enemyRematch) {

			isSave = false;
			myRematch = false;
			enemyRematch = false;
			enemyExit = false;

			scene = COURSE_SELECT;
		}
		if (enemyExit) {

			isSave = false;
			myRematch = false;
			enemyRematch = false;
			enemyExit = false;

			scene = TITLE;
		}

		if (IsPushKey(KEY_INPUT_SPACE)) {

			myRematch = true;

			ResultSelectDate  date;

			date.shot = myShot;
			date.rematch = true;
			date.exit = false;

			netWork->Send(&date, sizeof(date));

		}

		if (IsPushKey(KEY_INPUT_RETURN)) {

			ResultSelectDate date;

			date.shot = myShot;
			date.rematch = false;
			date.exit = true;

			netWork->Send(&date, sizeof(date));
			
			scene = TITLE;

		}
	}
	else {

		if (IsPushKey(KEY_INPUT_RETURN)) {
			isSave = false;
			scene = TITLE;
		}

		if (IsPushKey(KEY_INPUT_SPACE)) {
			isSave = false;
			scene = COURSE_SELECT;
		}
		//グランプリ用のスコア
		if (isGrandPrix) {
			if (totalShot <= 12) rank = 'S';
			else if (totalShot <= 20) rank = 'A';
			else if (totalShot <= 32) rank = 'B';
		}
		//単ステージのスコア
		else {
			if (shot <= 3) rank = 'S';
			else if (shot <= 5) rank = 'A';
			else if (shot <= 8) rank = 'B';
		}

	}

	
	

}

void ResultScene::Draw() {

	int white = GetColor(255, 255, 255);
	int yellow = GetColor(255, 255, 0);
	int color = white;
	const char* text = "";



	switch (gameMode) {
	case SINGLE:

		

		if (isGrandPrix) {

			DrawString(240, 120, "GRAND PRIX RESULT", yellow);

			for (int i = 0; i < 4; i++)
			{


				if (holeScore[i] == 1) {
					text = "★HOLE IN ONE!★";
					color = yellow;
				}

				else if (holeScore[i] == 2) {
					text = "EAGLE!";
					color = white;
				}

				else if (holeScore[i] == 3) {
					text = "BIRDIE!";
					color = white;
				}

				else if (holeScore[i] == 4) {
					text = "PAR";
					color = white;
				}

				else {
					text = "BOGEY";
					color = white;
				}

				DrawFormatString(200, 180 + i * 40, color, "HOLE %d   %d SHOT   %s", i + 1, holeScore[i], text);
			}

			DrawFormatString(220, 360, yellow, "TOTAL SHOT : %d", totalShot);

			DrawFormatString(270, 410, yellow, "RANK : %c", rank);

			DrawBox(10, 20, 160, 200, white, FALSE);

			const RankingData* ranking = rankingManager.GetRanking(4);
			DrawString(50, 40, "Ranking", white);
			for (int i = 0;i < 5;i++) {
				DrawFormatString(40, 80 + i * 20, white, "%d %s %d", i + 1, ranking[i].name, ranking[i].score);
			}

		}
		else {

			if (shot == 1) {
				text = "★HOLE IN ONE!★";
				color = yellow;
			}
			else if (shot == 2) {
				text = "EAGLE!";
				color = white;
			}
			else if (shot == 3) {
				text = "BIRDIE!";
				color = white;
			}
			else if (shot == 4) {
				text = "PAR";
				color = white;
			}
			else {
				text = "BOGEY";
				color = white;
			}

			DrawString(290, 140, "RESULT", yellow);

			DrawFormatString(200, 210, white, "HOLE %d   %d SHOT   %s", course + 1, shot, text);

			DrawFormatString(280, 300, white, "RANK : %c", rank);

			DrawBox(10, 20, 160, 200, white, FALSE);

			const RankingData* ranking = rankingManager.GetRanking(course);
			DrawString(50, 40, "Ranking", white);
			for (int i = 0;i < 5;i++) {
				DrawFormatString(40, 80 + i * 20, white, "%d %s %d", i + 1, ranking[i].name, ranking[i].score);
			}

		}
		if (isNewRecode) {

			DrawString(40, 220, "NEW RECORD", yellow);

		}
		break;

	case LOCAL_VS:
		if (isGrandPrix) {

			DrawString(220, 120, "GRAND PRIX RESULT", white);

			for (int i = 0; i < 4; i++)
			{

				DrawFormatString(180, 180 + i * 40, color, "HOLE %d   P1:%d   P2:%d", i + 1, holeScoreP1[i], holeScoreP2[i]);
			}

			DrawFormatString(190, 360, yellow, "TOTAL SHOT   P1:%d P2:%d", totalShotP1, totalShotP2);

			if (totalShotP1 < totalShotP2) {
				DrawString(220, 410, "★Player1 WIN★", white);
			}
			else if (totalShotP1 > totalShotP2) {
				DrawString(220, 410, "★Player2 WIN★", white);
			}
			else {
				DrawString(250, 410, "★DROW★", white);
			}

		}
		else {

			DrawFormatString(200, 200, white, "Player1:%d  Player2:%d", player1Shot, player2Shot);
			if (player1Shot < player2Shot) {
				DrawString(220, 300, "★Player1 WIN★", white);
			}
			else if (player1Shot > player2Shot) {
				DrawString(220, 300, "★Player2 WIN★", white);
			}
			else {
				DrawString(250, 300, "★DROW★", white);
			}

		}

		break;
	case ONLINE_TURN:
		if (isGrandPrix)
		{
			DrawString(220, 120, "GRAND PRIX RESULT", yellow);

			for (int i = 0;i < 4;i++)
			{
				DrawFormatString(180, 170 + i * 40, white, "HOLE%d  YOU:%d  ENEMY:%d", i + 1, holeScoreP1[i], holeScoreP2[i]);
			}

			DrawFormatString(180, 320, yellow, "TOTAL  YOU:%d  ENEMY:%d", totalShotP1, totalShotP2);

			if (totalShotP1 < totalShotP2)
				DrawString(250, 60, "YOU WIN!", yellow);
			else if (totalShotP1 > totalShotP2)
				DrawString(250, 60, "YOU LOSE...", white);
			else
				DrawString(260, 60, "DRAW", white);

			if (!myRematch)
				DrawString(170, 380, "Press SPACE to Rematch", white);
			else if (!enemyRematch)
				DrawString(170, 380, "Waiting for opponent...", white);
			else
				DrawString(170, 380, "Starting...", yellow);

			if (enemyRematch) {
				if ((GetNowCount() / 300) % 2 == 0) {
					DrawString(170, 400, "Opponent wants Rematch!", yellow);
				}
			}
		}
		else
		{
			DrawFormatString(220, 180, white, "YOU   : %d", myShot);
			DrawFormatString(220, 220, white, "ENEMY : %d", enemyShot);

			if (myShot < enemyShot)
				DrawString(290, 60, "YOU WIN!", yellow);
			else if (myShot > enemyShot)
				DrawString(290, 60, "YOU LOSE...", white);
			else
				DrawString(290, 60, "DRAW", white);

			if (!myRematch)
				DrawString(170, 380, "Press SPACE to Rematch", white);
			else if (!enemyRematch)
				DrawString(170, 380, "Waiting for opponent...", white);
			else
				DrawString(170, 380, "Starting...", yellow);

			if (enemyRematch) {
				if ((GetNowCount() / 300) % 2 == 0) {
					DrawString(170, 400, "Opponent wants Rematch!", yellow);
				}
			}

		}
		if (gameMode == ONLINE_TURN)
		{
			DrawString(170, 450, "[SPACE] REMATCH   [ENTER] EXIT", white);

			DrawBox(140, 340, 500, 420, white, FALSE);

		    DrawString(170, 350, "STATUS", yellow);
			
		}
		else {
			DrawString(180, 450, "[SPACE] RETRY   [ENTER] TITLE", white);
			DrawString(290, 60, "CLEAR!", yellow);
		}


	}



}

void ResultScene::SetResult(int nextShot, int nextCourse,int total,int totalP1,int totalP2,bool grandPrix,const int score[],const int scoreP1[],const int scoreP2[], int p1shot, int p2shot, GameMode gmode,int mShot,int eShot) {

	//クリア時の打数やコース情報を取得
	shot = nextShot;
	course = nextCourse;
	totalShot = total;
	totalShotP1 = totalP1;
	totalShotP2 = totalP2;
	isGrandPrix = grandPrix;
	player1Shot = p1shot;
	player2Shot = p2shot;
	myShot = mShot;
	enemyShot = eShot;
	gameMode = gmode;


	for (int i = 0;i < 4;i++) {
		holeScore[i] = score[i];
		holeScoreP1[i] = scoreP1[i];
		holeScoreP2[i] = scoreP2[i];
	}
}

void ResultScene::SetNetWork(Network* net) {
	netWork = net;
}


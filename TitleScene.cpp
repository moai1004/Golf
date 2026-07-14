#include<DxLib.h>
#include"TitleScene.h"
#include"Input.h"

void TitleScene::Update(SceneType& scene,GameMode& mode) {
	if (!isLode) {
		for(int i = 0;i < 5;i++) {
			rankingManager.Lode(4);
		}
		isLode = true;
	}
	

	if (IsPushKey(KEY_INPUT_SPACE)) {
		mode = SINGLE;
		scene = COURSE_SELECT;
	}
	if (IsPushKey(KEY_INPUT_RETURN)){
		scene = MULTI_MENU;
		
	}
	if (IsPushKey(KEY_INPUT_L)) {
		mode = LOCAL_VS;
		scene = COURSE_SELECT;
	}
	if (IsPushKey(KEY_INPUT_TAB)) {
		scene = HOWTO;
	}
	if (IsPushKey(KEY_INPUT_N)) {
		scene = NAME_INPUT;
	}

}

void TitleScene::Draw(char* name) {

	int white = GetColor(255, 255, 255);
	int black = GetColor(0, 0, 0);

	//-----------------
	//”wŒi
	//-----------------

	DrawBox(0, 0, 1280, 480, GetColor(120, 200, 255), TRUE);
	//‰_
	DrawCircle(120, 120, 40, white);
	DrawCircle(160, 120, 50, white);
	DrawCircle(200, 120, 40, white);
	DrawCircle(480, 200, 40, white);
	DrawCircle(520, 200, 50, white);
	DrawCircle(560, 200, 40, white);
	//’n–Ê
	DrawBox(0, 410, 1280, 720, GetColor(80, 180, 80), TRUE);

	DrawCircle(40, 400, 10, white, TRUE);
	DrawCircle(40, 400, 10, black, FALSE);


	DrawBox(440, 20, 600, 200, black, FALSE);

	const RankingData* ranking = rankingManager.GetRanking(4);
	DrawString(490, 40, "Ranking", black);
	for (int i = 0;i < 5;i++) {
		DrawFormatString(475, 80 + i * 20, black, "%d %s %d", i + 1, ranking[i].name, ranking[i].score);
	}


	DrawString(250, 100, "Golf 2D", black);


	DrawString(220, 280, "[Space]  SINGLE", black);
	DrawString(220, 310, "[Enter]  MULTI", black);
	DrawString(220, 340, "[L]      TORN", black);
	DrawString(220, 370, "[N]      NAME", black);
	DrawString(220, 400, "[Tab]    Exit", black);

	DrawFormatString(50, 50, black, "NAME:%s", name);
}
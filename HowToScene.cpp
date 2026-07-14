#include<DxLib.h>
#include"SceneManager.h"
#include"HowToScene.h"
#include"Input.h"

void HowToScene::Update(SceneType& scene,SceneType& oldScene) {

	//--------------------------------
	// ÉyÅ[ÉWïœçX
	//--------------------------------

	if (IsPushKey(KEY_INPUT_RIGHT)) {
		page++;
		if (page > 1) {
			page = 0;
		}
	}

	if (IsPushKey(KEY_INPUT_LEFT)) {
		page--;
		if (page < 0) {
			page = 1;
		}
	}

	//--------------------------------
	// ñﬂÇÈ
	//--------------------------------

	if (IsPushKey(KEY_INPUT_TAB)) {
		scene = oldScene;
	}

	//--------------------------------
	// WEATHERê›íËÉyÅ[ÉW
	//--------------------------------

	if (page == 1) {

		if (IsPushKey(KEY_INPUT_UP)) {

			selectWeather--;

			if (selectWeather < 0) {
				selectWeather = 4;
			}

		}

		if (IsPushKey(KEY_INPUT_DOWN)) {

			selectWeather++;

			if (selectWeather > 4) {
				selectWeather = 0;
			}


		}

	}

}

void HowToScene::Draw() {

	int white = GetColor(255, 255, 255);
	int green = GetColor(0, 255, 0);
	int red = GetColor(255, 0, 0);
	int yellow = GetColor(255, 255, 0);
	int bule = GetColor(0, 0, 255);

	//--------------------------------
	// PAGE1
	//--------------------------------

	if (page == 0) {

		DrawString(250, 40, "How To", yellow);

		DrawString(220, 80, "=== CONTROL ===", green);

		DrawString(120, 120, "[1] SWING MODE", white);
		DrawString(120, 145, "[2] DRAG MODE", white);
		DrawString(120, 170, "[3] TIMING MODE", white);

		DrawString(120, 200, "Shot : Left Click + Drag", white);

		DrawString(120, 225, "[R] Retry", white);
		DrawString(120, 250, "[TAB] Back", white);

		DrawString(220, 285, "=== GAME RULE ===", green);

		DrawString(120, 315, "Single : All shot modes", white);

		DrawString(120, 340, "VS/Online : SWING only", white);

		DrawString(120, 365, "Host creates room", white);

		DrawString(120, 390, "Client joins room", white);

		DrawString(120, 415, "BUNKER : Ball slows down", yellow);

		DrawString(120, 440, "WATER : Reset position", bule);

		DrawString(450, 440, "[RIGHT] NEXT PAGE", white);

	}

	//--------------------------------
	// PAGE2
	//--------------------------------

	else if (page == 1) {

		//--------------------------------
		// îwåiÉvÉåÉrÉÖÅ[
		//--------------------------------

		switch (selectWeather) {

		case SUNNY:
			// ê∞ÇÍ
		//ãÛ
			DrawBox(0, 0, 1280, 480, GetColor(120, 200, 255), TRUE);
			//â_
			DrawCircle(120, 120, 40, GetColor(255, 255, 255));
			DrawCircle(160, 120, 50, GetColor(255, 255, 255));
			DrawCircle(200, 120, 40, GetColor(255, 255, 255));
			DrawCircle(480, 200, 40, GetColor(255, 255, 255));
			DrawCircle(520, 200, 50, GetColor(255, 255, 255));
			DrawCircle(560, 200, 40, GetColor(255, 255, 255));
			//ínñ 
			DrawBox(0, 410, 1280, 720, GetColor(80, 180, 80), TRUE);
			break;

		case SUNSET:
			//ó[ì˙
			//ãÛ
			DrawBox(0, 0, 1280, 450, GetColor(255, 170, 100), TRUE);
			//ínñ 
			DrawBox(0, 410, 1280, 720, GetColor(120, 100, 60), TRUE);
			//ëæóz
			DrawCircle(480, 120, 60, GetColor(255, 220, 120), TRUE);
			break;

		case RAIN:
			//âJ
			//ãÛ
			DrawBox(0, 0, 1280, 480, GetColor(80, 80, 100), TRUE);
			//âJ
			for (int i = 0;i < 200;i++) {
				int x = GetRand(1280);
				int y = GetRand(720);

				DrawBox(x, y, x - 5, y + 15, GetColor(180, 180, 255), TRUE);
			}
			break;

		case NIGHT:
			//ñÈãÛ
			//ãÛ
			DrawBox(0, 0, 1280, 720, GetColor(10, 10, 40), TRUE);
			//êØ
			for (int i = 0; i < 100; i++) {

				DrawPixel(GetRand(1280),GetRand(300),GetColor(255, 255, 255));

			}
			//åé
			DrawCircle(100, 100, 50, GetColor(255, 255, 200), TRUE);
			DrawCircle(120, 90, 50, GetColor(10, 10, 40));
			break;

		case SNOW:
			//ê·
			//ãÛ
			DrawBox(0, 0, 1280, 720, GetColor(220, 230, 255), TRUE);
			//ê·
			for (int i = 0;i < 150;i++) {
				DrawCircle(GetRand(1280), GetRand(720), 2, GetColor(255, 255, 255));
			}
			//ínñ 
			DrawBox(0, 450, 1280, 720, GetColor(240, 240, 240), TRUE);
			break;

		}

		//--------------------------------
		// UI
		//--------------------------------

		const char* weatherName[] = {
			 "SUNNY",
			"SUNSET",
			"RAIN",
			"NIGHT",
			"SNOW"
		};

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);

		DrawBox(20, 40, 620, 460, GetColor(0, 0, 0), TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawBox(20, 40, 620, 460, GetColor(255, 255, 255), FALSE);

		DrawBox(20, 40, 620, 460, GetColor(0, 0, 0), FALSE);

		DrawString(230, 60, "WEATHER SETTING", white);

		DrawFormatString(250, 170, white, "< %s >", weatherName[selectWeather]);

		DrawString(200, 300, "[UP/DOWN] CHANGE WEATHER", white);

		DrawString(230, 380, "[LEFT] BACK PAGE", white);

		

	}


	


}

int HowToScene::GetWeather()const {
	return selectWeather;
}
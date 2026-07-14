#include"SceneManager.h"


Scene::Scene() {
	currentScene = TITLE;
	nameInput.SetNameBuffer(playerName);
	resultScene.SetNetWork(&netWork);
}

//シーン切り替え
void Scene::ChangeScene(SceneType next) {
	currentScene = next;
}

void Scene::Update() {
	//前のシーンを保存
	static SceneType oldScene = TITLE;
	//HOWTOから戻るためのシーンを保存
	static SceneType backScene;
	//HOWTO以外のシーンを保存
	if (currentScene != HOWTO) {
		backScene = currentScene;
	}

	//現在のシーンを更新
	switch (currentScene) {
	case TITLE:
		titleScene.Update(currentScene,currentMode);
		break;
	case COURSE_SELECT:
		selectScene.Update(currentScene,selectCourse,isOnline,isHost,netWork);
		break;
	case GAME:
		playScene.Update(currentScene);
		break;
	case CLEAR:
		resultScene.Update(currentScene);
		break;
	case HOWTO:
		howToScene.Update(currentScene,backScene);
		break;
	case NAME_INPUT:
		nameInput.Update(currentScene);
		break;
	case MULTI_MENU:
		multiMenuScene.Update(currentScene,isHost);
		break;
	case MULTI_LOBBY:
		multiLobbyScene.Update(currentScene, isHost,netWork);
		if (multiLobbyScene.GetIsOnLine()) {
			isOnline = true;
			currentMode = (GameMode)multiLobbyScene.GetMode();
			
		}
		
		break;
	}


	//ゲームシーンに移行するときにコースを初期化する
	if (oldScene != currentScene) {
		if (oldScene==COURSE_SELECT&&currentScene == GAME) {
			playScene.SetGameMode(currentMode);
			playScene.SetNetWork(&netWork);
			playScene.SetIsHost(isHost);
			playScene.Init(selectCourse);
		}
		else if (currentScene == NAME_INPUT) {
			nameInput.Init();
		}
		oldScene = currentScene;
	}
	//選択した背景を取得
	playScene.SetWeather((WEATHER)howToScene.GetWeather());
	resultScene.SetPlayerName(playerName);
	playScene.SetPlayerName(playerName);

	
	//リザルトに結果を残す
	resultScene.SetResult(playScene.GetShotCount(), selectCourse, playScene.GetTotalShot(), playScene.GetTotalShotP1(), playScene.GetTotalShotP2(), playScene.GetIsGrandPrix(), playScene.GetHoleScore(), playScene.GetHoleScoreP1(), playScene.GetHoleScoreP2(),
		playScene.GetCurrentCount(0), playScene.GetCurrentCount(1), playScene.GetGameMode(), playScene.GetMyShot(), playScene.GetEnemyShot());

	nameInput.SetNameBuffer(playerName);
}
void Scene::Draw() {
	//シーンそれぞれの描写
	
	switch (currentScene) {
	case TITLE:
		titleScene.Draw(playerName);
		break;
	case COURSE_SELECT:
		selectScene.Draw();
		break;
	case GAME:
		playScene.Draw();
		break;
	case CLEAR:
		resultScene.Draw();
		break;
	case HOWTO:
		howToScene.Draw();
		break;
	case NAME_INPUT:
		nameInput.Draw();
		break;
	case MULTI_MENU:
		multiMenuScene.Draw();
		break;
	case MULTI_LOBBY:
		multiLobbyScene.Draw(isHost);
		break;
	}

}


#include"MultiMenuScene.h"
#include"Input.h"
#include<DxLib.h>


void MultiMenuScene::Update(SceneType& scene,bool& isHost) {
	if (IsPushKey(KEY_INPUT_H)) {
		isHost = true;
		scene = MULTI_LOBBY;
	}
	if (IsPushKey(KEY_INPUT_J)) {
		isHost = false;
		scene = MULTI_LOBBY;
	}
	if (IsPushKey(KEY_INPUT_TAB)) {
		scene = TITLE;
	}
}

void MultiMenuScene::Draw() {
	DrawString(10, 450, "[TAB] title", GetColor(255, 255, 255));
	DrawString(300, 200, "[H] host", GetColor(255, 255, 255));
	DrawString(300, 240, "[J] join", GetColor(255, 255, 255));
}
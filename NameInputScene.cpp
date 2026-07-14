#include<DxLib.h>
#include"NameInputScene.h"
#include"Input.h"

 /*NameInput::NameInput() {
	inputHandle = MakeKeyInput(16, FALSE, FALSE, FALSE);
	SetActiveKeyInput(inputHandle);
}*/

 void NameInput::Init() {
	 if (inputHandle == -1) {
		 DeleteKeyInput(inputHandle);
	  }
	 inputHandle = MakeKeyInput(16, FALSE, FALSE, FALSE);

	 if (playerName) {
		 SetKeyInputString(playerName, inputHandle);
		 strcpy_s(temp,playerName);
	 }

	 SetActiveKeyInput(inputHandle);

 }

void NameInput::SetNameBuffer(char* name) {
	playerName = name;
}

void NameInput::Update(SceneType& scene) {


	

	GetKeyInputString(temp,inputHandle);

	if (playerName) {
		strcpy_s(playerName, 16,temp);
	}
	if (IsPushKey(KEY_INPUT_RETURN)) {
		scene = TITLE;
	}

}

void NameInput::Draw() {

	DrawString(220, 150, "ENTER YOUR NAME", GetColor(255, 255, 255));
	//DrawFormatString(200,180,GetColor(255, 255, 0),"Handle = %d",inputHandle);
		DrawKeyInputString(200, 220, inputHandle);
	
	DrawString(200, 300, "[ENTER] SAVE", GetColor(255, 255, 255));

}
#include<DxLib.h>
#include"MultiLobbyScene.h"
#include"Input.h"



void MultiLobbyScene::Update(SceneType& scene, bool isHost,Network& netWork) {
	
	static int power = 0;
	static bool isJoined = false;
	if (isHost) {

		if (!isConnected) {
			 netWork.Host(5000);
			isConnected = true;
			
		}

		if (IsPushKey(KEY_INPUT_RIGHT)) {
			page++;
			if (page > 1) {
				mode = ONLINE_TURN;
				page = 0;
			}
			LobbyDate date;
			date.mode = mode;
			netWork.Send(&date, sizeof(date));
			
		}

		if (IsPushKey(KEY_INPUT_LEFT)) {
			page--;
			if (page < 0) {
				mode = ONLINE_SIMULANEOUS;
				page = 1;
			}
			LobbyDate date;
			date.mode = mode;
			netWork.Send(&date, sizeof(date));
			
		}


		LobbyDate date;
		
		if (netWork.Receive(&date, sizeof(date)) > 0) {
			isPlayerJoined = true;
		}

	

		if (isPlayerJoined && IsPushKey(KEY_INPUT_RETURN)) {
			isOnline = true;

			LobbyDate date;
			date.mode = -1;
			netWork.Send(&date, sizeof(date));

			scene = COURSE_SELECT;
			

		}
		
	}
	else {

		if (!isConnected) {
			 netWork.Connect("127.0.0.1", 5000);
			isConnected = true;
		
		}
		
		

		//if (IsPushKey(KEY_INPUT_SPACE)) {
		if (!isJoined) {
			LobbyDate date;
			date.mode = 999;
			netWork.Send(&date, sizeof(date));
			isJoined = true;
		}
	
		//}
		LobbyDate date;
			if (netWork.Receive(&date, sizeof(date)) > 0) {
				
				if (date.mode == -1) {
					isOnline = true;
					scene = COURSE_SELECT;
				}else{
					mode = (GameMode)date.mode;
				}
				
			}
			/*int ret = netWork.Receive(&date, sizeof(date));
			printfDx("%d", ret);
			if (ret > 0) {
				printfDx("%f", date.power);
			}*/
			

	}
}



void MultiLobbyScene::Draw(bool isHost) {
	int white = GetColor(255, 255, 255);
	if (isHost) {
		DrawString(300, 200, "HOST", white);
	}
	else {
		DrawString(300, 200, "Connect", white);
	}

	if (isPlayerJoined) {
		DrawString(300, 100, "Player Join", white);
		DrawFormatString(250, 300, white, "mode:%d", mode);
	}

}

bool MultiLobbyScene::GetIsOnLine() const {
	return isOnline;
}
GameMode MultiLobbyScene::GetMode()const {
	return mode;
}
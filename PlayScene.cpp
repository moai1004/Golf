#include<DxLib.h>
#include"PlayScene.h"
#include"Input.h"
#include"GolfPlayer.h"


// update前に行う処理
void PlayScene::Init(int course) {

	if (gameMode == ONLINE_TURN) {
		isMyTurn = isHost;
		enemyWatching = false;
		enemyTurnEnd = false;

		if (isHost) {
			myPlayer = 1;
			enemyPlayer = 0;
		}
		else {
			myPlayer = 0;
			enemyPlayer = 1;
		}
	}
	

	//ステージ読み込み
	currentCourse = course;

	hitSE = LoadSoundMem("music/hitSE.mp3");
	capInSE = LoadSoundMem("music/capInSE.mp3");

	const char* fileName[] = {
		"data/course1.txt",
		"data/course2.txt",
		"data/course3.txt",
		"data/course4.txt",
		
	};
	//コース単体
	if (course >= 0 && course < NORMAL_COURSE_MAX) {
		stage.Lode(fileName[course]);
		isGrandPrix = false;
	}
	//連続
	else if (course == 4) {

		isGrandPrix = true;
		grandCourse = 0;
		totalShot = 0;
		totalShotP1 = 0;
		totalShotP2 = 0;
	}
	//全コースを取得
	if (isGrandPrix) {
		LodeCourse(grandCourse);
	}
	else {
		LodeCourse(course);
	}

	player[0].name = "Player1";
	player[1].name = "Player2";

	player[0].finished = false;
	player[1].finished = false;

	goalEffectPlayer[0] = false;
	goalEffectPlayer[1] = false;

	player[0].shotCount = 0;
	player[1].shotCount = 0;

	currentPlayer = 0;

	player[0].ball.Reset();
	player[1].ball.Reset();
	sp.Reset();
	
	cameraTarget = &player[myPlayer].ball;
	enemyWatching = false;
	//ゴール設定

	holeIndex = stage.FindHole();
	//初期位置
	 goalX = holeIndex*stage.GetSize()+stage.GetSize()/2;
	
	 goalY = stage.GetGroundY(goalX);
	 //ゴールの大きさ
	 goalRadius = 20;
	
	 //打った回数
	 shotCount = 0;
	 
	 classicState = CLASSIC_ANGLE;

	 classicPower = 0;
	 classicAngle = 0;

	 powerUp = true;
	 angleUp = true;

	 //状態
	 isGoal = false;
	 isHit = false;
	 isResult = false;

	 totalShotP1 = 0;
	 totalShotP2 = 0;

	 goalTime = 0;

	 //星
	 for (int i = 0;i < STAR_MAX;i++) {
		 stars[i].x = GetRand(1280);
		 stars[i].y = GetRand(300);
		 stars[i].time = GetRand(360);

	 }

	 for (int i = 0;i < SCORE_MAX;i++) {
		 holeScore[i] = 0;
		 holeScoreP1[0] = 0;
		 holeScoreP2[1] = 0;
	 }

	 int rand= GetRand(99);
	//ランダムに風の強さを決める
	 if (rand>80) {
		 windPower = (GetRand(60) - 20) * 0.1f;
	 }
	 else {
		 windPower = (GetRand(50) - 20) * 0.01f;
	 }
	 if (gameMode == ONLINE_TURN) {
		 if (isHost) {
			 Packet gamePacket{};
			 gamePacket.game.wind = windPower;
			 netWork->Send(&gamePacket, sizeof(gamePacket));

	
		 }
		 else {
			
			 Packet gamePacket{};
			 if (netWork->Receive(&gamePacket, sizeof(gamePacket))) {
				 windPower = gamePacket.game.wind;
			 }

		 }
	 }

}
// 毎フレーム更新
void PlayScene::Update(SceneType& scene) {
	Packet packet{};
	if (netWork->Receive(&packet, sizeof(packet))>0) {
		
		switch (packet.type) {
		case PACKET_SHOT:
		{

			ShotDate& shot = packet.shot;

			if (shot.endTurn) {
			
				enemyTurnEnd = true;

			}
			else {
				if(!player[enemyPlayer].finished)
				{
					player[enemyPlayer].ball.HitMove(SWING, shot.power, shot.angle, shot.high, shot.offsetY);
					enemyWatching = true;
					cameraTarget = &player[enemyPlayer].ball;
					isMyTurn = false;
				}
			}


			

			if (player[enemyPlayer].finished && !player[myPlayer].finished) {
				isMyTurn = true;
			}

			break;
		}

		case PACKET_GAME:
		{

			GameDate& game = packet.game;

			if (game.nextHole) {
				
				isGoal = true;
				goalTime = 0;
			}

			if (game.finished) {
				player[enemyPlayer].finished = true;

				if (!goalEffectPlayer[enemyPlayer]) {

					Vec3 pos = player[enemyPlayer].ball.GetPosition();

					CreatEffect(pos.x - cameraTarget->GetPosition().x + 200, 400 - pos.z, GetColor(255, 255, 0), 60, EFFECT_GOAL);

					goalEffectPlayer[enemyPlayer] = true;
				}
			}
			player[enemyPlayer].shotCount = game.shotCount;
			break;
		}

		}
	}
	

	Ball& ball = GetActiveBall();
	Ball* activeBall;

	if (gameMode == ONLINE_TURN) {
		activeBall = isMyTurn ? &player[myPlayer].ball : &player[enemyPlayer].ball;

		if (enemyTurnEnd && !player[enemyPlayer].ball.IsMove()) {
			enemyTurnEnd = false;
			enemyWatching = false;
			cameraTarget = &player[myPlayer].ball;
			if (player[myPlayer].finished) {
				
				Packet endPacket = {};

				endPacket.type = PACKET_SHOT;

				endPacket.shot.endTurn = true;

				netWork->Send(&endPacket, sizeof(endPacket));

			}
			else {
				isMyTurn = true;
			}
			
		}
	}
	else {
		activeBall = &player[currentPlayer].ball;
	}

	//ボールの位置受け取り格納
	Vec3 ballPos = activeBall->GetPosition();
	//カメラはボールを中心に追従
	int cameraX = ballPos.x - 200;
	isHit = false;

	

	//設定
	if (IsPushKey(KEY_INPUT_TAB)) {
		scene = HOWTO;
	}
	//モード切替
	if (IsPushKey(KEY_INPUT_1)) {
		shotMode = SWING;
	}
	if (IsPushKey(KEY_INPUT_2)) {
		shotMode = DRAG;
	}
	if (IsPushKey(KEY_INPUT_3)) {
		shotMode = CLASSIC;
	}
	if (shotMode == DRAG) {
		UpdateDrag(cameraX);
	}

	if (shotMode == CLASSIC)
	{
		UpdateClassic();
	}

	if (shotMode == SWING) {

	//--------------------------------
	// 入力処理（ボールが止まっている時のみ）
	//--------------------------------
	
		//ボールが動いていなければ実行
		if (!ball.IsMove()) {


			// マウス軌跡更新
			sp.Update(cameraX);
			Vec3 hitpos;
			// 軌跡とボールの当たり判定
			isHit = sp.CheckHitBall(ball, hitpos);

			if (isHit) {

				//打数+1
				ShotCount();

				// ボール中心座標（画面座標）
				Vec3 center;

				center.x = ballPos.x;
				center.y = 400 - ballPos.z;
				// 打点の上下ズレ
				float offsetY = hitpos.y - center.y;
				//高さ補正（下叩きで浮く）
				float high = -offsetY * 0.015f;
				//スイング方向
				Vec3 dir = sp.SetTrail();
				//パワー（長さ）
				float power = sqrt(dir.x * dir.x + dir.y * dir.y) * 0.15;
				// 角度（ラジアン）
				float angle = atan2(-dir.y, dir.x);
				if (power < 1.0f)power = 1.0f;
				if (power > 26.0f)power = 26.0f;

				if (gameMode == ONLINE_TURN) {
					if (!player[enemyPlayer].finished) {
						if (!isMyTurn)return;
					}
				

					if (player[myPlayer].finished)return;
				

					Packet shotPacket{};

					shotPacket.type = PACKET_SHOT;

					shotPacket.shot.power = power * 0.8;
					shotPacket.shot.angle = angle;
					shotPacket.shot.high = high;
					shotPacket.shot.offsetY = offsetY;
					shotPacket.shot.endTurn = false;

					

					netWork->Send(&shotPacket, sizeof(shotPacket));

					Packet gamePacket{};

					gamePacket.type = PACKET_GAME;

					gamePacket.game.shotCount = player[myPlayer].shotCount;

					netWork->Send(&gamePacket, sizeof(gamePacket));

				}
				
				//打つ
				ball.HitMove(SWING,power * 0.8f, angle, high, offsetY);
				cameraTarget = &player[myPlayer].ball;
				if (!player[myPlayer].finished) {
					isMyTurn = false;
				}
			
				
				PlaySoundMem(hitSE, DX_PLAYTYPE_BACK);
				
			}
		}
	}
	
	//--------------------------------
	// ボール更新（物理）
	//--------------------------------
	if (gameMode == ONLINE_TURN) {
		player[myPlayer].ball.Update(stage, windPower);
		player[enemyPlayer].ball.Update(stage, windPower);
	}
	else {
		ball.Update(stage, windPower);
	}
	
	if (gameMode == ONLINE_TURN) {
		if (enemyWatching && !player[enemyPlayer].ball.IsMove()) {
			
			enemyWatching = false;
			cameraTarget = &player[myPlayer].ball;

			if (!player[myPlayer].finished&&!player[enemyPlayer].finished) {
				isMyTurn = true;
			}
			
		}
			
	}
	
	if (ball.GetJustLand()) {
		
		Vec3 ballPos = ball.GetPosition();

		float drawX = ballPos.x - cameraX;
		float drawY = 400 - ballPos.z;
		
		int type = ball.GetLandType();
		if (type == GRASS) {
			CreatEffect(drawX, drawY, GetColor(0, 255, 0),1,EFFECT_GRASS);
		}
		else if (type == BUNKER) {
			CreatEffect(drawX, drawY, GetColor(220, 200, 120), 15,EFFECT_BUNKER);
		}
		else if (type == WATER) {
			CreatEffect(drawX, drawY, GetColor(100, 180, 255), 20,EFFECT_WATER);
		}
	
	}

	//--------------------------------
	// ゴール判定
	//--------------------------------
	if (!isGoal) {
		CheckHitGoal();
	}
	//--------------------------------
	// リセット
	//--------------------------------

	if (CheckHitKey(KEY_INPUT_R) == 1) {
		Init(currentCourse);
	}
	
	if (isGoal&&!isResult) {
		
		goalTime++;

		if (goalTime >= 180) {
			
			isResult = true;

			if (isGrandPrix) {

				holeScore[grandCourse] = shotCount;
				holeScoreP1[grandCourse] = player[0].shotCount;
				holeScoreP2[grandCourse] = player[1].shotCount;

				totalShot += shotCount;
				
				if (gameMode == ONLINE_TURN) {
					totalShotP1 += player[myPlayer].shotCount;
					totalShotP2 += player[enemyPlayer].shotCount;
				}
				else {
					totalShotP1 += player[0].shotCount;
					totalShotP2 += player[1].shotCount;
				}

				grandCourse++;

				if (grandCourse >= 4) {

					scene = CLEAR;

				}
				else {

					LodeCourse(grandCourse);

					player[0].ball.Reset();
					player[1].ball.Reset();

					player[0].shotCount = 0;
					player[1].shotCount = 0;

					player[0].finished = false;
					player[1].finished = false;

					goalEffectPlayer[0] = false;
					goalEffectPlayer[1] = false;

					currentPlayer = 0;

					sp.Reset();

					shotCount = 0;
					goalTime = 0;
					
					isResult = false;

					if (gameMode == ONLINE_TURN) {
						enemyTurnEnd = false;
						enemyWatching = false;

						cameraTarget = &player[myPlayer].ball;
						if (isHost) {
							isMyTurn = true;
						}
						else {
							isMyTurn = false;
						}

					}


					return;

				}

			}
			else {
				scene = CLEAR;
			}

		}

	}

	for (int i = 0;i < EFFECT_MAX;i++) {
		
		if (!effects[i].use)continue;

		if (effects[i].type == EFFECT_GRASS) {

			effects[i].x += effects[i].vx;
			effects[i].y += effects[i].vy;

			effects[i].vx *= 0.98f;
			effects[i].vy *= 0.98f;
		}
		else if (effects[i].type == EFFECT_BUNKER) {

			effects[i].x += effects[i].vx;
			effects[i].y += effects[i].vy;

			effects[i].vx *= 0.98f;
			effects[i].vy *= 0.98f;


		}
		else if (effects[i].type == EFFECT_WATER) {

			effects[i].x += effects[i].vx;
			effects[i].y += effects[i].vy;

			effects[i].vx *= 0.99f;
			effects[i].vy += 0.08f;
		}
		else if (effects[i].type == EFFECT_GOAL) {

			effects[i].x += effects[i].vx;
			effects[i].y += effects[i].vy;

			effects[i].vx *= 0.98f;
			effects[i].vy *= 0.98f;

			effects[i].size *= 1.02f;
		}
	
		effects[i].size *= 0.97f;
		
		effects[i].life--;

		if (effects[i].life < 0) {
			effects[i].use = false;
		}

	}

	if (gameMode == ONLINE_TURN) {
		
		static bool myWasMoving = false;

		if (player[myPlayer].ball.IsMove()) {

			myWasMoving = true;

		}else if (myWasMoving) {

			myWasMoving = false;

			Packet endPacket = {};

			endPacket.type = PACKET_SHOT;

			endPacket.shot.endTurn = true;

			netWork->Send(&endPacket, sizeof(endPacket));
	
		}
		
		
	}
	else {

		static bool wasMoving = false;

		if (ball.IsMove()) {
			wasMoving = true;
		}
		else if (wasMoving) {
			wasMoving = false;
			if (gameMode == LOCAL_VS)
			{

				ChangePlayer();
			
			}

		}

	}
	//星
	starTimer++;
	
	
}
// 描画処理
void PlayScene::Draw() {
	

	Ball& ball = player[currentPlayer].ball;
	
	int white = GetColor(255, 255, 255);
	int black = GetColor(0, 0, 0);

	Vec3 ballPos ;

	if (gameMode == ONLINE_TURN) {
	
		if (cameraTarget) {
			ballPos = cameraTarget->GetPosition();
		}
		 
	}
	else {

		ballPos = ball.GetPosition();
	}


	int cameraX = ballPos.x - 200;

	int cx = holeIndex*stage.GetSize()+stage.GetSize()/2;
	int cy = stage.GetGroundY(cx);

	int drawX = cx - cameraX;
	int drawY = 400 - cy;

	//--------------------------------
	// オブジェクト描画
	//--------------------------------
	BackGround();
	if (gameMode == ONLINE_TURN) {
		player[myPlayer].ball.Draw(cameraX);
		player[enemyPlayer].ball.Draw(cameraX);
	}
	else {
		ball.Draw(cameraX);
	}
	
	
	stage.Draw(cameraX);
	if (shotMode == SWING) {
		sp.Draw(cameraX);
	}
	if (shotMode == DRAG && isDrag) {
		int mx, my;
		GetMousePoint(&mx, &my);

		DrawLine(ballPos.x - cameraX, 400 - ballPos.z, mx, my, GetColor(0, 0, 0));
	}
	
	//--------------------------------
	// ゴール描画
	//--------------------------------
	DrawLine(drawX, drawY - 30, drawX, drawY + 10, GetColor(255, 255, 255));
	DrawTriangle(drawX, drawY - 60, drawX + 30, drawY - 48, drawX, drawY - 26, GetColor(255, 0, 0), TRUE);

	//--------------------------------
	// UI(debug含)
	//--------------------------------

	int uiX = 20;
	int uiY = 20;
	int uiW = 360;
	int uiH = 120;

	

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);

	DrawBox(uiX, uiY, uiX + uiW, uiY + uiH, black, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawBox(uiX, uiY, uiX + uiW, uiY + uiH, white, FALSE);

	DrawLine(uiX, uiY + 55, uiX + uiW, uiY + 55, white);

	DrawString(uiX + 15, uiY + 10, "HOLE", white);

	DrawFormatString(uiX + 15, uiY + 30, white, "SHOT : %d", player[currentPlayer].shotCount);
	
	//ゴールまでの距離
	float dx = goalX - ballPos.x;
	float distance = fabs(dx);
	DrawFormatString(400, 10, GetColor(255, 255, 255), "距離:%.0f", distance);
	

	//---------------------------------
	//ミニマップ
	//---------------------------------

	int mapX = uiX+20;
	int mapY = uiY+80;
	int mapW = 300;
	int mapH = 12;

	float stageWidth = W * stage.GetSize();
	float t = ballPos.x / stageWidth;
	float goalT = goalX / stageWidth;

	int mx, my;

	//バー（背景）
	DrawBox(mapX, mapY, mapX + mapW, mapY + mapH, GetColor(100, 100, 100), TRUE);
		

	//ゴール位置
	int goalPos = mapX + mapW * goalT;
	
	DrawLine(goalPos, mapY - 15, goalPos, mapY, GetColor(255, 255, 255));
	DrawTriangle(goalPos, mapY - 15, goalPos + 12, mapY-10 , goalPos, mapY - 5, GetColor(255, 0, 0), TRUE);


	
	
	for (int i = 0;i < W;i++) {
		int x = mapX + (i / (float)W) * mapW;

		int color = GetColor(0, 255, 0);

		int terrain = stage.GetTypeIndex(i);
		if (terrain == BUNKER) color = GetColor(255, 255, 0);
		if (terrain == WATER) color = GetColor(0, 0, 255);

		DrawLine(x, mapY, x, mapY + mapH, color);
	 }
	
	//プレイヤー位置
	int playerPos = mapX + mapW * t;

	//点滅
	int pulse = 5 + sin(GetNowCount() * 0.02) * 2;

	if (gameMode == ONLINE_TURN)
	{
		DrawFormatString(uiX + 160, uiY + 30, white, "NOW  YOU:%d  ENEMY:%d", player[myPlayer].shotCount, player[enemyPlayer].shotCount);
		DrawFormatString(uiX + 160, uiY + 10, white, "TOTAL  YOU:%d  ENEMY:%d", totalShotP1, totalShotP2);
		float myT = player[myPlayer].ball.GetPosition().x / stageWidth;

		float enemyT = player[enemyPlayer].ball.GetPosition().x / stageWidth;

		int myPos = mapX + mapW * myT;
		int enemyPos = mapX + mapW * enemyT;

		// 自分
		DrawCircle(myPos, mapY + mapH / 2, 5, GetColor(255, 255, 0), TRUE);
		DrawCircle(myPos, mapY + mapH / 2, 5, black, FALSE);

		// 相手
		DrawCircle(enemyPos, mapY + mapH / 2, 5, GetColor(255, 0, 0), TRUE);
		DrawCircle(enemyPos, mapY + mapH / 2, 5, black, FALSE);
	}
	else
	{
		DrawFormatString(uiX + 180, uiY + 30, white, "TOTAL  P1:%d  P2:%d", totalShotP1, totalShotP2);

		DrawCircle(playerPos, mapY + mapH / 2, pulse, white, TRUE);
		DrawCircle(playerPos, mapY + mapH / 2, pulse, black, FALSE);
	}


	//--------------------------------
	//予測線
	//--------------------------------
	if (!ball.IsMove())
	{

		//------------
		// SWING MODE
		//------------

		if (shotMode == SWING) {
			int mx, my;

			GetMousePoint(&mx, &my);

			Vec3 dir = sp.SetTrail();
			//ボール位置
			float startX = ballPos.x - cameraX;
			float startY = 400 - ballPos.z;
			// マウス→ボール方向
			float dx = startX - mx;
			float dy = startY - my;
			// 長さ
			float len = sqrt(dx * dx + dy * dy);
			//スウィングの長さを取ってスピードを取る
			float swingSpeed = sqrt(dir.x * dir.x + dir.y * dir.y);

			float maxSwing = 120.0f;

			if (swingSpeed > maxSwing)swingSpeed = maxSwing;
			//スウィングスピードからpowerに変える
			float power = swingSpeed / maxSwing * 100.0f;
			//powerを100%で考えて10%ずつに分ける
			int percent = (int)(power / 10) * 10;

			int gaugeX = 40;
			int gaugeY = 450;
			int gaugeW = 300;
			int gaugeH = 20;
			// 最大距離
			float maxLen = 200.0f;
			//powerとゲージの値をmax100にする
			if (power > 100)power = 100;

			if (percent > 100)percent = 100;

			if (len > 0) {
				dx = dx / len * maxLen;
				dy = dy / len * maxLen;
				len = maxLen;
			}
			// 円までの距離
			float pull = power * 2;

			if (pull > 200)pull = 200;
			//ボールから予測位置までの半径
			float radius = 150.0f;

			float dirX = dx / len;
			float dirY = dy / len;

			float targetWorldX = startX + dirX * radius;//ボールの予測位置
			float groundZ = stage.GetGroundY(targetWorldX);//地面の情報取得

			// 反対位置
			
			float targetX = targetWorldX - cameraX;
			float targetY = 400 - groundZ;

			

			//その場所から当たった場合の位置
			float hitHight = (my - startY);

			DrawCircle(targetX, targetY, 10, GetColor(255, 255, 0), FALSE);

			//--------------
			//powerゲージ
			//--------------

			int fill = gaugeW * power / 100;

			int color;

			if (percent < 40)
				color = GetColor(0, 255, 0);

			else if (percent < 80)
				color = GetColor(255, 255, 0);

			else
				color = GetColor(255, 0, 0);
			//ゲージ背景
			DrawBox(gaugeX, gaugeY, gaugeX + gaugeW, gaugeY + gaugeH, GetColor(80, 80, 80), TRUE);
			//powerゲージ
			DrawBox(gaugeX, gaugeY, gaugeX + fill, gaugeY + gaugeH, color, TRUE);
			//powerの割合
			DrawFormatString(gaugeX + 120, gaugeY - 25, white, "POWER %d%%", percent);
			//powerから予測線の長さを決める
			float arcPower = power * 3.0f;
			//その位置にいるマウスから大まかな位置設定
			float arcHight = hitHight * 0.3f;

			if (arcHight < 0)arcHight = 0;
			if (arcHight > 120)arcHight = 120;


			

			//-------------
			//予測線
			//-------------

			for (int i = 0;i < 10;i++) {

				//予測位置まで放物線上に10個の円で表示する

				float t = i / 10.0f;

				float px = startX + dirX * radius * t;
				float py = startY + dirY * radius * t - sin(t * DX_PI_F) * arcHight;


				DrawCircle(px, py, 3, GetColor(0, 255, 0));
		}
		
		}

		//------------
		// DRAG MODE
		//------------
		
		if (shotMode == DRAG) {

			int mx, my;
			//マウスの現在地取得
			GetMousePoint(&mx, &my);
			//ボール位置の取得
			float startX = ballPos.x - cameraX;
			float startY = 400 - ballPos.z;
			//ボールとマウスの距離
			float dx = startX - mx;
			float dy = startY - my;
			//距離によって伸ばす距離を決める
			float dist = sqrt(dx * dx + dy * dy);
			
			float power = dist * 0.08;
			
			int percent = 0;

			if (isDrag) {
				//powerを100%で考えて10%ずつに分ける
				percent = (int)(power / 20 * 100);
			}
			//最大値の設定
			if (power > 100)power = 100;
			if (percent > 100)percent = 100;


			int gaugeX = 40;
			int gaugeY = 450;
			int gaugeW = 300;
			int gaugeH = 20;
			
			DrawBox(gaugeX, gaugeY, gaugeX + gaugeW, gaugeY + gaugeH, GetColor(80, 80, 80), TRUE);
			DrawBox(gaugeX, gaugeY, gaugeX + gaugeW * percent/100, gaugeY + gaugeH, GetColor(0, 255, 0),TRUE);
			DrawFormatString(gaugeX+100,gaugeY-25,GetColor(255,255,255),"POWER%d",percent);

			//----------------
			// 予測線
			//----------------

			for (int i = 0;i < 10;i++) {
				float t = i / 10.0f;
				int predictLength = power * 10.0f;
				float px = startX + dx *predictLength* t;
				float py = startY + dy *predictLength* t-sin(t*DX_PI_F)*5;

				DrawCircle(px, py, 3, GetColor(255, 255, 0),TRUE);
			}
		}

		//------------------
		// CLASSIC MODE
		//------------------

		if (shotMode == CLASSIC)
		{
			int white = GetColor(255, 255, 255);
			int gray = GetColor(80, 80, 80);
			int green = GetColor(0, 255, 0);
			int red = GetColor(255, 0, 0);

			DrawString(40, 420, "SPACE : SHOT", white);

			float startX = ballPos.x - cameraX;
			float startY = 400 - ballPos.z;

			//--------------------------
			//  角度インジケータ
			//--------------------------

			int angleCenterX = 190;
			int angleY = 500;

			DrawString(40, 390, "ANGLE", green);

			// 背景バー
			DrawBox(40, angleY, 340, angleY + 20, gray, TRUE);

			// 中心ライン（0度）
			DrawLine(angleCenterX, angleY, angleCenterX, angleY + 20, white);

			// 現在角度位置
			int angleX = angleCenterX + (int)classicAngle * 2;

			// インジケータ
			DrawBox(angleX - 3, angleY, angleX + 3, angleY + 20, green, TRUE);

			DrawFormatString(360, 500, white, "ANGLE %.0f", classicAngle);

			//-----------------------
			// パワーゲージ
			//-----------------------
			DrawString(40, 420, "POWER", white);

			DrawBox(40, 450, 340, 470, gray, TRUE);

			DrawBox(40, 450, 40 + classicPower * 3, 470, red, TRUE);

			DrawFormatString(360, 450, white, "POWER %.0f", classicPower);

			//--------------------------
			//  予測線（角度＋パワー）
			//--------------------------

			//角度とpower入力
			float angle = classicAngle * DX_PI_F / 180.0f;
			float power = classicPower / 100.0f * 25.0f;
			//ゴール位置の確認
			float goalDir = (goalX > ballPos.x) ? 1.0 : -1.0;
			float maxLen = power * 6.0f;

			float bestPower = -1;
			float bestDist = 999999.0f;

			for (int p = 1; p <= 100; p++)
			{
				Ball sim = ball;

				float testPower = p / 100.0f * 25.0f;

				sim.HitMove(CLASSIC,testPower * goalDir,angle,0,0);

				for (int i = 0; i < 400; i++)
				{
					sim.Update(stage, windPower);

					if (!sim.IsMove())
						break;
				}

				Vec3 pos = sim.GetPosition();

				float dx = pos.x - goalX;
				float dy = pos.z - goalY;

				float dist = sqrt(dx * dx + dy * dy);

				if (dist < bestDist)
				{
					bestDist = dist;
					bestPower = (float)p;
				}
			}

			if (bestDist < 20.0f)
			{
				int targetPowerX = 40 + (int)(bestPower * 3);

				DrawLine(targetPowerX,440,targetPowerX,480,GetColor(255, 255, 0));

				DrawTriangle(targetPowerX,430,targetPowerX - 8,440,targetPowerX + 8,440,GetColor(255, 255, 0),TRUE);
			}

			Ball preview = ball;

			preview.HitMove(CLASSIC,power * goalDir,angle,0,0);

			for (int i = 0; i < 80; i++)
			{
				preview.Update(stage, windPower);

				if (i % 3 == 0)
				{
					Vec3 p = preview.GetPosition();

					DrawCircle(p.x - cameraX,400 - p.z,3,GetColor(255, 255, 0),TRUE);
				}

				if (!preview.IsMove())
					break;
			}

			float dirX = goalDir * cos(angle);
			float dirY = sin(angle);
			float guideLen = 25.0f;
			//---------------------------
			//  方向確認ライン
			//---------------------------
			DrawLine(startX, startY, startX + dirX * guideLen, startY - dirY * guideLen, GetColor(255, 0, 0));

		}

	}
	else {
		DrawString(uiX+200,uiY+10, "Moveing", GetColor(255, 0, 0));
	}

	//---------------
	//　風
	//---------------

	int windX = 500;
	int windY = 40;
	
	int color = white;

	//2より高い場合と-2より低いとき赤、0.5より高い場合と-0.5より低いとき黄色、他白
	if (windPower > 2.0f||windPower<-2.0f) {
		color = GetColor(255, 0, 0);
	}
	else if(windPower>0.5||windPower<-0.5f){
		color = GetColor(255, 255, 0);
	}
	else {
		color = white;
	}
	//windPowerが0より大きい場合右向きに、0以下なら左向き
	if (windPower > 0) {

		DrawLine(windX, windY, windX + 50, windY, color);
		DrawTriangle(windX + 50, windY, windX + 40, windY - 5, windX + 40, windY + 5, color,TRUE);

	}
	else {

		DrawLine(windX + 50, windY, windX, windY, color);
		DrawTriangle(windX, windY,windX + 10, windY - 5,windX + 10, windY + 5,color, TRUE);

	}

	DrawFormatString(uiX + 470, uiY+30 , color, "WIND %.1f", fabs(windPower));
	


	for (int i = 0;i < EFFECT_MAX;i++) {
		if (!effects[i].use)continue;
		if (effects[i].type == EFFECT_GOAL) {
			DrawCircle(effects[i].x, effects[i].y, effects[i].size, GetColor(255, 255, 0), TRUE);
			DrawCircle(effects[i].x, effects[i].y, effects[i].size + 4, GetColor(255, 200, 0), FALSE);
		}
		else {
			DrawCircle(effects[i].x, effects[i].y, effects[i].size, effects[i].color);
		}
	
	}
	
	if (gameMode == SINGLE) {

		DrawFormatString(uiX + 15, uiY + 200, white, "PLAYER:%s", playerName);

	}
	else if (gameMode == ONLINE_TURN) {
		DrawFormatString(500, 80, black, isMyTurn ? "YOUR TURN" : "ENEMY TURN");

	}
	else {
		DrawFormatString(uiX + 15, uiY + 200, white, "TRUN:PLAYER%d", currentPlayer + 1);
	}
	
	


	if (isGoal) {

		const char* result;

		if (shotCount < par) {
			result = "★HOLE IN ONE★";
		}
		else if (shotCount == par) {
			result = "EAGLE!";
		}
		else {
			result = "BOGEY";
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		SetFontSize(64);
		DrawString(250, 140, "GOAL!", GetColor(255, 255, 0));

		SetFontSize(28);
		if (gameMode == SINGLE) {
			DrawFormatString(240, 220, GetColor(255, 255, 0), "%d STROKES", shotCount);
			DrawString(240, 250, result, GetColor(255, 255, 0));
		}
		else if(gameMode==LOCAL_VS){
			DrawFormatString(200, 220, GetColor(255, 255, 0), "P1:%d STROKES P2:%d STROKES", player[0].shotCount, player[1].shotCount);
		}
		else {
			DrawFormatString(200, 220, GetColor(255, 255, 0), "P1:%d STROKES P2:%d STROKES", player[myPlayer].shotCount, player[enemyPlayer].shotCount);
		}
		
		SetFontSize(16);

	}

}
// ゴール判定
void PlayScene::CheckHitGoal() {
	
	if (gameMode == ONLINE_TURN) {

		Ball& ball = (gameMode == ONLINE_TURN) ? player[myPlayer].ball : player[enemyPlayer].ball;
		Vec3 ballPos = ball.GetPosition();
		//ゴールの中心取得
		float holeX = holeIndex * stage.GetSize() + stage.GetSize() / 2;
		float holeY = stage.GetGroundY(holeX);

		//ボールとゴールの距離
		float dx = ballPos.x - holeX;
		float dz = ballPos.z - holeY;
		float dist = sqrt(dx * dx + dz * dz);
		//水平速度
		float speed = fabs(ball.GetSpeedX());
		//ゴール条件
		if (dist < goalRadius&&!player[myPlayer].finished) {

			PlaySoundMem(capInSE, DX_PLAYTYPE_BACK);

			if (!goalEffectPlayer[myPlayer]) {

				Vec3 pos = ball.GetPosition();

				CreatEffect(pos.x - cameraTarget->GetPosition().x + 200, 400 - pos.z, GetColor(255, 255, 0), 60, EFFECT_GOAL);

				goalEffectPlayer[myPlayer] = true;
			}


				player[myPlayer].finished = true;

				Packet gamePacket{};
				gamePacket.type = PACKET_GAME;
				gamePacket.game.finished = true;
				gamePacket.game.shotCount = player[myPlayer].shotCount;
				netWork->Send(&gamePacket, sizeof(gamePacket));
	
				//isMyTurn = false;
			
		}

		if (player[0].finished && player[1].finished) {
			if (isHost) {

				Packet gamePacket{};
				gamePacket.type = PACKET_GAME;
				gamePacket.game.nextHole = true;
				netWork->Send(&gamePacket, sizeof(gamePacket));

			}
			isGoal = true;
			goalTime = 0;
		}
	}
	else {

		Ball& ball = player[currentPlayer].ball;
		Vec3 ballPos = ball.GetPosition();
		//ゴールの中心取得
		float holeX = holeIndex * stage.GetSize() + stage.GetSize() / 2;
		float holeY = stage.GetGroundY(holeX);

		//ボールとゴールの距離
		float dx = ballPos.x - holeX;
		float dz = ballPos.z - holeY;
		float dist = sqrt(dx * dx + dz * dz);
		//水平速度
		float speed = fabs(ball.GetSpeedX());
		//ゴール条件
		if (dist < goalRadius) {

			PlaySoundMem(capInSE, DX_PLAYTYPE_BACK);
			Vec3 pos = ball.GetPosition();

			CreatEffect(pos.x - cameraTarget->GetPosition().x + 200, 400 - pos.z, GetColor(255, 255, 0), 60, EFFECT_GOAL);


			if (gameMode == SINGLE) {
				if (!goalEffectPlayer[0]) {
					Vec3 pos = ball.GetPosition();

					CreatEffect(pos.x - cameraTarget->GetPosition().x + 200, 400 - pos.z, GetColor(255, 255, 0), 60, EFFECT_GOAL);

					goalEffectPlayer[0] = true;
				}
				isGoal = true;
				goalTime = 0;
				return;
			}

			if (!goalEffectPlayer[currentPlayer]) {
				Vec3 pos = ball.GetPosition();

				CreatEffect(pos.x - cameraTarget->GetPosition().x + 200, 400 - pos.z, GetColor(255, 255, 0), 60, EFFECT_GOAL);

				goalEffectPlayer[currentPlayer] = true;
			}

			player[currentPlayer].finished = true;

	}

		if (player[0].finished && player[1].finished) {
			isGoal = true;
		}

	}
	
	

}
// 打数加算
void PlayScene::ShotCount() {
	shotCount++;

	if (gameMode == ONLINE_TURN) {
		player[myPlayer].shotCount++;
	}
	else {
		player[currentPlayer].shotCount++;
	}

	
}
// 背景
void PlayScene::BackGround() {
	//--------------------------------
	//背景
	//--------------------------------
	
	switch (weather) {

	case SUNNY:
		// 晴れ
	//空
		DrawBox(0, 0, 1280, 480, GetColor(120, 200, 255), TRUE);
		//雲
		DrawCircle(120, 120, 40, GetColor(255, 255, 255));
		DrawCircle(160, 120, 50, GetColor(255, 255, 255));
		DrawCircle(200, 120, 40, GetColor(255, 255, 255));
		DrawCircle(480, 200, 40, GetColor(255, 255, 255));
		DrawCircle(520, 200, 50, GetColor(255, 255, 255));
		DrawCircle(560, 200, 40, GetColor(255, 255, 255));
		//地面
		DrawBox(0, 410, 1280, 720, GetColor(80, 180, 80), TRUE);
		break;

	case SUNSET:
		//夕日
		//空
		DrawBox(0, 0, 1280, 450, GetColor(255, 170, 100), TRUE);
		//地面
		DrawBox(0, 410, 1280, 720, GetColor(120, 100, 60), TRUE);
		//太陽
		DrawCircle(480, 120, 60, GetColor(255, 220, 120), TRUE);
		break;

	case RAIN:
		//雨
		//空
		DrawBox(0, 0, 1280, 480, GetColor(80, 80, 100), TRUE);
		//雨
		for (int i = 0;i < 200;i++) {
			int x = GetRand(1280);
			int y = GetRand(720);

			DrawBox(x, y, x - 5, y + 15, GetColor(180, 180, 255), TRUE);
		}
		break;

	case NIGHT:
		//夜空
		//空
		DrawBox(0, 0, 1280, 720, GetColor(10, 10, 40), TRUE);
		//星Into(),Update()の一番下にもある
		for (int i = 0;i < 100;i++) {

			float t = (starTimer + stars[i].time) * 0.3;

			int bright = 180 + sin(t) * 75;
			DrawPixel(stars[i].x, stars[i].y, GetColor(bright, bright, bright));
		}
		//月
		DrawCircle(100, 100, 50, GetColor(255, 255, 200), TRUE);
		DrawCircle(120, 90, 50, GetColor(10, 10, 40));
		break;

	case SNOW:
		//雪
		//空
		DrawBox(0, 0, 1280, 720, GetColor(220, 230, 255), TRUE);
		//雪
		for (int i = 0;i < 150;i++) {
			DrawCircle(GetRand(1280), GetRand(720), 2, GetColor(255, 255, 255));
		}
		//地面
		DrawBox(0, 450, 1280, 720, GetColor(240, 240, 240), TRUE);
		break;

	}

	
}
// コース読み込み
void PlayScene::LodeCourse(int course) {

	const char* fileName[] = {
		"data/course1.txt",
		"data/course2.txt",
		"data/course3.txt",
		"data/course4.txt",
	
	};

	stage.Lode(fileName[course]);

	holeIndex = stage.FindHole();

	goalX = holeIndex * stage.GetSize() + stage.GetSize() / 2;

	goalY = stage.GetGroundY(goalX);
	
	isGoal = false;

}
// Effect描画
void PlayScene::CreatEffect(float x,float y,int color,int count,int type) {
	for (int i = 0;i < EFFECT_MAX;i++) {
		if (!effects[i].use) {

			effects[i].type = type;

			effects[i].use = true;

			effects[i].x = x;
			effects[i].y = y;

			effects[i].vx = GetRand(100) / 50.0f - 1.0f;
			effects[i].vy = -(GetRand(100) / 50.0f);

			effects[i].size = 2 + GetRand(4);

			effects[i].life = 20 + GetRand(20);

			effects[i].color = color;

			count--;
			if (count < 0) {
				break;
			}

		}
	}
}
// 引っ張るモードの処理
void PlayScene::UpdateDrag(int cameraX) {
	Ball& ball = player[currentPlayer].ball;
	if (ball.IsMove()) return;

	int mx, my;
	GetMousePoint(&mx, &my);

	Vec3 ballPos = ball.GetPosition();

	float ballPosX = ballPos.x - cameraX;
	float ballPosY = 400 - ballPos.z;

	float dx = mx - ballPosX;
	float dy = my - ballPosY;

	float dist = sqrt(dx * dx + dy * dy);



	if (GetMouseDown()) {
		if (dist <  24.0f) {
			isDrag = true;
		}
	}

	if (isDrag) {
		if (GetMouseUp()) {
			//パワー（長さ）
			float power = dist * 0.08;

			if (power < 1.0f)power = 1.0f;
			if (power > 25.0f)power = 25.0f;

			// 角度（ラジアン）
			float angle = atan2(ballPosY - my, ballPosX - mx);

			float offsetY = my - ballPosY;
			float high = offsetY * 0.03f;

			if (high < 0.0f)high = 0.0f;
			if (high > 10.0f)high = 10.0f;

			

			ball.HitMove(DRAG,power, angle, high, offsetY);
			PlaySoundMem(hitSE, DX_PLAYTYPE_BACK);
			ShotCount();
			

			isDrag = false;
		}
	}
	
}
// タイミングモードの処理
void PlayScene::UpdateClassic()
{
	Ball& ball = player[currentPlayer].ball;
	if (ball.IsMove()) return;

	//-------------------
	// 角度決定
	//-------------------
	if (classicState == CLASSIC_ANGLE)
	{
		float angleSpeed = 1.0f;

		if (ball.IsBunker())
		{
			angleSpeed = 3.0f;
		}

		if (angleUp)
			classicAngle += angleSpeed;
		else
			classicAngle -= angleSpeed;

		if (classicAngle >= 90)
			angleUp = false;

		if (classicAngle <= -30)
			angleUp = true;

		if (IsPushKey(KEY_INPUT_SPACE))
		{
			classicState = CLASSIC_POWER;
		}
	}

	//-------------------
	// パワー決定
	//-------------------
	else if (classicState == CLASSIC_POWER)
	{
		float powerSpeed = 1.0f;

		if (ball.IsBunker())
		{
			powerSpeed = 3.0f;
		}

		if (powerUp)
			classicPower += powerSpeed;
		else
			classicPower -= powerSpeed;

		if (classicPower >= 100)
			powerUp = false;

		if (classicPower <= 0)
			powerUp = true;

		if (IsPushKey(KEY_INPUT_SPACE))
		{
			Vec3 ballPos = ball.GetPosition();

			float dir = (goalX > ballPos.x) ? 1.0f : -1.0f;

			float power =fabs(classicPower) / 100.0f * 25.0f;

			float angle =classicAngle * DX_PI_F / 180.0f;

			//ghostRecord.push_back({ ballPos.x, ballPos.z, power, angle, high, offsetY, CLASSIC });

			ball.HitMove(CLASSIC,power*dir,angle,0,0);
			PlaySoundMem(hitSE, DX_PLAYTYPE_BACK);
			ShotCount();

			//----------------
			// リセット
			//----------------
			classicPower = 0;
			classicAngle = 0;

			powerUp = true;
			angleUp = true;

			classicState = CLASSIC_ANGLE;
		}
	}
}
// 背景の情報
void PlayScene::SetWeather(WEATHER next) {
	weather = next;
}

void PlayScene::SetGameMode(GameMode mode) {
	gameMode = mode;
	
}

void PlayScene::ChangePlayer() {
	
	
	do {
		currentPlayer = (currentPlayer + 1) % 2;
	}
	while (player[currentPlayer].finished &&!(player[0].finished && player[1].finished));
	/*currentPlayer = (currentPlayer + 1) % 2;
	if (player[currentPlayer].finished) {
		currentPlayer = (currentPlayer + 1) % 2;
	}*/

}
// ショット数受け取り
int PlayScene::GetShotCount()const {

	return shotCount;
}
// 全コース合わせたショット数受け取り
int PlayScene::GetTotalShot()const {
	return totalShot;
}

int PlayScene::GetTotalShotP1()const {
	return totalShotP1;
}

int PlayScene::GetTotalShotP2()const {
	return totalShotP2;
}

int PlayScene::GetCurrentCount(int index)const {
	return player[index].shotCount;
}

GameMode PlayScene::GetGameMode()const {
	return gameMode;
}
// コースごとのショット数受け取り
const int* PlayScene::GetHoleScore()const {
	return holeScore;
}

const int* PlayScene::GetHoleScoreP1()const {
	return holeScoreP1;
}

const int* PlayScene::GetHoleScoreP2()const {
	return holeScoreP2;
}
// グランプリ判断
bool PlayScene::GetIsGrandPrix()const {
	return isGrandPrix;
}

void PlayScene::SetNetWork(Network* net) {
	netWork = net;
}

void PlayScene::SetIsHost(bool host) {
	isHost = host;
}

Ball& PlayScene::GetActiveBall() {
	if (gameMode == ONLINE_TURN)
		return player[myPlayer].ball;

	return player[currentPlayer].ball;
}

int PlayScene::GetMyShot() {
	return player[myPlayer].shotCount;
}

int PlayScene::GetEnemyShot() {
	return player[enemyPlayer].shotCount;
}
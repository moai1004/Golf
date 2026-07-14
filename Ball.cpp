#include<DxLib.h>
#include <cmath>
#include"Ball.h"
#include"input.h"


//ボールリセット
void Ball::Reset() {
	//位置
	position.x = 100;//初期x位置
	position.y = 400;//描写用Y
	position.z = stage.GetGroundY(position.x);  //高さ

	shotPos = position;

	radius = 10;

	//速度リセット
	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;

	//状態リセット
	hitSwing = false;
	isFlying = false;
	isBunker = false;

	justLand = false;
	landType = GRASS;

}

//毎フレーム更新
void Ball::Update(Stage& stage,float wind) {
	
	justLand = false;


	//現在の地面のタイプ取得
	int type = stage.GetType(position.x);

	float groundY = stage.GetGroundY(position.x);//地面の高さ

	

	if (type == HOLE&&!isFlying) {
		
	
		float holecenter = floor(position.x / 64) * 64 + 32;
		position.x += (holecenter - position.x)*0.1f;

		if (fabs(velocity.x) < 0.5f) {
			velocity.x = 0;
			hitSwing = false;
			
		}
	}
	if (IsPushKey(KEY_INPUT_M)) {
		Reset();
	}
	
	//================
	//空中時の挙動
	//================
	if (isFlying) {
		
		float windPower = wind;
		if (!isFlying) {
			windPower *= 0.2f;
		}

		velocity.x += windPower*0.01f;
		//重力
		velocity.z += gravity *0.3f;
		//空気抵抗
		velocity.x *= ar*0.9995f;
		velocity.z *= ar*0.998f;
		//次の移動予定位置
		float nextX = position.x + velocity.x;
		//現在地と次の地面の高さ取得
		float nowGround = stage.GetGroundY(position.x);
		float nextGround = stage.GetGroundY(nextX);
		//段差量
		float Wall = nextGround - nowGround;
		//地面の角度を計算
		float angle = atan2(fabs(Wall), fabs(velocity.x)) * 180.0f / DX_PI_F;
		//次フレームで地面にめり込むか
		bool hitGround = position.z < nextGround;
		//急な壁に当たった時
		if (hitGround&&Wall > 10 && angle > 60) {

			velocity.x *= -0.5f;
			//壁の前で止める
			nextX = position.x;
		}


		//位置更新
		//x=x+vx
		//y=y+vy
		position.x = nextX;
		position.z += velocity.z;
		//地面
		if (position.z <= groundY && velocity.z <= 0) {
			position.z = groundY;
			
			justLand = true;
			landType = type;
			
			if (velocity.z < 0.1f) {
				velocity.z = 0;
				isFlying = false;
			}
			else {
				//バウンド
			    //vy=-e*vy

				if (type == GRASS) velocity.z *= -0.6f;
				else if (type == BUNKER) velocity.z *= -0.2f;
			}
		}
	}

	//==============
	//地面時
	//==============
	else {
		
		float dx = 4.0f;
		float slope = (stage.GetGroundY(position.x+dx)-stage.GetGroundY(position.x))/dx;
		if (fabs(slope) < 0.005f)slope = 0;
		//velocity.x-=slope*0.03f;
		debugSlope = slope;
		
		
		

		//バンカー wasFly&&
		if (type == BUNKER && /*wasFly &&*/ !isFlying)
		{
			isBunker = true;
			velocity.x *= 0.6f;
		}
		//池
		else if (type == WATER) {
			
			position = shotPos;

			velocity.x = 0;
			velocity.z = 0;

			isBunker = false;
			return;
		}
		//位置更新
		else {
		 velocity.x -= slope * 0.2f;
			velocity.x *= 0.97f;
			isBunker = false;  
		}
		

	 //移動量を取得
	 float move = velocity.x;
	
	 int count = (int)fabs(move);
	 //最低1回は判定する
	 if (count < 1)count = 1;
	 //1回ごとの移動量
	 float slopeMove = move / count;

	 //----------------------
	 //地形との細かい衝突判定
	 //----------------------

	 for (int i = 0;i < count;i++) {

		 float nextX = position.x + slopeMove;
		 //現在地と次の地面の高さ取得
		 float nowGround = stage.GetGroundY(position.x);
		 float nextGround = stage.GetGroundY(nextX);
		 //段差量
		 float step = nextGround - nowGround;
		 //地面の角度を計算
		 float angle = atan2(fabs(step), fabs(slopeMove)) * 180.0f / DX_PI_F;

		 //-------------------
		 //急な壁に衝突
		 //-------------------
		 if (fabs(angle) > 60.0f&&step>0) {
			 //反射して跳ね返る
			 velocity.x *= -0.5f;

			 isFlying = true;
			 //少し上に跳ね上がる
			 velocity.z = 2.0f;

			 break;

		 }
		 else if (step <-1.0f&&fabs(velocity.x)>6.0f) {
			 isFlying = true;

			 velocity.z = fabs(velocity.x) * 0.12f;

			 position.x = nextX;
			 break;
		 }
		 
		
			 position.x = nextX;

			 if (!isFlying) {
				 float targetGround = nextGround;
				 float diff = targetGround - position.z;
				 if (diff <=0.0f&&fabs(velocity.x)<4.0f) {
					 position.z = targetGround;
				 }
				 else {
					 if (diff > 1.0) {
						 isFlying = true;
					 }
					 

					 velocity.z = 0;
					// velocity.z = velocity.x * fabs(slope) * 0.3f;
				 }
			 }
		 
	 }

		//完全停止
		if (!isFlying && std::abs(velocity.z) < 0.1 && std::abs(velocity.x) < 0.1f) {
			velocity.x = 0;
			velocity.z = 0;
			hitSwing = false;
		
			//isBunker = false;
		}
	}

	if (type == HOLE && !isFlying) {
		float holeCenter = floor(position.x / size) * size + size / 2;

		//中心に寄せる
		//position.x += (holeCenter - position.x) * 1.0f;
		
		//下に引っ張る
		velocity.z -= 1.5f;

		//横速度減衰
		velocity.x *= 0.1f;
	
		//完全停止
		if (fabs(velocity.x) < 0.1f) {
			velocity.x = 0;

		}
	}

	//打ってなかったら何もしない
	if (!hitSwing) return;
	
	prevGround = groundY;
	debugGround = groundY;

	wasFly = isFlying;

	int stageWight = W * stage.GetSize();

	if (position.x < 0) {

		position.x = 0;

		velocity.x *= -0.5f;

		velocity.z = 2.0f;

		isFlying = true;
	}
	else if (position.x > stageWight) {

		position.x = stageWight;

		velocity.x *= -0.5f;

		velocity.z = 2.0;

		isFlying = true;
	}
}

//ボール描写
void Ball::Draw(int cameraX)const {
	
	//カメラ補正
	int drawX = (int)position.x-cameraX;
	//高さY
	int drawY = 400- (int)position.z;

	DrawCircle(drawX, drawY, radius, GetColor(255, 255, 255), TRUE);

	DrawCircle(drawX, drawY, radius, GetColor(0,0,0), FALSE);
}





//ボールの現在地を渡す
Vec3 Ball::GetPosition()const {
	return position;
}
//ボールの大きさを渡す
int Ball::GetRadius()const {
	return radius;
}

int Ball::GetSpeedX() const {
	return velocity.x;
}

int Ball::GetLandType()const {
	return landType;
}

bool Ball::GetJustLand()const {
	return justLand;
}

//ボールの状況
bool Ball::IsMove()const {
	return hitSwing;
}

//打たれた時の初速設定
void Ball::HitMove(ShotMode mode,float power,float angle,float high,float offsetY) {

	shotPos = position;

	lastShotMode = mode;
	//打点保管
	hitoffsetY = offsetY;

	switch(mode) {
	case SWING:
	//水平、垂直方向の計算
	//vx=v*cos(0)
	//vy=v*sin(0)
		velocity.x = power * cos(angle) * 3.0f;
		velocity.z = power * sin(angle) * 1.8f;
		//打点での高さ補正
		velocity.z += high;
		break;
	case DRAG:
		velocity.x = power * cos(angle) * 2.5f;

		// 高さはドラッグ量だけで決める
		velocity.z = power*0.3f+high;
		break;
	case CLASSIC:

		float dir = (power >= 0) ? 1.0 : -1.0;
		float abspower = abs(power);

		velocity.x = dir*abspower * cos(angle) * 2.7f;
		velocity.z = abspower * sin(angle) * 1.0f;
		velocity.z += high;
		break;
	}


	//状態変更
	isFlying = true;
	hitSwing = true;
}

bool Ball::IsBunker() const {
	return isBunker;
}



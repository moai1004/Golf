#include<DxLib.h>
#include"SwingInput.h"

// =========================
// 入力更新（毎フレーム）
// =========================
void SwingInput::Update(int cameraX) {



	GetMousePoint(&mx,&my);       //今のマウス座標を取得

	//左クリック中なら
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		trail.push_back({ mx+cameraX,my });   //今の座標を軌跡に追加

		if (trail.size() > maxTrajectory) {
			trail.erase(trail.begin());    //50個を超えたら一番古い点を削除
		}
	}
	else {
		Reset();    //ボタンを離したら軌跡を全部消す
		alreadyHit = false;// 再度ヒットできるようにする
	}
	
}

// =========================
// 軌跡の描画
// =========================
void SwingInput::Draw(int cameraX)const{

	//2点目から最後まで順番に見る
	for (int i = 1;i < trail.size();i++) {
		DrawLine(trail[i-1].x-cameraX, trail[i-1].y, trail[i].x-cameraX,trail[i].y, GetColor(0,0,0), TRUE);    //1個前の点と今の点を線で結ぶ
	}



}

// =========================
// リセット
// =========================
void SwingInput::Reset() {

	isHit = false;// ヒット状態解除
	trail.clear();// 軌跡を全削除
	
}

// =========================
// ボールに当たったか判定
// =========================
bool SwingInput::CheckHitBall(const Ball& ball,Vec3& hitpos) {

	Vec3 ballPos = ball.GetPosition();
	int radius = ball.GetRadius();
	// ボールの画面座標（円の中心）
	Vec3 center;
	center.x = ballPos.x;
	center.y = 400 - ballPos.z;

	// まだ当たっていない場合だけチェック
	if (!alreadyHit) {

		// 軌跡の「線分ごと」にチェック
		for (int i = 1;i < (int)trail.size();i++) {

			Vec3 A, B;
			// 線分の始点・終点
			A.x = trail[i - 1].x;
			A.y = trail[i - 1].y;

			B.x = trail[i].x;
			B.y = trail[i].y;
			// 線分 vs 円の当たり判定
			if (LineCheckHit(A, B, center, radius,hitpos)) {

				alreadyHit = true;// 一度だけヒット
				return true;

			}
		}
	}
	return false;
}

// =========================
// スイング方向を計算
// =========================
Vec3 SwingInput::SetTrail()const {

	Vec3 dir;
	dir.x = 0;
	dir.y = 0;
	dir.z = 0;
	
	int n = trail.size();
	if (n < 2) return dir;
	// 最新の数点だけ使う（ブレ防止）
	int count = 12;
	if (n < count)count = n;
	// 移動量を足し合わせる
	for (int i = n - count;i < n-1;i++) {
		dir.x += trail[i + 1].x - trail[i].x;
		dir.y += trail[i + 1].y - trail[i].y;
	}


	return dir;
}

// =========================
// 線分と円の当たり判定
// =========================
bool SwingInput::LineCheckHit(Vec3 A,Vec3 B,Vec3 C,float radius,Vec3& hitpos) {
	// 線分の長さ
	float length = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
	// 分割数（細かくチェックするため）
	float step = 3.0;
	int split = (int)(length/step);
	if (split < 1)split = 1;
	// 線分を細かく分割してチェック
	for (int i = 0;i <=split;i++) {

		float t = (float)i / split;
		// A→B のベクトル
		float ABx = (float)(B.x - A.x);
		float ABy = (float)(B.y - A.y);
		
		float ACx = (float)(C.x - A.x);
		float ACy = (float)(C.y - A.y);

		// 分割点（線上の点）
		float px = A.x + ABx * t;
		float py = A.y + ABy * t;
		// 円との距離
		float dx = px - C.x;
		float dy = py - C.y;

		float r = radius * 2.0f;// 判定を少し広めに
		// 円の中に入ったらヒット
		if (dx * dx + dy * dy <= r * r) {
			hitpos.x = px;// 当たった位置
			hitpos.y = py;
			return true;
		}
	}
	return false;
}

float SwingInput::GetPower()const {

	Vec3 dir = SetTrail();

	return sqrt(dir.x * dir.x + dir.z * dir.z) * 0.25f;
}
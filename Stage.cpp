#include<fstream>
#include<string>
#include<sstream>
#include<DxLib.h>
#include"Stage.h"

// =========================
// ステージ描画
// =========================
void Stage::Draw(int cameraX) {

	
	//全地形取得
	for (int i = 0;i < W - 1;i++) {
	
		//ワールドから画面座標
		int x0 = i * size - cameraX;
		int x1 = (i + 1) * size - cameraX;
		//高さを描写
		int y0 = 400 - high[i];
		int y1 = 400 - high[i + 1];
		//地形ごとに色を変える
		int color = GetColor(0, 255, 0);
		if (type[i] == BUNKER)color = GetColor(255, 255, 0);
		if (type[i] == WATER)color = GetColor(0, 0, 255);
		if (type[i] == HOLE)color = GetColor(255, 0, 0);

		DrawLine(x0, y0+10, x1, y1+10, color,5.0f);
	}
	

}

// =========================
// マップ読み込み
// =========================

void Stage::Lode(const char* filename) {
	//courseファイル読み込み
	std::ifstream file(filename);
	std::string  str;
	//数値の0を地形タイプに変換
	std::getline(file, str);
	for (int i = 0;i < W && i < str.size();i++) {
		type[i] = str[i] - '0';
	}
	//同じcourseファイルから高さを取得
	std::getline(file, str);

	std::stringstream ss(str);
	//累計の高さ
	float currentHigh = 0;

	for (int i = 0;i < W;i++) {

		std::string num;
		
		if (!std::getline(ss, num, ',')) {
			break;
		}

		float diff = std::stof(num);
		//高さを差分で加算して実際の高さを出す
		currentHigh += diff;
		//高さに倍率をかける
		high[i] = currentHigh * 20;

	}
	
	
}



// =========================
// 指定Xの地面タイプ取得
// =========================
int Stage::GetType(float worldX) {
	// ワールド座標 → マップ座標
	int x = (int)(worldX / size);

	//配列範囲外を防止
	x = max(0, min(W - 1, x));
	debugtype = type[x];
	
	return type[x];

}
// =========================
// 指定Xの地面の高さ取得
// =========================
float Stage::GetGroundY(float worldX) {

	// ワールド座標 → 地形座標
	float fx = worldX / size;	

	//補間用の前後を取得
	int x0 = (int)fx;
	int x1 = x0 + 1;
	
	x0 = max(0, min(W - 1, x0));
	x1 = max(0, min(W - 1, x1));
	//補間係数
	float t = fx - x0;
	
	//線形補間
	return high[x0]+(high[x1]-high[x0])*t;
	
}

//地形のサイズ取得
int Stage::GetSize()const {
	return size;
}

//ゴールの位置を探す
int Stage::FindHole()const {
	for (int i = 0;i < W;i++) {
		if (type[i] == HOLE) return i;
	}
	return -1;
}

//ステージタイプを取得
int Stage::GetTypeIndex(int index) const {

	index = max(0, min(W - 1, index));

	return type[index];
}
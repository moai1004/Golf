#pragma once
#define H (64)
#define W (200)

enum {
	EMPTY=0,
	GRASS=1,
	BUNKER=2,
	WATER=3,
	HOLE=4

};



class Stage {
private:

	int debug = 0;
	int lodeType = 0;
	int type[W];
	float high[W];

	//int map[H][W];
	int debugtype = 0;
	int size = 32;

public:

	

	void Into();
	void Draw(int cameraX);
	void Lode(const char* filename);
	//void LodeHeigh(const char* filename);
	int GetType(float worldX);
	int GetSize()const;
	int FindHole()const;
	int GetTypeIndex(int index) const;
	float GetGroundY(float worldX);
	
};
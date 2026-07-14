#pragma once


struct TURNDate {
	bool isPlayer1Turn;
};

struct GameDate {

	int shotCount;
	int totalShot;

	float wind;

	bool finished;

	bool nextHole = false;

};

struct LobbyDate {
	int mode;
	bool start;
};

struct courseSelectDate {
	int course;
	bool start;
};

struct ShotDate {

	float power;
	float angle;
	float high;
	float offsetY;

	bool endTurn;

	int shotCount;
};

enum PacketType {
	PACKET_SHOT,
	PACKET_GAME
};

struct Packet {
	int type;

	union {
		ShotDate shot;
		GameDate game;
	};
};


struct BallDate {

	float x;
	float z;

	float vx;
	float vz;

	int shot;

};

struct ResultSelectDate {

	int shot;
	
	bool rematch;
	bool exit;

};



enum ResultType {
	WIN,
	DROW,
	LOSE
};

enum ResultSelect {
	REMATCH,
	EXIT
};



class Network {
private:

	int socketHandle = -1;

	//IPDATA connectIP;
	int connectPort = 0;

public:

	bool Host(int port);

	bool Connect(const char* ip, int port);

	void Send(void* data, int size);

	int Receive(void* data, int size);

	void Close();

};
#include<DxLib.h>
#include<cstring>
#include"Input.h"

static char now[256];
static char old[256];

static bool oldMouse = false;
static bool nowMouse = false;

void UpdateKey() {


	memcpy(old, now, 256);

	GetHitKeyStateAll(now);

}

bool IsPushKey(int key) {

	return now[key] && !old[key];
}

bool IsHoldKey(int key) {
	return now[key];
}

void UpdateInput() {
	oldMouse = nowMouse;

	nowMouse = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
}

bool GetMousePress() {
	return nowMouse;
}

bool GetMouseDown() {
	return !oldMouse && nowMouse;
}

bool GetMouseUp() {
	return oldMouse && !nowMouse;
}
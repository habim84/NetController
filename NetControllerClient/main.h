#pragma once
#include <Xinput.h>

#ifndef XCONTROLLER_KEYS
#define XCONTROLLER_KEYS
namespace xcon_keys {
	const char* KEY_A = "A";
	const char* KEY_B = "B";
	const char* KEY_X = "X";
	const char* KEY_Y = "Y";
	const char* KEY_UP = "DU";
	const char* KEY_DOWN = "DD";
	const char* KEY_LEFT = "DL";
	const char* KEY_RIGHT = "DR";
	const char* KEY_TOGGLE_R = "TR";
	const char* KEY_TOGGLE_L = "TL";
	const char* KEY_RT = "RT";
	const char* KEY_RB = "RB";
	const char* KEY_LT = "LT";
	const char* KEY_LB = "LB";
	const char* KEY_SELECT = "S";
	const char* KEY_PAUSE = "P";
	const char* KEY_HOME = "H";
};
#endif


#ifndef PLAYER_ID
#define PLAYER_ID 1
#endif

#ifndef LISTENER_RATE
#define LISTENER_RATE 50
#endif

#ifndef XCONTROLLER
#define XCONTROLLER
class XController
{
private:
	int controllerId;
	XINPUT_STATE state;
	float deadzoneX;
	float deadzoneY;

public:
	XController() : deadzoneX(0.05f), deadzoneY(0.02f) {}
	XController(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}

	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;
	float leftTrigger;
	float rightTrigger;

	int GetPort();
	XINPUT_GAMEPAD* GetState();
	bool CheckConnection();
	bool Refresh();
	bool IsPressed(WORD);
};
#endif
#include <Windows.h>
#pragma comment (lib, "xinput.lib")
#include <iostream>
#include <string>
#include <minwinbase.h>
#include <winerror.h>
#include "main.h"


int XController::GetPort()
{
	return controllerId + 1;
}

XINPUT_GAMEPAD* XController::GetState()
{
	return &state.Gamepad;
}

bool XController::CheckConnection()
{
	int cid = -1;
	for (DWORD i = 0; i < XUSER_MAX_COUNT && cid == -1; i++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			cid = i;
		}
	}

	if (cid == -1)
	{
		//std::cout << "No Controllers Connected. Please connect a controller and re-run this program.";
		return false;
	}
	else {
		//std::cout << "Controller ID: " << controllerId;
		controllerId = cid;
		return true;
	}
}

bool XController::Refresh()
{
	if (controllerId == -1)
	{
		XController::CheckConnection();
	}
	else {
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(controllerId, &state) != ERROR_SUCCESS)
		{
			controllerId = -1;
			return false;
		}
		float normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

		leftStickX = (abs(normLX) < deadzoneX ? 0 : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
		leftStickY = (abs(normLY) < deadzoneY ? 0 : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));

		if (deadzoneX > 0) leftStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) leftStickY *= 1 / (1 - deadzoneY);

		float normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);

		rightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
		rightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));

		if (deadzoneX > 0) rightStickX *= 1 / (1 - deadzoneX);
		if (deadzoneY > 0) rightStickY *= 1 / (1 - deadzoneY);

		leftTrigger = (float)state.Gamepad.bLeftTrigger / 255;
		rightTrigger = (float)state.Gamepad.bRightTrigger / 255;

		return true;
	}
	return false;
}

bool XController::IsPressed(WORD button)
{
	return (state.Gamepad.wButtons & button) != 0;
}



#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <InitGuid.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")


class InputDirectInput8
{
public:
	struct DeviceMouse
	{
		LPDIRECTINPUTDEVICE8 lpdid = nullptr;
		DIMOUSESTATE state = { 0 };
	};
	struct DeviceKeyboard
	{
		LPDIRECTINPUTDEVICE8 lpdid = nullptr;
		BYTE state[256] = { 0 };	// diKeys[DIK_ESCAPE] & 0x80
	};
	struct DeviceJoystick
	{
		LPDIRECTINPUTDEVICE8 lpdid = nullptr;
		DIJOYSTATE state = { 0 };
	};

protected:
	LPDIRECTINPUT8 lpdi = nullptr;

	DeviceMouse deviceMouse;
	DeviceKeyboard deviceKeyboard;
	std::vector<DeviceJoystick> deviceJoysticks;

	bool acquired = false;
	
public:
	BOOL DIEnumDevicesCallbackJoystick(LPCDIDEVICEINSTANCE lpddi);
	BOOL DIEnumDeviceObjectsCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi);

	bool acquireDevices();
	bool releaseDevices();


public:
	InputDirectInput8();
	virtual ~InputDirectInput8();

	int refreshDevices();

	void poll();

	size_t getJoystickCount() const;

	const DIMOUSESTATE& getMouseState() const;
	const BYTE* getKeyboardState() const;
	const DIJOYSTATE& getJoystickState(size_t idx) const;

public:
	static InputDirectInput8& inst();
};

#endif
#pragma once
#include<Windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include <dinput.h>
#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024
class KeyEventHandler
{
public:
	virtual void KeyState(BYTE *state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};
typedef KeyEventHandler * LPKEYEVENTHANDLER;
class Game
{
private:
	static Game *_instance;
	HWND hWnd;
	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddv = NULL;
	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;
	LPDIRECTINPUT8       di;		
	LPDIRECTINPUTDEVICE8 didv;
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		
	LPKEYEVENTHANDLER keyHandler;
	BYTE  keyStates[256];
public:
	void InitKeyboard(LPKEYEVENTHANDLER handler);
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom);
	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }
	static Game * GetInstance();
	~Game();
};


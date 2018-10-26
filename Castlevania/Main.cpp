#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Simon.h"
#include"Animation.h"
#include"Animations.h"
#include"Sprite.h"
#include"Sprites.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"Castlevania"
#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define MAX_FRAME_RATE 90
#define ID_TEX_SIMON 0
#define ID_TEX_SIMON1 1
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20

Game *game;
Simon *simon;
class SampleKeyHander : public KeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

SampleKeyHander * keyHandler;

void SampleKeyHander::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_SPACE:
		simon->SetState(SIMON_STATE_JUMP);
		break;
	}
}

void SampleKeyHander::OnKeyUp(int KeyCode)
{
}

void SampleKeyHander::KeyState(BYTE *states)
{
	if (game->IsKeyDown(DIK_RIGHT))
	{
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else
	{
		simon->SetState(SIMON_STATE_IDLE);
	}
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
void LoadResources()
{
	Textures * textures = Textures::GetInstance();
	textures->Add(ID_TEX_SIMON, L"simon.png", D3DCOLOR_XRGB(176, 224, 248));
	Textures * textures1 = Textures::GetInstance();
	textures1->Add(ID_TEX_SIMON1, L"simon1.png", D3DCOLOR_XRGB(176, 224, 248));
	Sprites * sprites = Sprites::GetInstance();
	Animations * animations = Animations::GetInstance();
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	LPDIRECT3DTEXTURE9 texSimon1 = textures->Get(ID_TEX_SIMON1);
	//right
	sprites->Add(10001, 588, 3, 616, 60, texSimon);
	sprites->Add(10002, 619, 1, 641, 60, texSimon);
	sprites->Add(10003, 643, 1, 673, 60, texSimon);

	//left
	sprites->Add(10011, 183, 3, 214, 60, texSimon1);
	sprites->Add(10012, 159, 1, 181, 60, texSimon1);
	sprites->Add(10013, 126, 1, 156, 60, texSimon1);
	LPANIMATION ani;
	ani = new Animation(100);
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new Animation(100);
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new Animation(100);
	ani->Add(10001);
	ani->Add(10002);
	ani->Add(10003);

	animations->Add(500, ani);

	ani = new Animation(100);
	ani->Add(10011);
	ani->Add(10012);
	ani->Add(10013);
	animations->Add(501, ani);

	simon = new Simon();
	Simon::AddAnimation(400);		// idle right
	Simon::AddAnimation(401);		// idle left
	Simon::AddAnimation(500);		// walk right
	Simon::AddAnimation(501);		// walk left
	simon->SetPosition(0.0f,100.0f);
}
void Update(DWORD dt)
{
	simon->Update(dt);
}
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);
		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		simon->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, 
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = 1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();
		DWORD dt = now - frameStart;
		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->ProcessKeyboard();
			Update(dt);
			Render();
		}
		else 
		{
			Sleep(tickPerFrame - dt);
		}
	}
	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = Game::GetInstance();
	game->Init(hWnd);
	keyHandler = new SampleKeyHander();
	game->InitKeyboard(keyHandler);
	LoadResources();
	Run();
	return 0;
}
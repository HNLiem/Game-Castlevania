#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include"Animation.h"
#include"Animations.h"
class GameObject
{
protected:
	float x, y;
	float vx, vy;
	int nx;
	int state;
	static vector<LPANIMATION> animations;
public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetState(int state) { this->state = state; }
	int GetState() { return this->state; }
	static void AddAnimation(int aniId);
	GameObject();
	void Update(DWORD dt);
	void Render();
	~GameObject();
};


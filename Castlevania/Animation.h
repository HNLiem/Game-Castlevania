#pragma once
#include<Windows.h>
#include<d3dx9.h>
#include <unordered_map>
#include"AnimationFrame.h"
using namespace std;
class Animation
{
private:
	DWORD lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	Animation(int defaultTime) { this->defaultTime = defaultTime; this->lastFrameTime = -1; this->currentFrame = -1; };
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y);
	~Animation();
};
typedef Animation *LPANIMATION;


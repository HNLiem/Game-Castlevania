#include "Sprites.h"
#include"Game.h"

Sprites * Sprites::_instance = NULL;
void Sprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	LPSPRITE s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;
}

LPSPRITE Sprites::Get(int id)
{
	return sprites[id];
}

Sprites * Sprites::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new Sprites();
	}
	return _instance;
}

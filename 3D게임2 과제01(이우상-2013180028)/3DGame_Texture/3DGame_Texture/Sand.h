#pragma once
#include "GameObject.h"
class CSand : public CGameObject
{
public:
	CSand();
	~CSand();

	void Animate(float fTimeElapsed);
};


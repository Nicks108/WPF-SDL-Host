#pragma once
#include "I_GUIWindow.h"
class ComponentBase : I_GUIWindow
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void End() = 0;
	virtual void GUIDraw() =0;
};


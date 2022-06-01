#pragma once
#include <iostream>
#include "imgui.h"

class I_GUIWindow
{
public:
    static I_GUIWindow* SelectedObject;
    static I_GUIWindow* SelectedObjectForDragging;


	virtual void GUIDraw() = 0;
    
};


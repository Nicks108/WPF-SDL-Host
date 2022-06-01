#pragma once
#include "../Libraries/imgui-master/imgui.h"
#include <string>
#include <vector>
class GUIWindow 
{
public:
    std::string WindowName;

    virtual void Draw() = 0;




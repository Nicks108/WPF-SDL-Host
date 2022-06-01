#pragma once
#include "../Libraries/imgui-master/imgui.h"
class GUIControle
{
public :
	virtual void Draw()
	{
		ImGui::Text("this is the base controle");
	}
};


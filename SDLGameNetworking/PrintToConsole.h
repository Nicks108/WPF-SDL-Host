#pragma once
#include <iostream>
#include <string>

#include "ComponentBase.h"


class PrintToConsole :
    public ComponentBase
{
public:
    std::string Message = "Update";

    void Start() override
    {
        std::cout << "Start Function" << std::endl;
    }
    void Update() override
    {
        std::cout << Message << std::endl;
    }

    void End() override
    {
        std::cout << "End Function" << std::endl;
    }

    void GUIDraw() override
    {
        ImGui::Text(typeid(this).name());

        ImGui::InputText(this->Message.c_str(), (char*)this->Message.c_str(), this->Message.length());

    }
};


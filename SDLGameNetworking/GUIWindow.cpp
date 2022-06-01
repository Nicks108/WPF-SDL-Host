#include "GUIWindow.h"


GUIWindow::GUIWindow(): GUIWindow("New Window")
{
    
}

GUIWindow::GUIWindow(std::string windowName)
{
    WindowName = windowName;
    
}



void GUIWindow::Draw()
{
    ImGui::Begin(this->WindowName.c_str());

    for (auto controle : ConsotlesVector)
    {
        controle->Draw();
    }

    ImGui::End();
}

GUIWindow::~GUIWindow()
{
    for (auto controle : ConsotlesVector)
    {
        delete controle;
        controle = nullptr;
    }
}


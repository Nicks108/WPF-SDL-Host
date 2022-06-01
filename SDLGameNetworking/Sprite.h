#pragma once
#include "Bitmap.h"

#include <string>
class Sprite :
	public Bitmap
{
public:
	Sprite(SDL_Renderer* renderer, string filename, int xPos, int yPos, bool useTransparency, string name);
    /*Sprite(Bitmap& bitmapToCopy);*/
	~Sprite();
    void Update();


    void GUIDraw() override
    {
        //all sprites use the samf window ID to stop the window "pop" problem
        ImGui::Begin("Selection");
        ImGui::Text(this->ObjectName.c_str());

        int* TempPosAddressArry[2] = { &Transfrom.Position().X , &Transfrom.Position().Y };
        ImGui::InputInt2("Position:", *TempPosAddressArry);

        int* TempScaleAddressArry[2] = { &Transfrom.Scale().X , &Transfrom.Scale().Y };
        ImGui::InputInt2("Scale:", *TempScaleAddressArry);

        ImGui::Image(this->m_pBitmapTexture, ImVec2(m_pBitmapSurface->w, m_pBitmapSurface->h));

        ImGui::Separator();
        for (ComponentBase* component : Components)
        {
            component->GUIDraw();
        }


        ImGui::End();
    }

    void GUIUpdateSceneSelfAndChildren(ImGuiIO& io, bool BDrawDebug)
    {
        this->DrawDebug(BDrawDebug);
        this->MouseUpdate(io);
        
        if (this->CheckBounds(io))
        {
            I_GUIWindow::SelectedObject = this;
        }
        for (Transfrom2D* child : Transfrom.Children)
        {
            Sprite* childAsSprite = static_cast<Sprite*>(child->Owner);
            childAsSprite->GUIUpdateSceneSelfAndChildren(io, BDrawDebug);
        }
    }


    bool CheckBounds(ImGuiIO& io)
    {
        SDL_Rect ImageBound = { Transfrom.Position().X , Transfrom.Position().Y , m_pBitmapSurface->w, m_pBitmapSurface->h };
        SDL_Point MousePosition = { io.MouseClickedPos->x, io.MouseClickedPos->y };

        return SDL_PointInRect(&MousePosition, &ImageBound);
    }

    void DrawDebug(bool b)
    {
        if(b)
        {
            Transfrom2D updatedTrnasfrom = Transfrom.UpateTransform();
            SDL_Rect ImageBound = { updatedTrnasfrom.Position().X , updatedTrnasfrom.Position().Y, m_pBitmapSurface->w, m_pBitmapSurface->h };


            Uint8 Color[4];
            SDL_GetRenderDrawColor(m_pRenderer, &Color[0], &Color[1], &Color[2], &Color[3]);

            SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(m_pRenderer, &ImageBound);

            SDL_SetRenderDrawColor(m_pRenderer, Color[0], Color[1], Color[2], Color[3]);
        }
    }




    void MouseUpdate(ImGuiIO& io)
    {
        OnMouseDown(io);
        OnMouseDrag(io);
        OnMouseUp(io);
    }

    void OnMouseDown(ImGuiIO& io)
    {
        if (SelectedObjectForDragging == nullptr)
        {
            if (CheckBounds(io))
            {
                SelectedObjectForDragging = this;
            }
        }
    }
    void OnMouseDrag(ImGuiIO& io)
    {
        if (SelectedObjectForDragging == this && io.MouseDown[0])
        {
            Transfrom.Position().X += io.MouseDelta.x;
            Transfrom.Position().Y += io.MouseDelta.y;

            io.MousePos.x = 0;
            io.MousePos.y = 0;
            io.MouseDelta.x = 0;
            io.MouseDelta.y = 0;
        }
    }
    void OnMouseUp(ImGuiIO& io)
    {
        if(SelectedObjectForDragging == this && !io.MouseDown[0])
        {
            SelectedObjectForDragging = nullptr;
        }
            
        
    }


    


};


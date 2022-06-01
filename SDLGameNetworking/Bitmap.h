#ifndef BITMAP_H
#define BITMAP_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <iostream>
#include <sstream>


#include "ComponentBase.h"
#include "I_GUIWindow.h"
#include "Transfrom2D.h"
using namespace std;



class Bitmap: public I_GUIWindow
{
public:
	static int ObjectCount;

	Transfrom2D Transfrom;
	string FileName;

	Bitmap(SDL_Renderer* renderer, string filename,int xPos, int yPos, bool useTransparency = false, const string objectName ="" );
	/*Bitmap(const Bitmap& BitmapToCopy);*/
	virtual ~Bitmap();
	void Update();

	void Draw();
	void Draw(SDL_Rect destRect);

	void AddComponet(ComponentBase* comp)
	{
		Components.push_back(comp);
		comp->Start();
	}

	void GUIDraw() override
	{
	}

	void GUIDrawChildGUI()
	{

		 
			//|(selected ? ImGuiTreeNodeFlags_Selected : 0) |
			//(gameObject->Children().empty() ? ImGuiTreeNodeFlags_Leaf : 0)

		//if (ImGui::TreeNode(this->ObjectName.c_str()))
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
		

		bool isNodeOpen = ImGui::TreeNodeEx(this->ObjectName.c_str(), nodeFlags, this->ObjectName.c_str());

		if (ImGui::IsItemClicked())
		{
			I_GUIWindow::SelectedObject = this;
			cout << "selected object is " << static_cast<Bitmap*>(I_GUIWindow::SelectedObject)->ObjectName << endl;
		}
		if (I_GUIWindow::SelectedObject == this && ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("_TREENODE", this, sizeof(Bitmap*));
			//cout << reinterpret_cast<void*>(this) << " " << this->ObjectName <<  endl;
			ImGui::Text("This is a drag and drop source");
			//cout << "selected object is " << static_cast<Bitmap*>(I_GUIWindow::SelectedObject)->ObjectName<<endl;
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
			{
				//IM_ASSERT(payload->DataSize == sizeof(Bitmap*));
				Bitmap* PayloadAsBitmap = static_cast<Bitmap*>(I_GUIWindow::SelectedObject);
				cout << PayloadAsBitmap->ObjectName << " on top of " << this->ObjectName << endl;

				PayloadAsBitmap->Transfrom.ParentSet(this->Transfrom);

			}

			ImGui::EndDragDropTarget();
		}
		if (isNodeOpen)
		{
		    for (int i = 0; i < this->Transfrom.Children.size(); i++)
			{
				Bitmap* child = (Bitmap*)this->Transfrom.Children[i]->Owner;
				child->GUIDrawChildGUI();

			}

			if (I_GUIWindow::SelectedObject == this)
				nodeFlags |= ImGuiTreeNodeFlags_Selected;

			
			ImGui::TreePop();
		}


		

		
	}

	void GUIDrawComponets()
	{
        for (ComponentBase* component : Components)
        {
			component->GUIDraw();
        }
	}




public:
	std::string ObjectName = "Name Object";

	const SDL_Texture* GetTextureRef()
	{
		return m_pBitmapTexture;
	}

protected:
	SDL_Surface* m_pBitmapSurface; //software rendering
	SDL_Texture* m_pBitmapTexture; // GPU version of surface, faster!
	SDL_Renderer* m_pRenderer;

	//int m_x;
	//int m_y;

protected:
	vector<ComponentBase*> Components;
};

#endif // BITMAP_H
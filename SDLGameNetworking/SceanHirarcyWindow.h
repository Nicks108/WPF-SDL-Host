#pragma once
#include "Bitmap.h"
#include "imgui.h"
#include "Transfrom2D.h"
#include "GameWindow.h"
#include "I_GUIWindow.h"
#include "Sprite.h"


class SceanHirarcyWindow
{
private:
	Transfrom2D* Root;
	bool BDrawDebug = false;
public:
	
	SceanHirarcyWindow(GameWindow* window)
	{
		Root = &window->GetHirarcy();
	}

	void Draw()
	{
		ImGui::Begin("Scene Hirarcy");
		ImGui::Checkbox("Draw Debug", &BDrawDebug);


	
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_DefaultOpen;
		bool isNodeOpen = ImGui::TreeNodeEx("Scene", nodeFlags, "Scene");

		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
			{
				//IM_ASSERT(payload->DataSize == sizeof(Bitmap*));
				Bitmap* PayloadAsBitmap = static_cast<Bitmap*>(I_GUIWindow::SelectedObject);
				cout << PayloadAsBitmap->ObjectName << " on top of Root"<< endl;

				PayloadAsBitmap->Transfrom.ParentSet(*Root);

			}

			ImGui::EndDragDropTarget();
		}

		if(isNodeOpen)
		{
			//cout << bitmap->ObjectName.c_str() << endl;
			//I_GUIWindow::SelectedObject = .ObjectName;

			for (int i = 0; i < Root->Children.size(); i++)
			{
				Bitmap* child = (Bitmap*)Root->Children[i]->Owner;
				child->GUIDrawChildGUI();

			}
			ImGui::TreePop();
		}

		ImGui::End();
	}

	void GUIUpdateScene(ImGuiIO& io)
	{
		for (Transfrom2D* child : Root->Children)
		{
			//this only supports sprites
			//need to allow for other game object types
			Sprite* ChiledAsSprite = static_cast<Sprite*>(child->Owner);
			ChiledAsSprite->GUIUpdateSceneSelfAndChildren(io, BDrawDebug);
		}
	}

};


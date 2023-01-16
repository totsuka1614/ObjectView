//=============================================================================
//
// GUI クラス [GUI.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "GUI.h"
#include "GlobalData.h"
#include "SceneManager.h"
#include "Input.h"

GUI g_Gui;

GUI* GUI::m_pGui = &g_Gui;

using namespace ImGui;

void GUI::Init()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& io = GetIO(); (void)io;

	// Setup Dear ImGui style
	StyleColorsDark();
	//StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(Window::GetWindow()->GetWindowHandle());
	ImGui_ImplDX11_Init(BACKBUFFER->GetDevice(), BACKBUFFER->GetDeviceContext());

	m_bDisplay = false;
}

void GUI::Release()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
}

void GUI::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();

	
}

void GUI::Draw()
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ID3D11RenderTargetView* target = BACKBUFFER->GetRenderTargetView();

	// Rendering
	Render();
	//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	BACKBUFFER->GetDeviceContext()->OMSetRenderTargets(1, &target, NULL);
	//BACKBUFFER->GetDeviceContext()->ClearRenderTargetView(target, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
}

void GUI::Display()
{
	if (CInput::GetKeyTrigger(VK_O))
		m_bDisplay = !m_bDisplay;

	if (GLOBALDATA->GetStartFlag() || m_bDisplay)
		return;

	if (SCENE->GetID() != SCENE_DEBUG)
		return;

	ListDisplay();

	ObjectDisplay();

	CameraCreate();
}

void GUI::CameraCreate()
{
	CCamera *camera = CCamera::Get();

	Begin("Camera Info");
	Text("TransForm : %f , %f , %f", camera->GetTransform().x, camera->GetTransform().y, camera->GetTransform().z);
	DragFloat("Pos:x", &camera->GetTransform().x, 0.1f);
	DragFloat("Pos:y", &camera->GetTransform().y, 0.1f);
	DragFloat("Pos:z", &camera->GetTransform().z, 0.1f);
	Text("Target : %f , %f , %f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	DragFloat("Target:x", &camera->GetTarget().x, 0.1f);
	DragFloat("Target:y", &camera->GetTarget().y, 0.1f);
	DragFloat("Target:z", &camera->GetTarget().z, 0.1f);
	Text("FogStatus : %f , %f", camera->GetStart(), camera->GetRange());
	DragFloat("Start:z", &camera->GetStart(), 0.1f);
	DragFloat("Range:z", &camera->GetRange(), 0.1f);

	End();
}

void GUI::ListDisplay()
{


	for (auto model : m_ObjectList)
	{
		
		Begin("Object List");
		if (model->GetActive())
		{
			if (Selectable(model->GetName(), &model->GetActive()))
			{
				for (auto a : m_ObjectList)
				{
					a->GetActive() = false;
				}
			}
		}
		else
		{
			if (Selectable(model->GetName(), &model->GetActive()))
			{

				for (auto a : m_ObjectList)
				{
					a->GetActive() = false;
				}
				model->GetActive() = true;
			}
		}

		End();
	}
}

void GUI::ObjectDisplay()
{
	for (auto model : m_ObjectList)
	{
		if (!model->GetActive())
			continue;

		Begin("Model Info"); 
		Text("FileName : %s", model->GetName()); SameLine(); Checkbox("Enable", &model->GetEnable());
		Text("TransForm : %f , %f , %f", model->GetTransform().x, model->GetTransform().y, model->GetTransform().z);
		DragFloat("Pos:x", &model->GetTransform().x, 0.1f);
		DragFloat("Pos:y", &model->GetTransform().y, 0.1f);
		DragFloat("Pos:z", &model->GetTransform().z, 0.1f);
		Text("Scale : %f , %f , %f", model->GetScale().x, model->GetScale().y, model->GetScale().z);
		DragFloat("Scale:x", &model->GetScale().x, 0.1f);
		DragFloat("Scale:y", &model->GetScale().y, 0.1f);
		DragFloat("Scale:z", &model->GetScale().z, 0.1f);
		Text("Rot : %f , %f , %f", model->GetRotation().x, model->GetRotation().y, model->GetRotation().z);
		DragFloat("Rot:x", &model->GetRotation().x, 0.1f);
		DragFloat("Rot:y", &model->GetRotation().y, 0.1f);
		DragFloat("Rot:z", &model->GetRotation().z, 0.1f);
		Text("Color : %f , %f , %f , %f", model->GetMaterial().x, model->GetMaterial().y, model->GetMaterial().z,model->GetMaterial().w);
		static float col2[4] = { model->GetMaterial().x,model->GetMaterial().y,model->GetMaterial().z,model->GetMaterial().w };
		ColorEdit4("color 2", (float*)&model->GetMaterial());
		switch (model->GetPSType())
		{
		case PIXEL:
			Text("PSShaderType : NORMAL");
			break;
		case LAMBERT:
			Text("PSShaderType : LAMBERT");
			break;
		case PHONG:
			Text("PSShaderType : PHONG");
			break;
		case LIM:
			Text("PSShaderType : LIM");
			break;
		case UNLIT:
			Text("PSShaderType : UNLIT");
			break;
		case DISSOLVE:
			Text("PSShaderType : DISSOLVE");
			SliderFloat("Rate", &model->GetDissolveRate(), 0.0f, 1.0f);
			break;
		case BUMPMAP:
			Text("PSShaferType : BUMPMAP");
			break;
		case FOG:
			Text("PSShaferType : FOG");
			break;
		case TOONPS:
			Text("PSShaferType : TOON");
			break;
		}
		RadioButton("NORMAL", (int*)&model->GetPSType(), 3); SameLine();
		RadioButton("LAMBERT", (int*)&model->GetPSType(), 1); SameLine();
		RadioButton("PHONG", (int*)&model->GetPSType(), 2); SameLine();
		RadioButton("LIM", (int*)&model->GetPSType(), 5); 
		RadioButton("DISSOLVE", (int*)&model->GetPSType(), 8); SameLine();
		RadioButton("BUMP", (int*)&model->GetPSType(), 9); SameLine();
		RadioButton("FOG", (int*)&model->GetPSType(), 6); SameLine();
		RadioButton("TOON", (int*)&model->GetPSType(), 10); SameLine();
		RadioButton("UNLIT", (int*)&model->GetPSType(), 0);

		//Checkbox("Enable", &model->GetEnable());

		if (Button("Delete")) {
			SCENE->m_pDebug->GetNameList().remove(model->GetName());
			SCENE->m_pDebug->Delete(model->GetName());
			m_ObjectList.remove(model);
			End();
			break;
		}

		const char* listbox_items[] = {"None", "Player", "Wall", "Land", "Goal", "Damage"};
		ImGui::ListBox("listbox\n(single select)", (int*)&model->GetTag(), listbox_items, IM_ARRAYSIZE(listbox_items), 4);


		InputText("TexturePath", model->GetTexturePath() , 256);
		if (Button("Texure")) {
			if (model->SetTexture() == S_OK)
			{
				model->SetTextureFlag(true);
			}
			else
			{
				MessageBox(NULL, _T("テクスチャが見つかりません"), _T(""), MB_OK);
				model->SetTextureFlag(false);
			}
		}
		End();

	}


}

CREATE_OBJECT GUI::DebugDisplay()
{
	static CREATE_OBJECT obj;

	obj.bCreate = false;

	if (m_bDisplay)
		return obj;

	Begin("Create Object");


	InputText("Name", obj.cName, IM_ARRAYSIZE(obj.cName));

	switch (obj.type)
	{
	case BOX:
		Text("ObjectType : BOX");
		break;
	case SPHERE:
		Text("ObjectType : SPHERE");
		break;
	case FBX:
		Text("ObjectType : FBX");
		Text("FilePath : ");
		InputText("FileName", obj.cPath, IM_ARRAYSIZE(obj.cPath));
		break;

	}

	RadioButton("BOX", (int*)&obj.type, 0); SameLine();
	RadioButton("SPHERE", (int*)&obj.type, 0); SameLine();
	RadioButton("FBX", (int*)&obj.type, 2);

	obj.bCreate = Button("Create");

	End();

	return obj;
}

void GUI::Uninit()
{
	m_ObjectList.clear();
}
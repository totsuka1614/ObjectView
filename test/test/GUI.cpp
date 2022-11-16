//=============================================================================
//
// GUI ƒNƒ‰ƒX [GUI.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "GUI.h"
#include "SceneManager.h"

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
	ImGui_ImplDX11_Init(BackBuffer::GetBuffer()->GetDevice(), BackBuffer::GetBuffer()->GetDeviceContext());
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

	ID3D11RenderTargetView* target = BackBuffer::GetBuffer()->GetRenderTargetView();

	// Rendering
	Render();
	//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	BackBuffer::GetBuffer()->GetDeviceContext()->OMSetRenderTargets(1, &target, NULL);
	//BackBuffer::GetBuffer()->GetDeviceContext()->ClearRenderTargetView(target, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
}

void GUI::Display()
{
	ListDisplay();

	ObjectDisplay();


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

	End();
}

void GUI::ListDisplay()
{
	Begin("Object List");

	for (auto model : m_ObjectList)
	{
		Selectable(model->GetName(), &model->GetActive());
	}



	End();
}

void GUI::ObjectDisplay()
{
	for (auto model : m_ObjectList)
	{
		if (!model->GetActive())
			continue;

		Begin("Model Info");
		Text("FileName : %s", model->GetName());
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
		}
		RadioButton("NORMAL", (int*)&model->GetPSType(), 3); SameLine();
		RadioButton("LAMBERT", (int*)&model->GetPSType(), 1); SameLine();
		RadioButton("PHONG", (int*)&model->GetPSType(), 2); SameLine();
		RadioButton("LIM", (int*)&model->GetPSType(), 5); SameLine();

		Checkbox("Enable", &model->GetEnable());

		if (Button("Delete")) {
			SceneManager::Get()->m_pDebug->GetNameList().remove(model->GetName());
			SceneManager::Get()->m_pDebug->Delete(model->GetName());
			m_ObjectList.remove(model);
			End();
			break;
		}
		End();

	}


}

CREATE_OBJECT GUI::DebugDisplay()
{
	Begin("Create Object");

	static CREATE_OBJECT obj;

	InputText("input text", obj.cName, IM_ARRAYSIZE(obj.cName));

	switch (obj.type)
	{
	case BOX:
		Text("ObjectType : BOX");
		break;
	case SPHERE:
		Text("ObjectType : SPHERE");
		break;
	}
	RadioButton("BOX", (int*)&obj.type, 0); SameLine();
	RadioButton("SPHERE", (int*)&obj.type, 0); 

	obj.bCreate = Button("Create");

	End();

	return obj;
}
//=============================================================================
//
// GUI ƒNƒ‰ƒX [GUI.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "GUI.h"


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
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	BackBuffer::GetBuffer()->GetDeviceContext()->OMSetRenderTargets(1, &target, NULL);
	BackBuffer::GetBuffer()->GetDeviceContext()->ClearRenderTargetView(target, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(GetDrawData());
}

void GUI::Display()
{
	ListDisplay();

	for (auto model : m_ModelList)
	{
		if (!model->GetActive())
			continue;

		Begin("Model Info");
		Text("FileName : %s", model->GetFileName());
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
		}
		RadioButton("NORMAL", (int*)&model->GetPSType(), 3); SameLine();
		RadioButton("LAMBERT", (int*)&model->GetPSType(), 1); SameLine();
		RadioButton("PHONG", (int*)&model->GetPSType(), 2); SameLine();

		if (Button("Save")) {

			SAVE_TRANSFORM save;
			save.pos = model->GetTransform();
			save.scale = model->GetScale();
			save.deglee = model->GetRotation();

			FILE* fp;

			char path[256] = "data/save/";
			strcat(path, model->GetName());
			strcat(path, ".totsuka");

			fopen_s(&fp, path, "wb");
			if (fp)
			{
				fwrite(&save, sizeof(SAVE_TRANSFORM), 1, fp);
				fclose(fp);
			}
		}
		End();

	}

	for (auto mesh : m_MeshList)
	{
		if (!mesh->GetActive())
			continue;

		Begin("Mesh Info");
		Text("Mesh type : %s", mesh->GetName());
		Text("TransForm : %f , %f , %f", mesh->GetTransform().x, mesh->GetTransform().y, mesh->GetTransform().z);
		DragFloat("Pos:x", &mesh->GetTransform().x, 0.1f);
		DragFloat("Pos:y", &mesh->GetTransform().y, 0.1f);
		DragFloat("Pos:z", &mesh->GetTransform().z, 0.1f);
		Text("Scale : %f , %f , %f", mesh->GetScale().x, mesh->GetScale().y, mesh->GetScale().z);
		DragFloat("Scale:x", &mesh->GetScale().x, 0.1f);
		DragFloat("Scale:y", &mesh->GetScale().y, 0.1f);
		DragFloat("Scale:z", &mesh->GetScale().z, 0.1f);
		Text("Rot : %f , %f , %f", mesh->GetRotation().x, mesh->GetRotation().y, mesh->GetRotation().z);
		DragFloat("Rot:x", &mesh->GetRotation().x, 0.1f);
		DragFloat("Rot:y", &mesh->GetRotation().y, 0.1f);
		DragFloat("Rot:z", &mesh->GetRotation().z, 0.1f);

		if (Button("Save")) {

			SAVE_TRANSFORM save;
			save.pos = mesh->GetTransform();
			save.scale = mesh->GetScale();
			save.deglee = mesh->GetRotation();

			FILE* fp;

			char path[256] = "data/save/";
			strcat(path, mesh->GetName());
			strcat(path, ".totsuka");

			fopen_s(&fp, path, "wb");
			if (fp)
			{
				fwrite(&save, sizeof(SAVE_TRANSFORM), 1, fp);
				fclose(fp);
			}
		}

		End();
	}
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

	for (auto model : m_ModelList)
	{
		Selectable(model->GetName(), &model->GetActive());
	}
	for (auto mesh : m_MeshList)
	{
		Selectable(mesh->GetName() , &mesh->GetActive());
	}


	End();
}
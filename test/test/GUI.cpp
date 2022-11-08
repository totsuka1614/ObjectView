//=============================================================================
//
// GUI ƒNƒ‰ƒX [GUI.cpp]
// Author : TOTSUKA KENSUKE
//
//=============================================================================
#include "GUI.h"

GUI g_Gui;

GUI* GUI::m_pGui = &g_Gui;

void GUI::Init()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(Window::GetWindow()->GetWindowHandle());
	ImGui_ImplDX11_Init(BackBuffer::GetBuffer()->GetDevice(), BackBuffer::GetBuffer()->GetDeviceContext());
}

void GUI::Release()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	
}

void GUI::Draw()
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ID3D11RenderTargetView* target = BackBuffer::GetBuffer()->GetRenderTargetView();

	// Rendering
	ImGui::Render();
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	BackBuffer::GetBuffer()->GetDeviceContext()->OMSetRenderTargets(1, &target, NULL);
	BackBuffer::GetBuffer()->GetDeviceContext()->ClearRenderTargetView(target, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Display(Model& model)
{
	ImGui::Begin("Model Info");
	ImGui::Text("FileName : %s",model.GetFileName());
	ImGui::Text("TransForm : %f , %f , %f", model.GetTransform().x, model.GetTransform().y, model.GetTransform().z);
	ImGui::DragFloat("Pos:x", &model.GetTransform().x, 0.1f);
	ImGui::DragFloat("Pos:y", &model.GetTransform().y, 0.1f);
	ImGui::DragFloat("Pos:z", &model.GetTransform().z, 0.1f);
	ImGui::Text("Scale : %f , %f , %f", model.GetScale().x, model.GetScale().y, model.GetScale().z);
	ImGui::DragFloat("Scale:x", &model.GetScale().x, 0.1f);
	ImGui::DragFloat("Scale:y", &model.GetScale().y, 0.1f);
	ImGui::DragFloat("Scale:z", &model.GetScale().z, 0.1f);
	ImGui::Text("Rot : %f , %f , %f", model.GetRotation().x, model.GetRotation().y, model.GetRotation().z);
	ImGui::DragFloat("Rot:x", &model.GetRotation().x, 0.1f);
	ImGui::DragFloat("Rot:y", &model.GetRotation().y, 0.1f);
	ImGui::DragFloat("Rot:z", &model.GetRotation().z, 0.1f);
	switch (model.GetPSType())
	{
	case PIXEL:
		ImGui::Text("PSShaderType : NORMAL");
		break;
	case LAMBERT:
		ImGui::Text("PSShaderType : LAMBERT");
		break;
	case PHONG:
		ImGui::Text("PSShaderType : PHONG");
		break;
	}
	ImGui::RadioButton("NORMAL", (int*)&model.GetPSType(), 0); ImGui::SameLine();
	ImGui::RadioButton("LAMBERT", (int*)&model.GetPSType(), 1); ImGui::SameLine();
	ImGui::RadioButton("PHONG", (int*)&model.GetPSType(), 2); ImGui::SameLine();
	ImGui::End();
}
void GUI::CameraCreate()
{
	CCamera *camera = CCamera::Get();

	ImGui::Begin("Camera Info");
	ImGui::Text("TransForm : %f , %f , %f", camera->GetTransform().x, camera->GetTransform().y, camera->GetTransform().z);
	ImGui::DragFloat("Pos:x", &camera->GetTransform().x, 0.1f);
	ImGui::DragFloat("Pos:y", &camera->GetTransform().y, 0.1f);
	ImGui::DragFloat("Pos:z", &camera->GetTransform().z, 0.1f);
	ImGui::Text("Target : %f , %f , %f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
	ImGui::DragFloat("Target:x", &camera->GetTarget().x, 0.1f);
	ImGui::DragFloat("Target:y", &camera->GetTarget().y, 0.1f);
	ImGui::DragFloat("Target:z", &camera->GetTarget().z, 0.1f);
	ImGui::Text("Rot : %f , %f , %f", camera->GetAngle().x, camera->GetAngle().y, camera->GetAngle().z);
	ImGui::DragFloat("Rot:x", &camera->GetAngle().x, 0.1f);
	ImGui::DragFloat("Rot:y", &camera->GetAngle().y, 0.1f);
	ImGui::DragFloat("Rot:z", &camera->GetAngle().z, 0.1f);

	ImGui::End();
}
/******************************************************************************
* 
* @file      GUI.cpp
* @brief     IMGUI�N���X
* @author    Totsuka Kensuke
* @date      2023/03/27
* @note      �f�[�^��UI�Ƃ��ĕ\��
* @attention 
* 
******************************************************************************/
#include "GUI.h"
#include "GlobalData.h"
#include "SceneManager.h"
#include "Input.h"
#include "light.h"

//���ݒ��
enum GUI_Envi
{
	GUI_CAMERA,
	GUI_LIGHT,
};

GUI g_Gui;		//�C���X�^���X����

GUI* GUI::m_pGui = &g_Gui;	//����

using namespace ImGui;

/******************************************************************************
* 
* @brief      Init
* @fn         ������
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
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
	ImGui_ImplWin32_Init(CWindow::GetWindow()->GetWindowHandle());
	ImGui_ImplDX11_Init(BACKBUFFER->GetDevice(), BACKBUFFER->GetDeviceContext());

	m_bDisplay = false;
}

/******************************************************************************
* 
* @brief      Release
* @fn         ���
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::Release()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	DestroyContext();
}

/******************************************************************************
* 
* @brief      Update
* @fn         �X�V
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::Update()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();

	
}

/******************************************************************************
* 
* @brief      Draw
* @fn         �`�揈��
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
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

/******************************************************************************
* 
* @brief      Display
* @fn         �\������
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::Display()
{
	// Control + 9 �L�[�ŕ\��/��\��
	if (CInput::GetKeyPress(VK_CONTROL) && CInput::GetKeyTrigger(VK_9))
		m_bDisplay = !m_bDisplay;
	
	//�Q�[�����n�܂������\��
	if (GLOBALDATA->GetStartFlag() || m_bDisplay)
		return;

	//�f�o�b�O�V�[���ȊO�͔�\��
	if (SCENE->GetID() != SCENE_DEBUG)
		return;

	//�I�u�W�F�N�g���X�g�\��
	ListDisplay();

	//�I�u�W�F�N�g���\��
	ObjectDisplay();

	//�����\��
	EnvironmentCreate();
}

/******************************************************************************
* 
* @brief      EnvironmentCreate
* @fn         �����\��
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::EnvironmentCreate()
{
	//�\�����[�h
	static GUI_Envi bEnvi = GUI_CAMERA;

	//�J�����E���C�g�Q�b�g
	CCamera *camera = CCamera::Get();
	CLight *light = CLight::Get();

	Begin("Environment", nullptr, ImGuiWindowFlags_MenuBar);

	//�����[�h�ύXUI
	if (BeginMenuBar()) {
		if (BeginMenu("Change"))
		{
			if (MenuItem("Camera")) {
				bEnvi = GUI_CAMERA;
			}
			if (MenuItem("Light")) {
				bEnvi = GUI_LIGHT;
			}

			ImGui::EndMenu();
		}
		EndMenuBar();
	}

	//�\������
	switch (bEnvi)
	{
		//�J�����\��
	case GUI_CAMERA:
		Text("Camera Info");
		//���_
		Text("TransForm : %f , %f , %f", camera->GetTransform().x, camera->GetTransform().y, camera->GetTransform().z);
		DragFloat("Pos:x", &camera->GetTransform().x, 0.1f);
		DragFloat("Pos:y", &camera->GetTransform().y, 0.1f);
		DragFloat("Pos:z", &camera->GetTransform().z, 0.1f);
		//�����_
		Text("Target : %f , %f , %f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
		DragFloat("Target:x", &camera->GetTarget().x, 0.1f);
		DragFloat("Target:y", &camera->GetTarget().y, 0.1f);
		DragFloat("Target:z", &camera->GetTarget().z, 0.1f);
		//�t�H�O�ݒ�
		Text("FogStatus : %f , %f", camera->GetStart(), camera->GetRange());
		DragFloat("Start:z", &camera->GetStart(), 0.1f);
		DragFloat("Range:z", &camera->GetRange(), 0.1f);

		break;
		//���C�g�\��
	case GUI_LIGHT:
		Text("Light Info");
		//����
		Text("Direction : %f , %f , %f", light->GetDir().x, light->GetDir().y, light->GetDir().z);
		SliderFloat("Dir:x", &light->GetDir().x, -1.0f, 1.0f);
		SliderFloat("Dir:y", &light->GetDir().y, -1.0f, 1.0f);
		SliderFloat("Dir:z", &light->GetDir().z, -1.0f, 1.0f);
		//���W
		Text("TransForm : %f , %f , %f", light->GetPos().x, light->GetPos().y, light->GetPos().z);
		DragFloat("Pos:x", &light->GetPos().x, 0.1f);
		DragFloat("Pos:y", &light->GetPos().y, 0.1f);
		DragFloat("Pos:z", &light->GetPos().z, 0.1f);
		break;
	default:
		break;
	}

	End();
}

/******************************************************************************
* 
* @brief      ListDisplay
* @fn         �I�u�W�F�N�g���X�g�\��
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::ListDisplay()
{
	//�I�u�W�F�N�g�̐����\��
	for (auto model : m_ObjectList)
	{
		Begin("Object List");
		//�A�N�e�B�u�Ȃ�
		if (model->GetActive())
		{
			//�\��
			if (Selectable(model->GetName(), &model->GetActive()))
			{
				for (auto model2 : m_ObjectList)
				{
					//���̃I�u�W�F�N�g���m���A�N�e�B�u
					model2->GetActive() = false;
				}
			}
		}
		else
		{
			if (Selectable(model->GetName(), &model->GetActive()))
			{

				for (auto model2 : m_ObjectList)
				{
					model2->GetActive() = false;
				}
				model->GetActive() = true;
			}
		}

		End();
	}
}

/******************************************************************************
* 
* @brief      ObjectDisplay
* @fn         �I�u�W�F�N�g���\��
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::ObjectDisplay()
{
	//�I�u�W�F�N�g�̐���
	for (auto model : m_ObjectList)
	{
		//�A�N�e�B�u�ł͂Ȃ��Ȃ�R���e�j���[
		if (!model->GetActive())
			continue;

		//���f�����\��
		Begin("Model Info"); 
		Text("FileName : %s", model->GetName()); SameLine(); Checkbox("Enable", &model->GetEnable());
		//���W
		Text("TransForm : %f , %f , %f", model->GetTransform().x, model->GetTransform().y, model->GetTransform().z);
		DragFloat("Pos:x", &model->GetTransform().x, 0.1f);
		DragFloat("Pos:y", &model->GetTransform().y, 0.1f);
		DragFloat("Pos:z", &model->GetTransform().z, 0.1f);
		//�g�嗦
		Text("Scale : %f , %f , %f", model->GetScale().x, model->GetScale().y, model->GetScale().z);
		DragFloat("Scale:x", &model->GetScale().x, 0.1f);
		DragFloat("Scale:y", &model->GetScale().y, 0.1f);
		DragFloat("Scale:z", &model->GetScale().z, 0.1f);
		//�p�x
		Text("Rot : %f , %f , %f", model->GetRotation().x, model->GetRotation().y, model->GetRotation().z);
		DragFloat("Rot:x", &model->GetRotation().x, 0.1f);
		DragFloat("Rot:y", &model->GetRotation().y, 0.1f);
		DragFloat("Rot:z", &model->GetRotation().z, 0.1f);
		//�J���[
		Text("Color : %f , %f , %f , %f", model->GetMaterial().x, model->GetMaterial().y, model->GetMaterial().z,model->GetMaterial().w);
		static float col2[4] = { model->GetMaterial().x,model->GetMaterial().y,model->GetMaterial().z,model->GetMaterial().w };
		ColorEdit4("color 2", (float*)&model->GetMaterial());

		//���_�V�F�[�_
		switch (model->GetVSType())
		{
		case VERTEX:
			Text("VSShaderType : VERTEX");
			break;
		case EDGEVS:
			Text("VSShaderType : EDGE");
			break;
		case VERTEX2D:
			Text("VSShaderType : 2D");
			break;
		case BUMPVS:
			Text("VSShaderType : BUMP");
			break;
		case SHADOWVS:
			Text("VSShaderType : SHADOW");
			break;
		case DEPTHWRITEVS:
			Text("VSShaderType : SHADOWWRITE");
			break;
		default:
			break;
		}
		SameLine();

		//�s�N�Z���V�F�[�_
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
		case DEPTHSHADOWPS:
			Text("PSShaferType : SHADOW");
			break;
		}

		//�s�N�Z���V�F�[�_�؂�ւ��{�^��
		RadioButton("NORMAL", (int*)&model->GetPSType(), PIXEL); SameLine();
		RadioButton("LAMBERT", (int*)&model->GetPSType(), LAMBERT); SameLine();
		RadioButton("PHONG", (int*)&model->GetPSType(), PHONG); SameLine();
		RadioButton("LIM", (int*)&model->GetPSType(), LIM);
		RadioButton("DISSOLVE", (int*)&model->GetPSType(), DISSOLVE); SameLine();
		RadioButton("BUMP", (int*)&model->GetPSType(), BUMPMAP); SameLine();
		RadioButton("FOG", (int*)&model->GetPSType(), FOG); SameLine();
		RadioButton("TOON", (int*)&model->GetPSType(), TOONPS);
		RadioButton("UNLIT", (int*)&model->GetPSType(), UNLIT); SameLine();
		RadioButton("SHADOW", (int*)&model->GetPSType(), DEPTHSHADOWPS); SameLine();

		//�f���[�g�{�^��
		if (Button("Delete")) {
			SCENE->m_pDebug->GetNameList().remove(model->GetName());
			SCENE->m_pDebug->Delete(model->GetName());
			m_ObjectList.remove(model);
			End();
			break;
		}

		//�R���W�����^�O�ύX�{�^��
		const char* listbox_items[] = {"None", "Player", "Wall", "Land", "Goal", "Damage"};
		ImGui::ListBox("listbox\n(single select)", (int*)&model->GetTag(), listbox_items, IM_ARRAYSIZE(listbox_items), 4);

		//�e�N�X�`���ݒ�UI
		InputText("TexturePath", model->GetTexturePath() , 256);
		if (Button("Texure")) {
			if (model->SetTexture() == S_OK)
			{
				model->SetTextureFlag(true);
			}
			else
			{
				MessageBox(NULL, _T("�e�N�X�`����������܂���"), _T(""), MB_OK);
				model->SetTextureFlag(false);
			}
		}

		if (model->GetPSType() == BUMPMAP)
		{
			InputText("NormalMapPath", model->GetNormalMapPath(), 256);

			if (Button("NormalMap")) {
				if (model->SetNormalMap() == S_OK)
				{
					model->SetNormalMapFlag(true);
				}
				else
				{
					model->SetNormalMapFlag(false);
					MessageBox(NULL, _T("�e�N�X�`����������܂���"), _T(""), MB_OK);
				}
			}
		}

		End();

	}


}

/******************************************************************************
* 
* @brief      DebugDisplay
* @fn         �I�u�W�F�N�g����UI
* @return     CREATE_OBJECT
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
CREATE_OBJECT GUI::DebugDisplay()
{
	//�I�u�W�F�N�g�����ϐ�
	static CREATE_OBJECT obj;

	obj.bCreate = false;

	//�f�o�b�O�\������Ă�����
	if (m_bDisplay)
		return obj;

	Begin("Create Object");

	InputText("Name", obj.cName, IM_ARRAYSIZE(obj.cName));

	//�I�u�W�F�N�g�^�C�v
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

	//�I�u�W�F�N�g�^�C�v�I���{�^��
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
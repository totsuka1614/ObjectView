/******************************************************************************
* 
* @file      GUI.cpp
* @brief     IMGUIクラス
* @author    Totsuka Kensuke
* @date      2023/03/27
* @note      データをUIとして表示
* @attention 
* 
******************************************************************************/
#include "GUI.h"
#include "GlobalData.h"
#include "SceneManager.h"
#include "Input.h"
#include "light.h"

//環境設定列挙
enum GUI_Envi
{
	GUI_CAMERA,
	GUI_LIGHT,
};

GUI g_Gui;		//インスタンス生成

GUI* GUI::m_pGui = &g_Gui;	//現在

using namespace ImGui;

/******************************************************************************
* 
* @brief      Init
* @fn         初期化
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
* @fn         解放
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
* @fn         更新
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
* @fn         描画処理
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
* @fn         表示処理
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::Display()
{
	// Control + 9 キーで表示/非表示
	if (CInput::GetKeyPress(VK_CONTROL) && CInput::GetKeyTrigger(VK_9))
		m_bDisplay = !m_bDisplay;
	
	//ゲームが始まったら非表示
	if (GLOBALDATA->GetStartFlag() || m_bDisplay)
		return;

	//デバッグシーン以外は非表示
	if (SCENE->GetID() != SCENE_DEBUG)
		return;

	//オブジェクトリスト表示
	ListDisplay();

	//オブジェクト情報表示
	ObjectDisplay();

	//環境情報表示
	EnvironmentCreate();
}

/******************************************************************************
* 
* @brief      EnvironmentCreate
* @fn         環境情報表示
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::EnvironmentCreate()
{
	//表示モード
	static GUI_Envi bEnvi = GUI_CAMERA;

	//カメラ・ライトゲット
	CCamera *camera = CCamera::Get();
	CLight *light = CLight::Get();

	Begin("Environment", nullptr, ImGuiWindowFlags_MenuBar);

	//環境モード変更UI
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

	//表示処理
	switch (bEnvi)
	{
		//カメラ表示
	case GUI_CAMERA:
		Text("Camera Info");
		//視点
		Text("TransForm : %f , %f , %f", camera->GetTransform().x, camera->GetTransform().y, camera->GetTransform().z);
		DragFloat("Pos:x", &camera->GetTransform().x, 0.1f);
		DragFloat("Pos:y", &camera->GetTransform().y, 0.1f);
		DragFloat("Pos:z", &camera->GetTransform().z, 0.1f);
		//注視点
		Text("Target : %f , %f , %f", camera->GetTarget().x, camera->GetTarget().y, camera->GetTarget().z);
		DragFloat("Target:x", &camera->GetTarget().x, 0.1f);
		DragFloat("Target:y", &camera->GetTarget().y, 0.1f);
		DragFloat("Target:z", &camera->GetTarget().z, 0.1f);
		//フォグ設定
		Text("FogStatus : %f , %f", camera->GetStart(), camera->GetRange());
		DragFloat("Start:z", &camera->GetStart(), 0.1f);
		DragFloat("Range:z", &camera->GetRange(), 0.1f);

		break;
		//ライト表示
	case GUI_LIGHT:
		Text("Light Info");
		//方向
		Text("Direction : %f , %f , %f", light->GetDir().x, light->GetDir().y, light->GetDir().z);
		SliderFloat("Dir:x", &light->GetDir().x, -1.0f, 1.0f);
		SliderFloat("Dir:y", &light->GetDir().y, -1.0f, 1.0f);
		SliderFloat("Dir:z", &light->GetDir().z, -1.0f, 1.0f);
		//座標
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
* @fn         オブジェクトリスト表示
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::ListDisplay()
{
	//オブジェクトの数分表示
	for (auto model : m_ObjectList)
	{
		Begin("Object List");
		//アクティブなら
		if (model->GetActive())
		{
			//表示
			if (Selectable(model->GetName(), &model->GetActive()))
			{
				for (auto model2 : m_ObjectList)
				{
					//他のオブジェクトをノンアクティブ
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
* @fn         オブジェクト情報表示
* @return     void
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
void GUI::ObjectDisplay()
{
	//オブジェクトの数分
	for (auto model : m_ObjectList)
	{
		//アクティブではないならコンテニュー
		if (!model->GetActive())
			continue;

		//モデル情報表示
		Begin("Model Info"); 
		Text("FileName : %s", model->GetName()); SameLine(); Checkbox("Enable", &model->GetEnable());
		//座標
		Text("TransForm : %f , %f , %f", model->GetTransform().x, model->GetTransform().y, model->GetTransform().z);
		DragFloat("Pos:x", &model->GetTransform().x, 0.1f);
		DragFloat("Pos:y", &model->GetTransform().y, 0.1f);
		DragFloat("Pos:z", &model->GetTransform().z, 0.1f);
		//拡大率
		Text("Scale : %f , %f , %f", model->GetScale().x, model->GetScale().y, model->GetScale().z);
		DragFloat("Scale:x", &model->GetScale().x, 0.1f);
		DragFloat("Scale:y", &model->GetScale().y, 0.1f);
		DragFloat("Scale:z", &model->GetScale().z, 0.1f);
		//角度
		Text("Rot : %f , %f , %f", model->GetRotation().x, model->GetRotation().y, model->GetRotation().z);
		DragFloat("Rot:x", &model->GetRotation().x, 0.1f);
		DragFloat("Rot:y", &model->GetRotation().y, 0.1f);
		DragFloat("Rot:z", &model->GetRotation().z, 0.1f);
		//カラー
		Text("Color : %f , %f , %f , %f", model->GetMaterial().x, model->GetMaterial().y, model->GetMaterial().z,model->GetMaterial().w);
		static float col2[4] = { model->GetMaterial().x,model->GetMaterial().y,model->GetMaterial().z,model->GetMaterial().w };
		ColorEdit4("color 2", (float*)&model->GetMaterial());

		//頂点シェーダ
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

		//ピクセルシェーダ
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

		//ピクセルシェーダ切り替えボタン
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

		//デリートボタン
		if (Button("Delete")) {
			SCENE->m_pDebug->GetNameList().remove(model->GetName());
			SCENE->m_pDebug->Delete(model->GetName());
			m_ObjectList.remove(model);
			End();
			break;
		}

		//コリジョンタグ変更ボタン
		const char* listbox_items[] = {"None", "Player", "Wall", "Land", "Goal", "Damage"};
		ImGui::ListBox("listbox\n(single select)", (int*)&model->GetTag(), listbox_items, IM_ARRAYSIZE(listbox_items), 4);

		//テクスチャ設定UI
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
					MessageBox(NULL, _T("テクスチャが見つかりません"), _T(""), MB_OK);
				}
			}
		}

		End();

	}


}

/******************************************************************************
* 
* @brief      DebugDisplay
* @fn         オブジェクト生成UI
* @return     CREATE_OBJECT
* @author     Totsuka Kensuke
* @date       2023/03/27
* @note       
* @attention  
* 
******************************************************************************/
CREATE_OBJECT GUI::DebugDisplay()
{
	//オブジェクト生成変数
	static CREATE_OBJECT obj;

	obj.bCreate = false;

	//デバッグ表示されていたら
	if (m_bDisplay)
		return obj;

	Begin("Create Object");

	InputText("Name", obj.cName, IM_ARRAYSIZE(obj.cName));

	//オブジェクトタイプ
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

	//オブジェクトタイプ選択ボタン
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
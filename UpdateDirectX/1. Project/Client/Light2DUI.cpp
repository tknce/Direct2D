#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>
#include <Engine/CTransform.h>


#include "imgui.h"
#include "ListUI.h"

#include "CImGuiMgr.h"


#define TARGET GetTarget()->Light2D()

Light2DUI::Light2DUI()
	: ComponentUI("LIGHT2D", COMPONENT_TYPE::LIGHT2D)
	, m_vLightColor()
	, m_vEmbient()
	, m_Type()
	, m_fRadius()
	, m_fAngle()
{
}

Light2DUI::~Light2DUI()
{
}


void Light2DUI::update()
{
	if (nullptr != GetTarget())
	{
		m_vLightColor	= GetTarget()->Light2D()->GetLightColor();
		m_vEmbient = GetTarget()->Light2D()->GetLightEmbient();

		m_Type = TARGET->GetLightType();

		m_fRadius = TARGET->GetRadius();
		m_fAngle = TARGET->GetAngle();

		m_fDir = GetTarget()->Transform()->GetRelativeRotation().z;
	}
	ComponentUI::update();
}

void Light2DUI::render_update()
{
	ComponentUI::render_update();

	ImGui::Text("LightColor"); ImGui::SameLine(); 

	
	ImGui::ColorEdit4("MyColor##3", (float*)&m_vLightColor, (false ? ImGuiColorEditFlags_AlphaPreview : 0));
	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	ImGui::Text("Embient   "); ImGui::SameLine(); ImGui::InputFloat3("##Embient", m_vEmbient);

	// Light Type

	vector<string> lightname;					
	lightname.push_back("DIRECTIONAL");
	lightname.push_back("POINT"); 
	lightname.push_back("SPOT"); 
	ImGui::Text("Light Type");			
	ImGui::SameLine();
	ImGui::InputText("##MeshName", (char*)lightname[(UINT)m_Type].data(), lightname[(UINT)m_Type].length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", Vec2(15.f, 15.f)))
	{
		ListUI* pListUI = dynamic_cast<ListUI*>(CImGuiMgr::GetInst()->FindUI("ListUI"));
		assert(pListUI);

		pListUI->SetItemList(lightname);
		pListUI->AddDynamicDBClicked(this, (FUNC_1)&Light2DUI::SetLightType);

		pListUI->Open();
	}

	ImGui::Text("Radius    "); ImGui::SameLine(); ImGui::InputFloat("##lightRadius", &m_fRadius);

	m_fAngle = (m_fAngle / XM_PI) * 180.f;
	ImGui::Text("Angle     "); ImGui::SameLine(); ImGui::InputFloat("##lightAngle", &m_fAngle);

	m_fDir = (m_fDir / XM_PI) * 180.f;
	ImGui::Text("Rot(SPOT) "); ImGui::SameLine(); ImGui::InputFloat("##ROT(spot)", &m_fDir);
		



	if (nullptr != GetTarget())
	{
		TARGET->SetLightColor(m_vLightColor);
		TARGET->SetLightEmbient(m_vEmbient);

		TARGET->SetLightType(m_Type);

		TARGET->SetRadius(m_fRadius);
		m_fAngle = (m_fAngle / 180.f)* XM_PI;
		TARGET->SetAngle(m_fAngle);
	}
}

void Light2DUI::SetLightType(DWORD_PTR _type)
{
	string LightType = (char*)_type;

	if (LightType == "DIRECTIONAL")
	{
		m_Type = LIGHT_TYPE::DIRECTIONAL;
		TARGET->SetLightType(m_Type);
	}
	else if (LightType == "POINT")
	{
		m_Type = LIGHT_TYPE::POINT;
		TARGET->SetLightType(m_Type);
	}
	else if (LightType == "SPOT")
	{
		m_Type = LIGHT_TYPE::SPOT;
		TARGET->SetLightType(m_Type);
	}
}




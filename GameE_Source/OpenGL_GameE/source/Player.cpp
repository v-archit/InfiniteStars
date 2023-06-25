#include "Player.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

Player::Player()
{
	
}


void Player::OnUpdate(const float& ts)
{
	m_Time += ts;

	if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Space)))
	{
		m_Velocity.y += m_EnginePower;
		if (m_Velocity.y < 0.0f)
			m_Velocity.y += m_EnginePower * 2.0f;

	}
	else
	{
		m_Velocity.y -= m_Gravity;
	}


	m_Velocity.y = glm::clamp(m_Velocity.y, -20.0f, 20.0f);
	
	m_Position += m_Velocity * (float)ts;
	

}


void Player::Reset()
{
	m_Position = { -10.0f, 0.0f };
	m_Velocity.y = 0;
}

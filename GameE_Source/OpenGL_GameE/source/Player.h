#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include "Random.h"


class Player
{
public:
	Player();

	void OnUpdate(const float& ts);
	void Reset();

	float GetRotation() { return m_Velocity.y * 6.0f - 90.0f; }
	const glm::vec2& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec2& pos) { m_Position = pos; }
    void SetVelocityX(const float& velocity) { m_Velocity.x = velocity; }

	uint32_t GetScore() const { return (uint32_t)(m_Position.x + 10.0f) / 10.0f; }
private:
	glm::vec2 m_Position = { -10.0f, 0.0f };
	glm::vec2 m_Velocity = { 8.0f, 0.0f };

	float m_EnginePower = 0.5f;
	float m_Gravity = 0.4f;

	float m_Time = 0.0f;

};
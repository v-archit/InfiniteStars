#pragma once

#include "Player.h"

struct Obstacle
{
	glm::vec3 TopPosition = { 0.0f, 8.0f, 0.0f };         //position and scale of 2 triangles that make a star
	glm::vec2 TopScale = { 5.0f, 5.0f };

	glm::vec3 BottomPosition = { 0.0f, -8.0f, 0.0f};
	glm::vec2 BottomScale = { 5.0f, 5.0f };
};

class Level
{
public:
	void Init();

	void OnUpdate(const float& ts);

	bool IsGameOver() const { return m_GameOver; }
	void Reset();

	Player& GetPlayer() { return m_Player; }
	std::vector<Obstacle>& GetObstacles() {	return m_Obstacles; }

	void SetPlayerShape(bool pShape) {
		playerShape = pShape;
	}

private:
	void CreateObstacle(int index, float offset);
	bool CollisionTest();

	void GameOver();
private:
	Player m_Player;

	bool m_GameOver = false;

	float m_ObstacleTarget = 30.0f;
	int m_ObstacleIndex = 0;

	std::vector<Obstacle> m_Obstacles;

	bool playerShape = false;

};
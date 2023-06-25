#include "Level.h"

#include <glm/gtc/matrix_transform.hpp>


static bool PointInTri(const glm::vec2& p, glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)         //checks for point p in the triangle p0, p1, p2
{
	float s = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

	if ((s < 0) != (t < 0))
		return false;

	float A = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	return A < 0 ?
		(s <= 0 && s + t >= A) :
		(s >= 0 && s + t <= A);
}


void Level::Init()
{

	m_Obstacles.resize(10);                 //resize the vector
	for (int i = 0; i < 10; i++)
		CreateObstacle(i, i * 7.0f);       //7 is the x distance b/w 2 obstacles
}

void Level::OnUpdate(const float& ts)
{
	
	m_Player.OnUpdate(ts);              //Update player class

	if (glm::abs(m_Player.GetPosition().y) >= 8.5f)                    //Outer bounds collision
	{
		GameOver();
		m_Player.SetPosition({ m_Player.GetPosition().x, m_Player.GetPosition().y < 0 ? -8.5f : 8.5f });
		return;
	}

	if (CollisionTest())             //Check for collision
	{
		GameOver();
		return;
	}

	if (m_Player.GetPosition().x > m_ObstacleTarget)                        //Redraw obstacles once they cross the screen
	{
		CreateObstacle(m_ObstacleIndex, m_ObstacleTarget + 20.0f);
		m_ObstacleIndex = ++m_ObstacleIndex % m_Obstacles.size();
		m_ObstacleTarget += 10.0f;
	}
}


void Level::CreateObstacle(int index, float offset)
{
	Obstacle& Obstacle = m_Obstacles[index];
	Obstacle.TopPosition.x = offset;
	Obstacle.BottomPosition.x = offset;
	Obstacle.TopPosition.z = 0;
	Obstacle.BottomPosition.z = 0;
	
	Obstacle.TopPosition.y = Random::RandGen(-8, 8);             //random no. between -8 and 8
	Obstacle.BottomPosition.y = Obstacle.TopPosition.y + 2.0f;
}

bool Level::CollisionTest()
{
	glm::vec4 playerTransformedVerts[4];

	if (!playerShape)
	{
		glm::vec4 playerVertices[4] = {               //original player vertices in vec4
			{ -0.5f, -0.5f, 1.0f, 1.0f },
			{ -0.5f,  0.5f, 1.0f, 1.0f },
			{  0.5f, -0.5f, 1.0f, 1.0f },
			{  0.5f,  0.5f, 1.0f, 1.0f }
		};

		const auto& pos = m_Player.GetPosition();

		for (int i = 0; i < 4; i++)                      //transform original vertices acc to current position, rotation and scale
		{
			playerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.0f })
				* playerVertices[i];
		}
	}
	else
	{
		glm::vec4 playerVertices[3] = {               //original player vertices in vec4
			{ -0.5f, -0.5f, 1.0f, 1.0f },
			{  0.5f, -0.5f, 1.0f, 1.0f },
			{  0.0f, 0.5f,  1.0f, 1.0f },
		};

		const auto& pos = m_Player.GetPosition();

		for (int i = 0; i < 3; i++)                      //transform original vertices acc to current position, rotation and scale
		{
			playerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { 1.5f, 1.5f, 1.0f })
				* playerVertices[i];
		}

		playerTransformedVerts[3] = playerTransformedVerts[2];
	}


	glm::vec4 ObstacleVertices[3] = {
		{ -0.5f, -0.5f, 1.0f, 1.0f },
		{  0.5f, -0.5f, 1.0f, 1.0f },
		{  0.0f, 0.5f,  1.0f, 1.0f },
	};

	for (auto& p : m_Obstacles)
	{
		glm::vec2 tri[3];
		
		// Upper Obstacle Triangle
		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), { p.TopPosition.x, p.TopPosition.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { p.TopScale.x, p.TopScale.y, 1.0f })
				* ObstacleVertices[i];
		}

		for (auto& vert : playerTransformedVerts)
		{
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))      //check transformed player point in transformed upper triangle
				return true;
		}

		// Bottom Obstacle Triangle
		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), { p.BottomPosition.x, p.BottomPosition.y, 0.0f })
				* glm::scale(glm::mat4(1.0f), { p.BottomScale.x, p.BottomScale.y, 1.0f })
				* ObstacleVertices[i];
		}

		for (auto& vert : playerTransformedVerts)
		{ 
			if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))         //check transformed player point in transformed lower triangle
				return true;
		}

	}
	return false;
}

void Level::GameOver()
{
	m_GameOver = true;
}

void Level::Reset()
{
	m_GameOver = false;

	m_Player.Reset();

	m_ObstacleTarget = 30.0f;
	m_ObstacleIndex = 0;
	for (int i = 0; i < 5; i++)
		CreateObstacle(i, i * 10.0f);
}
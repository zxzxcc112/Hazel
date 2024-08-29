#include "Level.h"
#include "Color.h"
#include "Random.h"
#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

static glm::vec4 HSVtoRGB(const glm::vec3& hsv)
{
	int H = (int)(hsv.x * 360.0f);
	double S = hsv.y;
	double V = hsv.z;

	double C = V * S;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = V - C;
	double Rs = 0, Gs = 0, Bs = 0;

	if (H >= 0 && H < 60 || H == 360)
	{
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120)
	{
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180)
	{
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240)
	{
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300)
	{
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else if (H >= 300 && H < 360)
	{
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	return { Rs + m, Gs + m, Bs + m, 1.0f };
}

static bool PointInTri(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
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

	m_TriangleTexture = Hazel::Texture2D::Create("assets/textures/Triangle3.png");
	m_Player.LoadAssets();

	m_Pillars.resize(5);
	for (int i = 0; i < 5; i++)
		CreatePillar(i, (i + 1) * 10.0f);
}

void Level::OnUpdate(Hazel::Timestep ts)
{
	m_Player.OnUpdate(ts);

	if (CollisionTest())
	{
		GameOver();
		return;
	}

	m_PillarHSV.x += 0.1f * ts;
	if (m_PillarHSV.x > 1.0f)
		m_PillarHSV.x = 0.0f;

	//Respawn Pillar
	if (m_Player.GetPosition().x - 30.0f > m_Pillars[m_PillarTarget].BtmPos.x)
	{
		size_t lastPillar = (m_PillarTarget + m_Pillars.size() - 1) % m_Pillars.size();
		float offset = m_Pillars[lastPillar].TopPos.x + 10.0f + Random::Float() * 10.0f;
		CreatePillar(m_PillarTarget, offset);
		m_PillarTarget = ++m_PillarTarget % m_Pillars.size();
	}
}

void Level::OnRender()
{
	auto position = m_Player.GetPosition();

	//background
	Hazel::Renderer2D::DrawQuad({ position.x, 0.0f, -0.99f }, { 60.0f, 50.0f }, { 0.2f, 0.3f, 0.3f, 1.0f });

	//Flooring and Ceiling
	glm::vec4 pillarColor = HSVtoRGB(m_PillarHSV);
	Hazel::Renderer2D::DrawQuad({position.x,  30.0f}, { 54.0f, m_FCScale }, pillarColor);
	Hazel::Renderer2D::DrawQuad({position.x, -30.0f}, { 54.0f, m_FCScale }, pillarColor);

	//Pillars
	for (int i = 0; i < 5; i++)
	{
		Hazel::Renderer2D::DrawRotatedQuad(m_Pillars[i].TopPos, m_Pillars[i].TopScale, glm::radians(180.0f), m_TriangleTexture, 1.0f, pillarColor);
		Hazel::Renderer2D::DrawQuad(m_Pillars[i].BtmPos, m_Pillars[i].BtmScale, m_TriangleTexture, 1.0f, pillarColor);
	}

	m_Player.OnRender();
}

void Level::OnImGuiRender()
{
	m_Player.OnImGuiRender();

	ImGui::Begin("Score", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
	ImGui::Text("Score: %d", (int)m_Player.GetPosition().x);
	ImGui::End();

	ImGui::Begin("Level Setting");
	ImGui::DragFloat("FC", &m_FCScale, 1.0f, 1.0f, 50.0f);
	ImGui::End();
}

void Level::GameOver()
{
	m_IsGameOver = true;
}

bool Level::CollisionTest()
{
	glm::vec4 playerVertices[] = {
		{-0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.5f, -0.5f, 0.0f, 1.0f},
		{ 0.5f,  0.5f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f, 1.0f}
	};

	const auto& pos = m_Player.GetPosition();
	for (int i = 0; i < 4; i++)
	{
		m_PlayerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), { pos, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.5f, 1.0f })
			* playerVertices[i];
	}

	// Floor and Ceil
	float gap = 13.5f;
	for (const auto& p : m_PlayerTransformedVerts)
	{
		if(p.y > gap || p.y < -gap)
			return true;
	}

	glm::vec4 pillarVertices[] = {
		{-0.5f + 0.079f, -0.5f + 0.136f, 0.0f, 1.0f},
		{ 0.5f - 0.079f, -0.5f + 0.136f, 0.0f, 1.0f},
		{ 0.0f         ,  0.5f - 0.136f, 0.0f, 1.0f}
	};

	for (const auto& pillar : m_Pillars)
	{
		glm::vec2 tri[3];

		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), pillar.TopPos)
				* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { pillar.TopScale, 1.0f })
				* pillarVertices[i];
		}

		for (const auto& p : m_PlayerTransformedVerts)
		{
			if (PointInTri({ p.x, p.y }, tri[0], tri[1], tri[2]))
				return true;
		}

		for (int i = 0; i < 3; i++)
		{
			tri[i] = glm::translate(glm::mat4(1.0f), pillar.BtmPos)
				* glm::scale(glm::mat4(1.0f), { pillar.BtmScale, 1.0f })
				* pillarVertices[i];
		}

		for (const auto& p : m_PlayerTransformedVerts)
		{
			if (PointInTri({ p.x, p.y }, tri[0], tri[1], tri[2]))
				return true;
		}
	}

	return false;
}

void Level::Reset()
{
	m_Player.Reset();

	for (int i = 0; i < 5; i++)
		CreatePillar(i, (i + 1) * 10.0f);
	m_PillarTarget = 0;

	m_IsGameOver = false;
}

void Level::CreatePillar(int index, float offset)
{
	Pillar& pillar = m_Pillars[index];

	pillar.TopPos.x = offset;
	pillar.BtmPos.x = offset;

	pillar.TopPos.z = index * 0.01f - 0.5f;
	pillar.BtmPos.z = index * 0.01f - 0.5f + 0.05f;

	float center = Random::Float() * 16.0f - 8.0f; //-7~7
	float gap = 3.0f + Random::Float() * 2.0f; //3-5

	pillar.TopPos.y =  10.0f + center + gap;
	pillar.BtmPos.y = -10.0f + center - gap;
}

#pragma once

/*
* �C������ : Flappy Bird
* ����: ���b
* �\��:
*	���b�����A��space�i�H���W�u
*	�ɤl�t�ΡA���b�Q�X����
*	����I���˴� (�x�ΡB�T����)
*	�H���a��
*	��B�z�ĪG (�ܦ�B���w)
*	�۾����H����
*	��ܤ���
*	���s�C��
*/

#include "Hazel.h"
#include "Level.h"
#include "imgui.h"

class GameLayer :public Hazel::Layer
{
public:
	GameLayer();
	~GameLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnImGuiRender() override;

	virtual void OnEvent(Hazel::Event& e) override;
private:
	bool OnWindowResized(Hazel::WindowResizeEvent& e);
	bool OnKeyPressed(Hazel::KeyPressedEvent& e);
	void CreateCamera(uint32_t width, uint32_t height);
private:
	Hazel::Scope<Hazel::OrthographicCamera> m_Camera;
	Level m_Level;
	ImFont* m_Font = nullptr;

	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2
	};

	GameState m_State = GameState::MainMenu;
};
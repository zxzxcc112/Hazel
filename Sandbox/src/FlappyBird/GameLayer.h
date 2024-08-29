#pragma once

/*
* 遊戲類型 : Flappy Bird
* 角色: 火箭
* 功能:
*	火箭掉落，按space可以往上彈
*	粒子系統，火箭噴出煙霧
*	物體碰撞檢測 (矩形、三角形)
*	隨機地圖
*	後處理效果 (變色、光暈)
*	相機跟隨角色
*	顯示分數
*	重製遊戲
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
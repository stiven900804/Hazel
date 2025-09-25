#pragma once
#include "hzpch.h"
#include "UIElement.h"
#include "State/UIState.h"
#include "Hazel/Renderer/Sprite.h"

namespace Hazel
{
	// UIInteractive 是所有可交互UI元素的基类
	class UIInteractive : public UIElement
	{
	public:
		UIInteractive(glm::vec2 position = { 0.0f, 0.0f }, glm::vec2 size = { 0.0f, 0.0f });
		virtual ~UIInteractive() = default;

		virtual void Clicked() {}       ///< @brief 如果有点击事件，则重写该方法

		void AddSprite(std::string_view filePath,Ref<Sprite> sprite);///< @brief 添加精灵
		void SetSprite(std::string_view name);                                                ///< @brief 设置当前显示的精灵
		void AddSound(std::string_view name, std::string_view path);                        ///< @brief 添加音效
		void PlaySound(std::string_view name);                                                ///< @brief 播放音效

		// --- Getters and Setters ---
		void SetState(Ref<UIState> state);       ///< @brief 设置当前状态
		UIState* GetState() const { return m_CurrentState.get(); }   ///< @brief 获取当前状态

		void SetInteractive(bool interactive) { m_Interactive = interactive; }   ///< @brief 设置是否可交互
		bool IsInteractive() const { return m_Interactive; }                     ///< @brief 获取是否可交互
		
		// --- 核心方法 ---
		bool HandleInput() override;
		void Render() override;

	protected:
		bool m_Interactable = true;
		bool m_IsMouseOver = false;
		Ref<Hazel::UIState> m_CurrentState;     ///< @brief 当前状态
		std::unordered_map<std::string, Ref<Sprite>> m_Sprites; ///< @brief 精灵集合
		std::unordered_map<std::string, std::string> m_Sounds;   ///< @brief 音效集合，key为音效名称，value为音效文件路径
		Ref<Sprite> m_CurrentSprite = nullptr;      ///< @brief 当前显示的精灵
		bool m_Interactive = true;                               ///< @brief 是否可交互

	};
}

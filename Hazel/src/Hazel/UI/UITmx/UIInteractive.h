#pragma once
#include "hzpch.h"
#include "UIElement.h"
#include "State/UIState.h"
#include "Hazel/Renderer/Sprite.h"

namespace Hazel
{
	// UIInteractive �����пɽ���UIԪ�صĻ���
	class UIInteractive : public UIElement
	{
	public:
		UIInteractive(glm::vec2 position = { 0.0f, 0.0f }, glm::vec2 size = { 0.0f, 0.0f });
		virtual ~UIInteractive() = default;

		virtual void Clicked() {}       ///< @brief ����е���¼�������д�÷���

		void AddSprite(std::string_view filePath,Ref<Sprite> sprite);///< @brief ��Ӿ���
		void SetSprite(std::string_view name);                                                ///< @brief ���õ�ǰ��ʾ�ľ���
		void AddSound(std::string_view name, std::string_view path);                        ///< @brief �����Ч
		void PlaySound(std::string_view name);                                                ///< @brief ������Ч

		// --- Getters and Setters ---
		void SetState(Ref<UIState> state);       ///< @brief ���õ�ǰ״̬
		UIState* GetState() const { return m_CurrentState.get(); }   ///< @brief ��ȡ��ǰ״̬

		void SetInteractive(bool interactive) { m_Interactive = interactive; }   ///< @brief �����Ƿ�ɽ���
		bool IsInteractive() const { return m_Interactive; }                     ///< @brief ��ȡ�Ƿ�ɽ���
		
		// --- ���ķ��� ---
		bool HandleInput() override;
		void Render() override;

	protected:
		bool m_Interactable = true;
		bool m_IsMouseOver = false;
		Ref<Hazel::UIState> m_CurrentState;     ///< @brief ��ǰ״̬
		std::unordered_map<std::string, Ref<Sprite>> m_Sprites; ///< @brief ���鼯��
		std::unordered_map<std::string, std::string> m_Sounds;   ///< @brief ��Ч���ϣ�keyΪ��Ч���ƣ�valueΪ��Ч�ļ�·��
		Ref<Sprite> m_CurrentSprite = nullptr;      ///< @brief ��ǰ��ʾ�ľ���
		bool m_Interactive = true;                               ///< @brief �Ƿ�ɽ���

	};
}

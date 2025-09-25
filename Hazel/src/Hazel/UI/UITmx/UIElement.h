#pragma once
#include "Hazel.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Hazel/Math/Math.h"

namespace Hazel
{

	class UIElement
	{
	public:
		/**
		 * @brief ����UIElement
		 * @param position ��ʼ�ֲ�λ��
		 * @param size ��ʼ��С
		 */
		explicit UIElement(glm::vec2 position = { 0.0f, 0.0f }, glm::vec2 size = { 0.0f, 0.0f });

		/**
		 * @brief ������������ȷ����������ȷ����
		 */
		virtual ~UIElement() = default;

		// --- ������ѭ������ --- (û��ʹ��init��clean��ע�⹹�캯��������������ʹ��)
		virtual bool HandleInput();
		virtual void Update(float delta_time);
		virtual void Render();

		// --- ��νṹ���� ---
		void AddChild(Scope<UIElement> child);                ///< @brief �����Ԫ��
		Scope<UIElement> RemoveChild(UIElement* childPtr);    ///< @brief ��ָ����Ԫ�ش��б����Ƴ���������������ָ��
		void RemoveAllChildren();                             ///< @brief �Ƴ�������Ԫ��

		// --- Getters and Setters ---
		const glm::vec2& GetSize() const { return m_Size; }              ///< @brief ��ȡԪ�ش�С
		const glm::vec2& GetPosition() const { return m_Position; }      ///< @brief ��ȡԪ��λ��(����ڸ��ڵ�)
		bool IsVisible() const { return m_Visible; }                     ///< @brief ���Ԫ���Ƿ�ɼ�
		bool IsNeedRemove() const { return m_NeedRemove; }               ///< @brief ���Ԫ���Ƿ���Ҫ�Ƴ�
		UIElement* GetParent() const { return m_Parent; }                ///< @brief ��ȡ��Ԫ��
		const std::vector<Scope<UIElement>>& GetChildren() const { return m_Children; } ///< @brief ��ȡ��Ԫ���б�

		void SetSize(const glm::vec2& size) { m_Size = size; }           ///< @brief ����Ԫ�ش�С
		void SetVisible(bool visible) { m_Visible = visible; }           ///< @brief ����Ԫ�صĿɼ���
		void SetParent(UIElement* parent) { m_Parent = parent; }         ///< @brief ���ø��ڵ�
		void SetPosition(const glm::vec2& position) { m_Position = position; }   ///< @brief ����Ԫ��λ��(����ڸ��ڵ�)
		void SetNeedRemove(bool needRemove) { m_NeedRemove = needRemove; }    ///< @brief ����Ԫ���Ƿ���Ҫ�Ƴ�

		// --- �������� ---
		Math::Rect GetBounds() const;                          ///< @brief ��ȡ(����)Ԫ�صı߽�(��Ļ����)
		glm::vec2 GetScreenPosition() const;                   ///< @brief ��ȡ(����)Ԫ������Ļ��λ��
		bool IsPointInside(const glm::vec2& point) const;      ///< @brief ���������Ƿ���Ԫ�صı߽���

		// --- ���ÿ������ƶ����� ---
		UIElement(const UIElement&) = delete;
		UIElement& operator=(const UIElement&) = delete;
		UIElement(UIElement&&) = delete;
		UIElement& operator=(UIElement&&) = delete;

	protected:
		glm::vec2 m_Position;                                    ///< @brief ����ڸ�Ԫ�صľֲ�λ��
		glm::vec2 m_Size;                                        ///< @brief Ԫ�ش�С
		bool m_Visible = true;                                   ///< @brief Ԫ�ص�ǰ�Ƿ�ɼ�
		bool m_NeedRemove = false;                               ///< @brief �Ƿ���Ҫ�Ƴ�(�ӳ�ɾ��)

		UIElement* m_Parent = nullptr;                           ///< @brief ָ�򸸽ڵ�ķ�ӵ��ָ��
		std::vector<Scope<UIElement>> m_Children;                ///< @brief ��Ԫ���б�(����)
	};

}

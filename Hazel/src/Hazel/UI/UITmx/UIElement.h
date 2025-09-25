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
		 * @brief 构造UIElement
		 * @param position 初始局部位置
		 * @param size 初始大小
		 */
		explicit UIElement(glm::vec2 position = { 0.0f, 0.0f }, glm::vec2 size = { 0.0f, 0.0f });

		/**
		 * @brief 虚析构函数，确保派生类正确清理
		 */
		virtual ~UIElement() = default;

		// --- 核心虚循环方法 --- (没有使用init和clean，注意构造函数和析构函数的使用)
		virtual bool HandleInput();
		virtual void Update(float delta_time);
		virtual void Render();

		// --- 层次结构管理 ---
		void AddChild(Scope<UIElement> child);                ///< @brief 添加子元素
		Scope<UIElement> RemoveChild(UIElement* childPtr);    ///< @brief 将指定子元素从列表中移除，并返回其智能指针
		void RemoveAllChildren();                             ///< @brief 移除所有子元素

		// --- Getters and Setters ---
		const glm::vec2& GetSize() const { return m_Size; }              ///< @brief 获取元素大小
		const glm::vec2& GetPosition() const { return m_Position; }      ///< @brief 获取元素位置(相对于父节点)
		bool IsVisible() const { return m_Visible; }                     ///< @brief 检查元素是否可见
		bool IsNeedRemove() const { return m_NeedRemove; }               ///< @brief 检查元素是否需要移除
		UIElement* GetParent() const { return m_Parent; }                ///< @brief 获取父元素
		const std::vector<Scope<UIElement>>& GetChildren() const { return m_Children; } ///< @brief 获取子元素列表

		void SetSize(const glm::vec2& size) { m_Size = size; }           ///< @brief 设置元素大小
		void SetVisible(bool visible) { m_Visible = visible; }           ///< @brief 设置元素的可见性
		void SetParent(UIElement* parent) { m_Parent = parent; }         ///< @brief 设置父节点
		void SetPosition(const glm::vec2& position) { m_Position = position; }   ///< @brief 设置元素位置(相对于父节点)
		void SetNeedRemove(bool needRemove) { m_NeedRemove = needRemove; }    ///< @brief 设置元素是否需要移除

		// --- 辅助方法 ---
		Math::Rect GetBounds() const;                          ///< @brief 获取(计算)元素的边界(屏幕坐标)
		glm::vec2 GetScreenPosition() const;                   ///< @brief 获取(计算)元素在屏幕上位置
		bool IsPointInside(const glm::vec2& point) const;      ///< @brief 检查给定点是否在元素的边界内

		// --- 禁用拷贝和移动语义 ---
		UIElement(const UIElement&) = delete;
		UIElement& operator=(const UIElement&) = delete;
		UIElement(UIElement&&) = delete;
		UIElement& operator=(UIElement&&) = delete;

	protected:
		glm::vec2 m_Position;                                    ///< @brief 相对于父元素的局部位置
		glm::vec2 m_Size;                                        ///< @brief 元素大小
		bool m_Visible = true;                                   ///< @brief 元素当前是否可见
		bool m_NeedRemove = false;                               ///< @brief 是否需要移除(延迟删除)

		UIElement* m_Parent = nullptr;                           ///< @brief 指向父节点的非拥有指针
		std::vector<Scope<UIElement>> m_Children;                ///< @brief 子元素列表(容器)
	};

}
